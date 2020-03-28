#include "../gasper.h"

/// Context
std::shared_ptr<wrapper::c_context> gasper::hooks::gl_context = nullptr;

/// Flag for initializing ImGui
std::once_flag flag;

/// Our le epic swap_buffers hook :)
/// NOTE: This is "stream" proof, but flickers. You can fix this by modifying the buffer.
int __stdcall gasper::hooks::swap_buffers_hk(HDC hdc) {

	/// Le retardation has arrived
	static auto mc_window = wrapper::find_window(xorstr_("Minecraft 1.7.10"));
	auto window_rect = wrapper::get_window_rect(xorstr_("Minecraft 1.7.10"));

	/// std::call_once because we're hip
	std::call_once(flag, [&]() {
		gl_context = wrapper::create_gl_context();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(mc_window);
		ImGui_ImplOpenGL2_Init();
		return gasper::hooks::oswap_buffers(hdc);
	});

	/// if for some reason our instance is nullptr'd or we aren't running anymore but this hook still is
	/// make sure to dip
	if (!gasper::instance || !gasper::instance->b_running)
		return gasper::hooks::oswap_buffers(hdc);

	/// Switch to our context
	wglMakeCurrent(gl_context->m_hdc_devicectx, gl_context->m_glrenderctx);

	/// If our menu is open why don't we draw
	/// NOTE: If it's according to phage, a 0fps dip when rendering the menu is too compromising
	/// which is frankly the funniest shit I've read in a while.
	if (gasper::instance->b_open)
	{
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(xorstr_("Gasper C++"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text(xorstr_("Hello, World!"));

		ImGui::End();

		ImGui::Render();
		glViewport(0, 0, window_rect.right, window_rect.top);          
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	/// Switch back
	wglMakeCurrent(gl_context->m_hdc_devicectx, gl_context->m_oglrenderctx);

	/// Call original and dip
	return gasper::hooks::oswap_buffers(hdc);
}
