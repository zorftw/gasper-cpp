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

		/// If the menu is open
		bool b_open;

		/// Function to try and attach to the JVM
		bool attach();

		/// Run the cheat 
		void run();

		/// Dispose of cheat
		void dispose();

		/// Get the launchwrapper for forge support 
		void get_launchwrapper();

		/// Hook necessary functions
		void hook();

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

	namespace hooks {
		/// Function template of SwapBuffers
		using swap_buffers_fn = int(__stdcall*)(HDC);

		/// Store pointer to the original SwapBuffers function
		inline swap_buffers_fn oswap_buffers = nullptr;

		/// Simple glcontext wrapper to make life easy
		extern std::shared_ptr<wrapper::c_context> gl_context;

		/// The actual hook
		int __stdcall swap_buffers_hk(HDC);

		/// Original WndProc of the Window we're hooking
		inline WNDPROC original_wndproc = nullptr;

		/// Actual "hook", this really isn't a hook but it's whatever
		long __stdcall wndproc_hk(const HWND hwnd, unsigned int usermsg, uintptr_t wparam, long lparam);
	}

	extern std::unique_ptr<c_gasper> instance;
}
