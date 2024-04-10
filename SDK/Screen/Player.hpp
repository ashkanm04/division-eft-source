#pragma once

#include "Screen.hpp"

#include "../../Dependencies/Window/Vendor/Renderer/Renderer.hpp"

namespace Screen
{
	class Player
	{
	private:
	public:
		Player(World::Player Player, const UnityEngine::Matrix& ViewMatrix, const Math::Vector3& CameraPosition)
		{
			if (!Player.Valid)
			{
				this->Valid = false;
				return;
			}

			if (!UnityEngine::Camera::WorldToScreen(Player.Upper, &this->Upper, ViewMatrix))
			{
				this->Valid = false;
				return;
			}
			
			
			if (!UnityEngine::Camera::WorldToScreen(Player.Lower, &this->Lower, ViewMatrix))
			{
				this->Valid = false;
				return;
			}
			
			this->Size.y = this->Upper.y - this->Lower.y;
			this->Size.x = this->Size.y / 2.00f;
			
			Math::Vector2 Screen = Math::Vector2();
			for(std::size_t Index = 0; Index < Player.BoneList.size(); Index++)
			{
				if (!UnityEngine::Camera::WorldToScreen(Player.BoneList[Index], &Screen, ViewMatrix))
				{
					this->Valid = false;
					return;
				}
				this->BoneList.push_back(Screen);
			}

			this->Name = Player.IsScav ? (Player.IsAI ? _("Scav") : _("Player Scav")) : Player.Name;
			this->ToCamera = Player.Lower.Distance(CameraPosition);
			this->Distance = std::to_string((int)this->ToCamera) + _("m");


			this->Health_Status = (Player.etagstatus == 1024) ? _("Healthy") :
				(Player.etagstatus == 2048) ? _("Injured") :
				(Player.etagstatus == 4096) ? _("Badly Injured") :
				(Player.etagstatus == 8192) ? _("Dying") :
				_("Dead");  

		}

		void Render(ImDrawList* Renderer)
		{
			if (!this->Valid)
			{
				return;
			}

			if (CONFIG_GET(bool, Skeleton))
			{
				Renderer::Skeleton(Renderer, this->BoneList, ImColor(1.00f, 1.00f, 1.00f, 1.00f));
			}

			if (CONFIG_GET(bool, Box))
			{
				Renderer::BorderBox(Renderer, this->Lower, this->Size, ImColor(1.00f, 1.00f, 1.00f, 1.00f));
			}

			if (CONFIG_GET(bool, Nickname))
			{
				ImVec2 NameSize = ImGui::CalcTextSize(this->Name.c_str());
				Renderer::Text(Renderer, Math::Vector2(this->Lower.x - NameSize.x / 2, this->Upper.y - NameSize.y - 1.00f), ImColor(1.00f, 1.00f, 1.00f, 1.00f), this->Name);
			}

			if (CONFIG_GET(bool, Distance))
			{
				ImVec2 DistanceSize = ImGui::CalcTextSize(this->Distance.c_str());
				Renderer::Text(Renderer, Math::Vector2(this->Lower.x - DistanceSize.x / 2, this->Lower.y + 1.00f), ImColor(1.00f, 1.00f, 1.00f, 1.00f), this->Distance);
			}
			if (CONFIG_GET(bool, HealthStatus))
			{
				ImVec2 Health_StatusSize = ImGui::CalcTextSize(this->Health_Status.c_str());
				Renderer::Text(Renderer, Math::Vector2(this->Lower.x - Health_StatusSize.x / 2, this->Lower.y + 10.00f), ImColor(1.00f, 1.00f, 1.00f, 1.00f), this->Health_Status);
			}
		}

		std::vector<Math::Vector2> BoneList;
		Math::Vector2 Upper;
		Math::Vector2 Lower;
		Math::Vector2 Size;
		std::string Name;

		std::string Distance;
		std::string Health_Status;
		float ToCamera;

		bool Valid = true;
	};
}