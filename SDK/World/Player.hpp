#pragma once

#include "World.hpp"


namespace World
{
	class Player
	{
	private:
	public:
		Player(SDK::ObservedPlayerView Entity)
		{
			if (!Entity.Address)
			{
				this->Valid = false;
				return;
			}

			UnityEngine::List TransformList = Entity.GetPlayerBody().GetDizSkinningSkeleton().GetTransformList();
			const std::uint64_t Start = TransformList.GetStart();

			static std::vector<std::uint32_t> IndexList = 
			{ 
				SDK::Player::Bones::Head,
				SDK::Player::Bones::Neck,

				SDK::Player::Bones::LeftUpperarm,
				SDK::Player::Bones::LeftForearm2,
				SDK::Player::Bones::LeftPalm,

				SDK::Player::Bones::RightUpperarm,
				SDK::Player::Bones::RightForearm2,
				SDK::Player::Bones::RightPalm,

				SDK::Player::Bones::Pelvis,

				SDK::Player::Bones::LeftThigh1, 
				SDK::Player::Bones::LeftThigh2, 
				SDK::Player::Bones::LeftToe, 
				
				SDK::Player::Bones::RightThigh1, 
				SDK::Player::Bones::RightThigh2, 
				SDK::Player::Bones::RightToe
			};
			
			for (std::size_t Index = 0; Index < IndexList.size(); Index++)
			{
				UnityEngine::Transform Transform = TransformList.GetClass<UnityEngine::Transform>(Start, IndexList[Index]);

				if (!Transform.Address)
				{
					this->Valid = false;
					return;
				}

				this->BoneList.push_back(Transform.GetPosition());
			}

			this->Upper = this->BoneList[0] + Math::Vector3(0.00f, 0.2f, 0.00f);
			this->Lower = (this->BoneList[14] + this->BoneList[11]) / 2.00f;

			this->IsAlive = Entity.GetObservedPlayerController().GetHealthController().IsAlive();
			this->Name = Entity.GetNickname();
			this->IsScav = Entity.IsScav();
			this->IsAI = Entity.IsAI();
			this->etagstatus = Entity.GetObservedPlayerController().GetHealthController().GetETagStatus();
		}

		std::vector<Math::Vector3> BoneList;
		Math::Vector3 Upper;
		Math::Vector3 Lower;
		std::string Name;
		
		bool IsAlive;
		bool IsScav;
		bool IsAI;
		int etagstatus;
		bool Valid = true;
	};
}





