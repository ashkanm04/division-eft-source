#pragma once

#include "../Cache.hpp"

#include "Object.hpp"
#include <set>
std::string getDocumentsPath() {
	PWSTR pszPath;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pszPath))) {
		std::wstring wstrPath(pszPath);
		CoTaskMemFree(pszPath);
		std::string strPath(wstrPath.begin(), wstrPath.end());
		return strPath;
	}
	return "";
}
std::vector<Item> femboygamink;
std::vector<std::string> AllTags = { "Barter", "Keys", "Weapon", "Ammo", "Gear", "Meds", "Currency", "Weapon_parts", "Provisions", "Suppressors", "Tactical_devices","Containers" };
std::vector<std::string> Allitems = {  };
const std::vector<std::string> containers = { "body", "XXXcap", "Ammo_crate_Cap", "Grenade_box_Door", "Medical_Door", "Toolbox_Door", "card_file_box", "cover", "lootable", "scontainer_Blue_Barrel_Base_Cap", "scontainer_wood_CAP", "suitcase_plastic_lootable_open", "weapon_box_cover", "container_crete_04_COLLIDER(1)" };
std::string executablePath = getDocumentsPath();
std::string filePath = executablePath + "\\loot.txt";
const char* GetFormattedPrice(float value)
{
	if (value >= 1000 && value < 10000)
	{
		static char buffer[32];
		snprintf(buffer, sizeof(buffer), "%.1fk", value / 1000.0f);
		return buffer;
	}
	else if (value >= 10000 && value < 1000000)
	{
		static char buffer[32];
		snprintf(buffer, sizeof(buffer), "%.0fk", value / 1000.0f);
		return buffer;
	}
	else if (value >= 1000000)
	{
		static char buffer[32];
		snprintf(buffer, sizeof(buffer), "%.1fm", value / 1000000.0f);
		return buffer;
	}
	static char buffer[32];
	snprintf(buffer, sizeof(buffer), "%.0f", value);
	return buffer;
}
std::vector<Item> parseItems(const std::string& jsonStr) {
	std::vector<Item> items;
	try {
		auto jsonData = nlohmann::json::parse(jsonStr);

		if (!jsonData.is_array()) {
			std::cout << "Invalid JSON format." << std::endl;
			return items;
		}
		std::vector<std::string> AllItemstempVec = {};

		for (const auto& itemData : jsonData) {
			Item item;
			item.bsgId = itemData["bsgId"];
			item.shortName = itemData["shortName"];
			item.avg24hPrice = itemData["avg24hPrice"];
			item.tags = itemData["tags"].get<std::vector<std::string>>();

			items.push_back(item);
			std::string avgPriceStrr = GetFormattedPrice(item.avg24hPrice);
			std::string pricetoappend = " (" + avgPriceStrr + ")";
			std::string tagtoappend = " (" + item.tags[0] + ")";



			AllItemstempVec.push_back(item.shortName + pricetoappend + tagtoappend);

		}
		Allitems.reserve(AllItemstempVec.size() + 32);//just incase ma niggalo
		Allitems = std::move(AllItemstempVec);
	}
	catch (const nlohmann::json::parse_error& e) {
		std::cout << "Failed to parse JSON data. Error: " << e.what() << std::endl;
	}
	return items;
}
static bool IsLootableObject(std::string objectName)
{
	return (objectName.find("_safe") != std::string::npos ||
		objectName.find("cr_bankrobber") != std::string::npos ||
		objectName.find("_door") != std::string::npos ||
		objectName.find("cover_") != std::string::npos ||
		objectName.find("card_file_box") != std::string::npos ||
		objectName.find("jacket") != std::string::npos ||
		objectName == "cap" ||
		objectName.find("ammo_crate_cap") != std::string::npos ||
		objectName.find("grenade_box_door") != std::string::npos ||
		objectName.find("medical_door") != std::string::npos ||
		objectName.find("pos_money") != std::string::npos ||
		objectName.find("toolbox_door") != std::string::npos ||
		objectName.find("lootable") != std::string::npos ||
		objectName.find("scontainer_blue_barrel_base_cap") != std::string::npos ||
		objectName.find("scontainer_wood_cap") != std::string::npos ||
		objectName.find("suitcase_plastic_lootable_open") != std::string::npos ||
		objectName.find("weapon_box_cover") != std::string::npos ||
		objectName.find("barrel_cache") != std::string::npos ||
		objectName.find("boss_container") != std::string::npos ||
		objectName.find("wood_cache") != std::string::npos ||
		objectName == "safe" ||
		objectName.find("Сейф") != std::string::npos ||
		objectName.find("burried") != std::string::npos ||
		objectName.find("cache") != std::string::npos);
}

std::set<std::string> LoadBsgIds(const std::string& filepath)
{
	std::set<std::string> bsgIds;
	std::ifstream file(filepath);
	std::string line;
	while (std::getline(file, line))
	{
		bsgIds.insert(line);
	}
	return bsgIds;
}


std::vector<Item> loadItemsFromFile(const std::string& filePath) {
	std::vector<Item> items;

	std::ifstream file(filePath);
	if (!file) {
		std::cout << "Failed to open the file." << std::endl;
		return items;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string str = buffer.str();

	if (str.empty()) {
		std::cout << "Failed to read data from the file." << std::endl;
		return items;
	}

	items = parseItems(str);

	if (items.empty()) {
		std::cout << "No items found in the JSON data." << std::endl;
	}

	return items;
}
std::vector<ItemInfo> itemInfoList;
std::vector<ItemInfo> AllitemInfoList;
std::vector<ItemInfo> GetLoot()
{
	itemInfoList.clear();
	std::vector<ItemInfo> TempAllitemInfoList;
	//std::vector<EFTCorpse> TempCorpses;
	uintptr_t LootList = Driver->Read<uintptr_t>(UnityEngine::GameObject::Find<SDK::GameWorld>(_("GameWorld")).Address + Offset::EFT_GameWorld::LootList);
	uintptr_t LootListEntity = Driver->Read<uintptr_t>(LootList + Offset::List::Base);
	int Size = Driver->Read<int>(LootList + Offset::List::Size);//size of list (int)
	for (int i = 0; i < Size; i++)
	{
		uintptr_t LootObjectsEntity = Driver->Read<uintptr_t>(LootListEntity + 0x20 + (0x8 * i));
		uintptr_t GameObject = Driver->ReadChain<uintptr_t>(LootObjectsEntity, { 0x10, 0x28, 0x10, 0x30 });
		uintptr_t PGameObject = Driver->Read<uintptr_t>(GameObject + 0x60);
		std::string GameObjectName = Driver->ReadString(PGameObject, 32);
		uintptr_t TransformPtr = Driver->ReadChain<uintptr_t>(GameObject, { 0x30, 0x8, 0x28 });
		uintptr_t Transform = Driver->Read<uintptr_t>(TransformPtr + 0x10);

		if (IsLootableObject(GameObjectName))
		{																//itemowner -> rootitem -> grids
			uintptr_t list = Driver->ReadChain<uintptr_t>(LootObjectsEntity, { Offset::EFT_Interactive_LootableContainer::ItemOwner, Offset::__GClass223B::item_0xC0, 0x70 }); // (0x10,0xC0,0x70)
			int gridscount = Driver->Read<int>(list + 0x18);

			if (gridscount > 0)
			{														//Grids1->Grids1->unkn
				uintptr_t more = Driver->ReadChain<uintptr_t>(list, { 0x20, 0x40, 0x10 });
				int morecount = Driver->Read<int>(more + 0x40);

				if (morecount > 0)
				{
					uintptr_t entry = Driver->Read<uintptr_t>(more + 0x18);
					for (int i = 0; i < morecount; i++)
					{
						uintptr_t itemInContainer = Driver->Read<uintptr_t>(entry + 0x28 + 0x18 * (uintptr_t)i);
						uintptr_t templateItemInContainer = Driver->Read<uintptr_t>(itemInContainer + 0x40);
						uintptr_t itemIdString = Driver->Read<uintptr_t>(templateItemInContainer + Offset::EFT_InventoryLogic_ItemTemplate::_id);
						std::string StringID = Driver->ReadStringIL2CPP(itemIdString + Offset::String::Name);
						auto iter = std::find_if(femboygamink.begin(), femboygamink.end(), [&](const Item& item) {return item.bsgId == StringID; });

						if (iter != femboygamink.end())
						{
							std::string itemText = iter->shortName;
							std::string avgPriceStr = GetFormattedPrice(iter->avg24hPrice); // cast this hoe to an int so you ain gotta have that .00000000000000 shit
							itemText += " (" + avgPriceStr + ")";
							itemText += " (" + iter->tags[0] + ")";
							ItemInfo itemInfo;
							itemInfo.shortName = iter->shortName;
							itemInfo.avg24hPrice = iter->avg24hPrice;
							itemInfo.tags = iter->tags;
							itemInfo.transform = Transform;
							TempAllitemInfoList.push_back(itemInfo);

							if (ConfigSystem.feature.MinLootPrice < iter->avg24hPrice &&
								(std::find(ConfigSystem.feature.SelctedTags.begin(), ConfigSystem.feature.SelctedTags.end(), iter->tags[0]) != ConfigSystem.feature.SelctedTags.end()) ||
								std::find(ConfigSystem.feature.SelctedItems.begin(), ConfigSystem.feature.SelctedItems.end(), itemText) != ConfigSystem.feature.SelctedItems.end())
							{


								itemInfoList.push_back(itemInfo);
							}
						}
					}
				}
			}
		}
		else
		{

			uintptr_t itemIdString1 = Driver->Read<uintptr_t>(LootObjectsEntity + 0x10);
			uintptr_t itemIdString2 = Driver->Read<uintptr_t>(itemIdString1 + 0x28);
			uintptr_t itemIdString = Driver->Read<uintptr_t>(LootObjectsEntity + Offset::EFT_Interactive_LootItem::TemplateId);
			//int stringSize = Driver->Read<int>(itemIdString + _String::Size);
			std::string StringID = Driver->ReadStringIL2CPP(itemIdString + Offset::String::Name);

			if (StringID == "55d7217a4bdc2d86028b456d")
			{
				//TempCorpses.push_back(EFTCorpse(Transform, "Corpse"));
			}
			auto iter = std::find_if(femboygamink.begin(), femboygamink.end(), [&](const Item& item) {return item.bsgId == StringID; });
			if (iter != femboygamink.end())
			{
				std::string itemText = iter->shortName;
				std::string avgPriceStr = GetFormattedPrice(iter->avg24hPrice); // cast this hoe to an int so you ain gotta have that .00000000000000 shit
				itemText += " (" + avgPriceStr + ")";
				itemText += " (" + iter->tags[0] + ")";
				ItemInfo itemInfo;
				itemInfo.shortName = iter->shortName;
				itemInfo.avg24hPrice = iter->avg24hPrice;
				itemInfo.tags = iter->tags;
				itemInfo.transform = Transform;
				TempAllitemInfoList.push_back(itemInfo);

				if (ConfigSystem.feature.MinLootPrice < iter->avg24hPrice &&
					(std::find(ConfigSystem.feature.SelctedTags.begin(), ConfigSystem.feature.SelctedTags.end(), iter->tags[0]) != ConfigSystem.feature.SelctedTags.end()) ||
					std::find(ConfigSystem.feature.SelctedItems.begin(), ConfigSystem.feature.SelctedItems.end(), itemText) != ConfigSystem.feature.SelctedItems.end())
				{


					itemInfoList.push_back(itemInfo);
				}
			}
		}
	}
	{
		AllitemInfoList = TempAllitemInfoList;
		//Corpses = TempCorpses;
		TempAllitemInfoList.clear();
	}
	return itemInfoList;
}
std::vector<ItemInfo> LootList;


struct TransformAccessReadOnly
{
	uint64_t pTransformData;
	int index;
};

struct TransformData
{
	uint64_t pTransformArray;
	uint64_t pTransformIndices;
};

struct Matrix34
{
	Math::Vector4 vec0;
	Math::Vector4 vec1;
	Math::Vector4 vec2;
};






Math::Vector3 GetPosition(uintptr_t transform)
{
	__m128 result;

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	TransformAccessReadOnly pTransformAccessReadOnly = Driver->Read<TransformAccessReadOnly>(transform + 0x38);
	unsigned int index = Driver->Read<unsigned int>(transform + 0x40);
	TransformData transformData = Driver->Read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

	if (transformData.pTransformArray && transformData.pTransformIndices)
	{
		result = Driver->Read<__m128>(transformData.pTransformArray + (uint64_t)0x30 * index);
		int transformIndex = Driver->Read<int>(transformData.pTransformIndices + (uint64_t)0x4 * index);
		int pSafe = 0;
		while (transformIndex >= 0 && pSafe++ < 200)
		{
			Matrix34 matrix34 = Driver->Read<Matrix34>(transformData.pTransformArray + (uint64_t)0x30 * transformIndex);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) & matrix34.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) & matrix34.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) & matrix34.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) & matrix34.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) & matrix34.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_load_si128((__m128i*) & matrix34.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

			result = _mm_add_ps(
				_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), _mm_load_ps(&matrix34.vec0.x));

			transformIndex = Driver->Read<int>(transformData.pTransformIndices + 0x4 * transformIndex);
		}
	}

	return Math::Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
}
std::vector<CachedItemInfo> WorldItemInfo;
namespace EFT
{
	namespace Cache
	{
		class CInstance
		{
		private:
			void Update()
			{
				while (true)
				{
					Sleep(50);
					std::vector<CachedItemInfo> cachedItemInfo;
					this->GameWorld->Update(UnityEngine::GameObject::Find<SDK::GameWorld>(_("GameWorld")).Address);
					this->LocalPlayer->Update(this->GameWorld->Data->Get().GetLocalPlayer().Address);
					this->Camera->Update(UnityEngine::Camera::Find(_("FPS Camera")).Address);
					if (femboygamink.empty()) {
						femboygamink = loadItemsFromFile(filePath);
					}
					LootList = GetLoot();
					for (const auto& item : LootList)
					{
						if (item.shortName.empty())
							continue;
						UnityEngine::Transform Transform;
						Math::Vector3 LootPos = GetPosition(item.transform);

						if (!LootPos.IsValid())
							continue;

						cachedItemInfo.push_back(CachedItemInfo(item, LootPos));
					}
					WorldItemInfo = cachedItemInfo;
				}
			}
		public:
			InstanceObject<UnityEngine::Camera>* Camera = new InstanceObject<UnityEngine::Camera>();
			InstanceObject<SDK::GameWorld>* GameWorld = new InstanceObject<SDK::GameWorld>();
			InstanceObject<SDK::Player>* LocalPlayer = new InstanceObject<SDK::Player>();

			void Run()
			{
				std::thread([&]() { this->Update(); }).detach();
			}
		};

		inline EFT::Cache::CInstance* Instance = new EFT::Cache::CInstance();
	}
}