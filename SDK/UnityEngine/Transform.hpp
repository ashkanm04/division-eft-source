#pragma once

#include "UnityEngine.h"

namespace UnityEngine
{
	static constexpr __m128 MultiplyVector1 = { -2.000, 2.000, -2.000, 0.000 };
	static constexpr __m128 MultiplyVector2 = { 2.000, -2.000, -2.000, 0.000 };
	static constexpr __m128 MultiplyVector3 = { -2.000, -2.000, 2.000, 0.000 };

	class Transform : public BaseObject
	{
	private:
		struct AccessReadOnly
		{
			std::uint64_t TransformData;
		};

		struct Data
		{
			std::uint64_t TransformArray;
			std::uint64_t TransformIndecies;
		};

		struct Matrix34
		{
			Math::Vector4 Vec1;
			Math::Vector4 Vec2;
			Math::Vector4 Vec3;
		};
	public:
		Transform(const std::uint64_t& Address = 0x00) : BaseObject(Address) {}

		Math::Vector3 GetPosition()
		{
			const std::uint64_t CachedPtr = Driver->Read<std::uint64_t>(this->Address + 0x10);

			if (!CachedPtr)
			{
				return Math::Vector3();
			}

			const AccessReadOnly ReadOnly = Driver->Read<AccessReadOnly>(CachedPtr + 0x38);

			if (!ReadOnly.TransformData)
			{
				return Math::Vector3();
			}

			Data TransformData = Driver->Read<Data>(ReadOnly.TransformData + 0x18);

			if (!TransformData.TransformArray || !TransformData.TransformIndecies)
			{
				return Math::Vector3();
			}

			const int Size = Driver->Read<int>(CachedPtr + 0x40);

			if (!Size || Size > 0x500)
			{
				return Math::Vector3();
			}

			__m128 Result = Driver->Read<__m128>(TransformData.TransformArray + 0x30 * Size);
			int Index = Driver->Read<int>(TransformData.TransformIndecies + 0x4 * Size);
			int Count = 0;

			while (Index >= 0 && Count++ < 200)
			{
				Matrix34 Matrix = Driver->Read<Matrix34>(TransformData.TransformArray + 0x30 * Index);

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&Matrix.Vec2), 0x00));	// xxxx
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&Matrix.Vec2), 0x55));	// yyyy
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&Matrix.Vec2), 0x8E));	// zwxy
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&Matrix.Vec2), 0xDB));	// wzyw
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&Matrix.Vec2), 0xAA));	// zzzz
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&Matrix.Vec2), 0x71));	// yxwy
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&Matrix.Vec3), Result);

				Result = _mm_add_ps(_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, MultiplyVector2), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, MultiplyVector3), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, MultiplyVector3), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, MultiplyVector1), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, MultiplyVector1), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, MultiplyVector2), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&Matrix.Vec1));

				Index = Driver->Read<int>(TransformData.TransformIndecies + 0x4 * Index);
			}

			return Math::Vector3(Result.m128_f32[0], Result.m128_f32[1], Result.m128_f32[2]);
		}
	};
}