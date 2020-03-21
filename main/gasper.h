#pragma once

#include "../sdk/includes.h"

namespace gasper {
	class c_gasper {
	private:
		JNIEnv* env;
		JavaVM* vm;

		jmethodID findclass_md;
		jobject classloader_obj;
	public:
		/// If we're still running
		bool b_running;

		/// Function to try and attach to the JVM
		bool attach();

		/// Run the cheat 
		void run();

		/// Dispose of cheat
		void dispose();

		/// Get the launchwrapper for forge support 
		void get_launchwrapper();

		const auto get_env() {
			return env;
		}

		const auto get_vm() {
			return vm;
		}

		/// Inline function to get class
		inline jclass find_class(const char* name) {
			jstring jname = get_env()->NewStringUTF(name);

			jclass cls = reinterpret_cast<jclass>(get_env()->CallObjectMethod(classloader_obj, findclass_md, jname));

			get_env()->DeleteLocalRef(jname);

			wrapper::handle_issue(name, cls);

			return cls;
		}
	};

	extern std::unique_ptr<c_gasper> instance;
}
