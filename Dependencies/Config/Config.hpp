#pragma once
#include <any>
#include <filesystem>
#include <deque>
#include <vector>
#include <fstream>
#include <shlobj.h>

#include "../../Dependencies/Window/Vendor/ImGui/imgui.h"
#include "json/json.hpp"

#define HASH_NAME  "1201096558"
#define HASH_TYPE  "1905268949"
#define HASH_VALUE "1015428498"

using FNV1A_t = std::uint32_t;
namespace FNV1A
{
    inline constexpr std::uint32_t ullBasis = 0x11C9DCD;
    inline constexpr std::uint32_t ullPrime = 0x1000197;

    constexpr FNV1A_t HashConst(const char* szString, const FNV1A_t uValue = ullBasis) noexcept
    {
        return (szString[0] == '\0') ? uValue : HashConst(&szString[1], (uValue ^ FNV1A_t(szString[0])) * ullPrime);
    }

    inline FNV1A_t Hash(const char* szString)
    {
        FNV1A_t uHashed = ullBasis;

        for (std::size_t i = 0U; i < strlen(szString); ++i)
        {
            uHashed ^= szString[i];
            uHashed *= ullPrime;
        }

        return uHashed;
    }
}

#define DEF_VAR( Type, szName, pDefault ) const std::uint32_t szName = Config::AddVariable<Type>(FNV1A::HashConst(#szName), FNV1A::HashConst(#Type), pDefault);

struct SVariableObject
{
    SVariableObject(const FNV1A_t uNameHash, const FNV1A_t uTypeHash, std::any&& pDefault)
        : uNameHash(uNameHash), uTypeHash(uTypeHash), pValue(std::move(pDefault)) { }

    ~SVariableObject() = default;

    template<typename T>
    T& Get()
    {
        return *static_cast<T*>(std::any_cast<T>(&pValue));
    }

    template<typename T>
    void Set(T value)
    {
        pValue.emplace<T>(value);
    }

    FNV1A_t uNameHash = 0x0;
    FNV1A_t uTypeHash = 0x0;
    std::any pValue = { };
};

namespace Config
{
    std::wstring ConfigPath = L"";
    const std::wstring FileExtension = L".cfg";
    inline std::vector<std::string> ConfigList = { };
    inline std::vector<SVariableObject> VariableList = { };

    std::size_t GetVariableIndex(const FNV1A_t uNameHash)
    {
        for (std::size_t i = 0U; i < VariableList.size(); i++)
        {
            if (VariableList.at(i).uNameHash == uNameHash)
                return i;
        }

        return -1;
    }

    bool Save(std::string FileName)
    {
        std::wstring FileNameW = std::wstring(FileName.begin(), FileName.end());
		FileNameW += FileExtension;

        const std::wstring szFile = ConfigPath + FileNameW;
        nlohmann::json config = { };

        for (auto& variable : VariableList)
        {
            nlohmann::json entry = { };

            entry[HASH_NAME] = variable.uNameHash;
            entry[HASH_TYPE] = variable.uTypeHash;

            switch (variable.uTypeHash)
            {
            case FNV1A::HashConst("int"):
            {
                entry[HASH_VALUE] = variable.Get<int>() * 2;
                break;
            }
            case FNV1A::HashConst("float"):
            {
                entry[HASH_VALUE] = variable.Get<float>() * 2.f;
                break;
            }
            case FNV1A::HashConst("bool"):
            {
                entry[HASH_VALUE] = (int)variable.Get<bool>();
                break;
            }
            case FNV1A::HashConst("std::string"):
            {
                entry[HASH_VALUE] = variable.Get<std::string>();
                break;
            }
            case FNV1A::HashConst("ImVec4"):
            {
                const auto& colVariable = variable.Get<ImVec4>();

                nlohmann::json sub = { };

                sub.push_back(colVariable.x * 2.f);
                sub.push_back(colVariable.y * 2.f);
                sub.push_back(colVariable.z * 2.f);
                sub.push_back(colVariable.w * 2.f);

                entry[HASH_VALUE] = sub.dump();
                break;
            }
            case FNV1A::HashConst("ImColor"):
            {
                const auto& colVariable = variable.Get<ImColor>();

                nlohmann::json sub = { };

                sub.push_back(colVariable.Value.x * 2.f);
                sub.push_back(colVariable.Value.y * 2.f);
                sub.push_back(colVariable.Value.z * 2.f);
                sub.push_back(colVariable.Value.w * 2.f);

                entry[HASH_VALUE] = sub.dump();
                break;
            }
            case FNV1A::HashConst("std::vector<bool>"):
            {
                const auto& vecBools = variable.Get<std::vector<bool>>();

                nlohmann::json sub = { };

                for (const auto&& bValue : vecBools)
                {
                    sub.push_back(static_cast<bool>(bValue));
                }

                entry[HASH_VALUE] = sub.dump();
                break;
            }
            case FNV1A::HashConst("std::vector<int>"):
            {
                const auto& vecInts = variable.Get<std::vector<int>>();

                nlohmann::json sub = { };

                for (const auto& iValue : vecInts)
                {
                    sub.push_back(iValue);
                }

                entry[HASH_VALUE] = sub.dump();
                break;
            }
            case FNV1A::HashConst("std::vector<float>"):
            {
                const auto& vecFloats = variable.Get<std::vector<float>>();

                nlohmann::json sub = { };

                for (const auto& flValue : vecFloats)
                {
                    sub.push_back(flValue);
                }

                entry[HASH_VALUE] = sub.dump();
                break;
            }
            case FNV1A::HashConst("std::list<std::uint64_t>"):
            {
                const auto& vecInts = variable.Get<std::list<std::uint64_t>>();

                nlohmann::json sub = { };

                for (const auto& iValue : vecInts)
                {
                    sub.push_back(iValue);
                }

                entry[HASH_VALUE] = sub.dump();
                break;
            }
            default:
                break;
            }

            config.push_back(entry);
        }

        std::ofstream ofsOutFile(szFile, std::ios::out | std::ios::trunc);

        if (!ofsOutFile.good())
            return false;

        ofsOutFile << config.dump(4);
        ofsOutFile.close();

        return true;
    }

    bool Load(std::string FileName)
    {
		std::wstring FileNameW = std::wstring(FileName.begin(), FileName.end());
		FileNameW += FileExtension;
        const std::wstring szFile = ConfigPath + FileNameW;
        nlohmann::json config = { };

        std::ifstream ifsInputFile(szFile, std::ios::in);

        if (!ifsInputFile.good())
            return false;

        config = nlohmann::json::parse(ifsInputFile, nullptr, false);

        if (config.is_discarded())
            return false;

        ifsInputFile.close();

        for (const auto& variable : config)
        {
            const std::size_t nIndex = GetVariableIndex(variable[HASH_NAME].get<FNV1A_t>());

            if (nIndex == -1)
                continue;

            auto& entry = VariableList.at(nIndex);

            switch (variable[HASH_TYPE].get<FNV1A_t>())
            {
            case FNV1A::HashConst("bool"):
            {
                entry.Set<bool>(variable[HASH_VALUE].get<int>());
                break;
            }
            case FNV1A::HashConst("float"):
            {
                entry.Set<float>(variable[HASH_VALUE].get<float>() / 2.f);
                break;
            }
            case FNV1A::HashConst("int"):
            {
                entry.Set<int>(variable[HASH_VALUE].get<int>() / 2);
                break;
            }
            case FNV1A::HashConst("std::string"):
            {
                entry.Set<std::string>(variable[HASH_VALUE].get<std::string>());
                break;
            }
            case FNV1A::HashConst("ImVec4"):
            {
                const nlohmann::json vector = nlohmann::json::parse(variable[HASH_VALUE].get<std::string>());

                ImVec4 clr = {
                    vector.at(0).get<float>() / 2.f,
                    vector.at(1).get<float>() / 2.f,
                    vector.at(2).get<float>() / 2.f,
                    vector.at(3).get<float>() / 2.f
                };

                entry.Set<ImVec4>(clr);

                break;
            }
            case FNV1A::HashConst("ImColor"):
            {
                const nlohmann::json vector = nlohmann::json::parse(variable[HASH_VALUE].get<std::string>());

                ImVec4 clr = {
                    vector.at(0).get<float>() / 2.f,
                    vector.at(1).get<float>() / 2.f,
                    vector.at(2).get<float>() / 2.f,
                    vector.at(3).get<float>() / 2.f
                };

                entry.Set<ImColor>(ImColor(clr));

                break;
            }
            case FNV1A::HashConst("std::vector<bool>"):
            {
                const nlohmann::json vector = nlohmann::json::parse(variable[HASH_VALUE].get<std::string>());
                auto& vecBools = entry.Get<std::vector<bool>>();

                for (std::size_t i = 0U; i < vector.size(); i++)
                {
                    if (i < vecBools.size())
                    {
                        vecBools.at(i) = vector.at(i).get<bool>();
                    }
                }

                break;
            }
            case FNV1A::HashConst("std::vector<int>"):
            {
                const nlohmann::json vector = nlohmann::json::parse(variable[HASH_VALUE].get<std::string>());
                auto& vecInts = entry.Get<std::vector<int>>();

                for (std::size_t i = 0U; i < vector.size(); i++)
                {
                    if (i < vecInts.size())
                    {
                        vecInts.at(i) = vector.at(i).get<int>();
                    }
                }

                break;
            }
            case FNV1A::HashConst("std::vector<float>"):
            {
                const nlohmann::json vector = nlohmann::json::parse(variable[HASH_VALUE].get<std::string>());
                auto& vecFloats = entry.Get<std::vector<float>>();

                for (std::size_t i = 0U; i < vector.size(); i++)
                {
                    if (i < vecFloats.size())
                    {
                        vecFloats.at(i) = vector.at(i).get<float>();
                    }
                }

                break;
            }
            case FNV1A::HashConst("std::list<std::uint64_t>"):
            {
                const nlohmann::json vector = nlohmann::json::parse(variable[HASH_VALUE].get<std::string>());
                auto& vecFloats = entry.Get<std::vector<std::uint64_t>>();

                for (std::size_t i = 0U; i < vector.size(); i++)
                {
                    if (i < vecFloats.size())
                    {
                        vecFloats.at(i) = vector.at(i).get<std::uint64_t>();
                    }
                }

                break;
            }
            default:
                break;
            }
        }

        return true;
    }

    void Refresh()
    {
        std::vector<std::string> FileNames = {};

        std::wstring DefaultName = L"default" + FileExtension;

        for (const auto& it : std::filesystem::directory_iterator(ConfigPath))
        {
            std::wstring Name = it.path().filename().wstring();
            if (it.path().filename().extension().wstring() == FileExtension && Name != DefaultName)
            {
                Name.erase(Name.find(FileExtension), FileExtension.length());
                FileNames.push_back(std::string(Name.begin(), Name.end()));
            }
        }
        ConfigList = FileNames;
        FileNames.clear();
    }

    bool Setup()
    {
        ConfigPath = std::wstring(_wgetenv(L"USERPROFILE")) + L"\\Documents\\division\\configs\\";

        if (!std::filesystem::is_directory(ConfigPath))
        {
            std::filesystem::remove(ConfigPath);
            if (!std::filesystem::create_directories(ConfigPath))
            {
                return false;
            }
        }

        if (!Save("default"))
        {
            return false;
        }

        if (!Load("default"))
        {
            return false;
        }

        Refresh();

        return true;
    }

    void Remove(std::string FileName)
    {
        if (FileName == ("default"))
        {
            return;
        }

        std::wstring FileNameW = std::wstring(FileName.begin(), FileName.end());
		FileNameW += FileExtension;

        std::wstring Path = ConfigPath + FileNameW;

        _wremove(Path.c_str());

        Refresh();
    }

    template <typename T>
    T& Get(const std::uint32_t nIndex)
    {
        return VariableList.at(nIndex).Get<T>();
    }

    template <typename T>
    std::uint32_t AddVariable(const FNV1A_t uNameHash, const FNV1A_t uTypeHash, const T pDefault)
    {
        VariableList.emplace_back(uNameHash, uTypeHash, std::make_any<T>(pDefault));
        return VariableList.size() - 1U;
    }
}