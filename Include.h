#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <fstream>
#include <thread>
#include <tuple>
#include <vector>
#include "p10Config/Config.h"

#include <functional>
#include <TlHelp32.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

#include "Dependencies/Dependencies.h"

#include "SDK/SDK.h"

#include "EFT/EFT.hpp"

