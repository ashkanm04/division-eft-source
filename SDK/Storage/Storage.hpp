#pragma once

#include "../SDK.h"

class CStorage
{
private:
public:
    Authentication::Info* Info = nullptr;

    Math::Vector2 ScreenSize = Math::Vector2(0, 0);

    std::uint64_t UnityPlayer = 0x00;
    std::uint64_t MonoDll = 0x00;
    bool InGame = false;
    bool Chinese = false;

    Math::Vector2 RadarPosition = Math::Vector2(100, 100);

    std::list<std::uint64_t> FriendList = {};
    std::list<std::uint64_t> EnemyList = {};
    std::list<std::uint64_t> CheaterList = {};
};

inline CStorage* Storage = new CStorage();