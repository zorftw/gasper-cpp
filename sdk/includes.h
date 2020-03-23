#pragma once

/*
	Main includes for gasper-cpp, would be a pain to write this constantly wouldn't it?
	
*/

#include <Windows.h>
#include <thread>
#include <memory>
#include <chrono>
#include <thread>
#include <string>

#include <mutex>
#include <functional>

#include <jni.h>
#include <jvmti.h>

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "libraries/xorstr.h"
#include "wrapper/wrapper.h"
#include "utils/utils.h"