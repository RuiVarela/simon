#pragma once

//
// standard template library
//
#include <cassert>

#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cstdio>
#include <cctype>
#include <iomanip>
#include <random>
#include <filesystem>

#include <array>
#include <vector>
#include <set>
#include <list>
#include <memory>
#include <algorithm>
#include <chrono>

#include <thread>
#include <condition_variable>
#include <mutex>

#include <cmath>
#include <limits>




//
// vendor
//
#include "../vendor/raylib/raylib.h"
#include "../vendor/tinyformat.h"
#include "../vendor/json.hpp"

//
// Code
//
#include "Lang.h"
#include "Log.h"
#include "Mathematics.h"

#include "Text.h"
#include "File.h"
#include "EasingCurve.h"
#include "JsonParser.h"

#include "MeanBuffer.h"
#include "Timer.h"
#include "Timestamp.h"
#include "Throttler.h"

#include "Barrier.h"
#include "WaitEvent.h"
#include "Worker.h"

#include "Tone.h"
#include "Cell.h"
#include "Button.h"
#include "Menu.h"
#include "Simon.h"