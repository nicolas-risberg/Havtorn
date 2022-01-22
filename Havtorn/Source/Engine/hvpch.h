// Copyright 2022 Team Havtorn. All Rights Reserved.

#pragma once

#include <cassert>

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <deque>

#include <d3d11.h>

#ifdef HV_PLATFORM_WINDOWS
#include "Core/WindowsInclude.h"
#endif

#include "Core/EngineTypes.h"
#include "Core/MathTypes/HavtornMath.h"
#include "Core/EngineException.h"
#include "Timer.h"
#include "Log.h"
#include "ECS/ECS.h"
