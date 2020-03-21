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

#include "libraries/xorstr.h"
#include "wrapper/wrapper.h"

namespace sdk {

	struct vec3d {
		double x, y, z;
	};

	using qword = unsigned long long;
	using t_createdvms = jint(__stdcall*)(JavaVM**, jsize, jsize*);
};