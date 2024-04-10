#pragma once

#include <filesystem>

struct Color {
	float r, g, b, a;
};


class c_config final {
public:
	void run(const char*) noexcept;
	void load(size_t) noexcept;
	void save(size_t) const noexcept;
	void add(const char*) noexcept;
	void remove(size_t) noexcept;
	void rename(size_t, const char*) noexcept;
	void reset() noexcept;

	constexpr auto& get_configs() noexcept {
		return configs;
	}

	struct {

		bool PlayerEsp;
		bool PlayerName;
		bool PlayerHealth;
		bool NormalBox;
		bool Healthbar;
		float PlayerTextScale = 1.f;
		float MinPlayerDist = 1000.f;
		bool Skeleton;
		bool ExtractEsp;
		bool Level;
		bool PlayerDist;
		//loot stuff
		bool LootEsp;
		bool LootPriceEsp;
		bool FilterInGameItems;
		bool LootDistance;
		bool LootWindow;
		bool TagEsp;
		bool ABCOrder;
		bool highestprice;
		float LootTextScale = 1.f;
		float MinLootDist = 1000.f;
		float MinLootPrice = 100000;
		bool Extractpoints;
		bool LootThruWall;
		std::vector<std::string> SelctedTags;
		std::vector<std::string> SelctedItems;




		//WEAPON
		bool SilentAim;
		bool AimBot;
		bool ShootThruWall;
		bool FastBullet;
		bool InstantHit;
		bool NoSpread;
		bool NoRecoil;
		bool InfStamina;
		int TargetBone = 1;
		bool Visor;

		bool ShortGun;
		bool HipAds;

		//MISC
		bool InstantSearch;
		bool MaxSkills;
		bool FastLoad;
		bool InstantExamine;
		bool SuperJump;
		bool FreeCam;
		int FreeCamKey;
		//misc shit
		float FovCircleSize = 150.f;
		bool DrawCircleFov = false;

		int AimbotKey;
	} feature;


	struct {
		//Player
		float PlayerName[4] = { 1, 1, 1, 1 };
		float PlayerScav[4] = { 1, 1, 1, 1 };
		float Scav[4] = { 1, 1, 1, 1 };
		float BossScav[4] = { 1, 1, 1, 1 };
		float HealthColor[4] = { 1, 1, 1, 1 };
		float PlayerLevel[4] = { 1, 1, 1, 1 };
		//Player Boxes
		float PlayerBoxColor[4] = { 1, 1, 1, 1 };
		float PlayerScavBoxColor[4] = { 1, 1, 1, 1 };
		float ScavBoxColor[4] = { 1, 1, 1, 1 };
		float BossScavBoxColor[4] = { 1, 1, 1, 1 };

		//Player Skeleton
		float PlayerSkeletonColor[4] = { 1, 1, 1, 1 };
		float PlayerScavSkeletonColor[4] = { 1, 1, 1, 1 };
		float ScavSkeletonColor[4] = { 1, 1, 1, 1 };
		float BossScavSkeletonColor[4] = { 1, 1, 1, 1 };

		//Player Healths
		float PlayerHealthColor[4] = { 1, 1, 1, 1 };
		float PlayerScavHealthColor[4] = { 1, 1, 1, 1 };
		float ScavHealthColor[4] = { 1, 1, 1, 1 };
		float BossScavHealthColor[4] = { 1, 1, 1, 1 };
		// Loot
		float LootName[4] = { 1, 1, 1, 1 };
		float LootPrice[4] = { 1, 1, 1, 1 };
		float LootTag[4] = { 1, 1, 1, 1 };

		// Extracts
		float ExfilPoints[4] = { 1, 1, 1, 1 };
	} color;

	struct {
		float x = 300.f;
		float y = 300.f;
		float range = 160.f;
		float size = 140.f;
	} radar;

	struct {
		float flyhack = 0.f;
		float hor_flyhack = 0.f;
		float max_flyhack = 0.f;
		float max_hor_flyhack = 0.f;
		float flyhackPauseTime = 0.0f;
		float flyhackDistanceVertical = 0.f;
		float flyhackDistanceHorizontal = 0.f;
	} flyhacksettings;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern c_config ConfigSystem;