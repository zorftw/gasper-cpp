#include "../gasper.h"

/// Include cheats for settings
#include "../cheats/impl/headers/reach.h"
#include "../cheats/impl/headers/aimbot.h"
#include "../cheats/impl/headers/strafe.h"

/// Context
std::shared_ptr<wrapper::c_context> gasper::hooks::gl_context = nullptr;

/// Flag for initializing ImGui
std::once_flag flag;

/// Set our colors
void set_colors();

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
		set_colors();
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
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = true;

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(xorstr_("Gasper C++"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Checkbox(xorstr_("Reach enabled"), &reach::m_enabled);
		ImGui::SliderFloat(xorstr_("Distance"), &reach::m_reach, 0.0f, 1.5f, xorstr_("%.1f"));
		ImGui::Checkbox(xorstr_("AimAssist enabled"), &aimbot::m_enabled);
		ImGui::SliderFloat(xorstr_("Speed"), &aimbot::m_speed, 1.0f, 10.0f, xorstr_("%.1f"));
		ImGui::SliderFloat(xorstr_("Speed on Body"), &aimbot::m_speed_body, 1.0f, 10.0f, xorstr_("%.1f"));
		ImGui::SliderFloat(xorstr_("FOV"), &aimbot::m_fov, 10.0f, 90.0f, xorstr_("%.1f"));
		ImGui::Checkbox(xorstr_("Strafe"), &strafe::m_enabled);

		ImGui::End();

		ImGui::Render();
		glViewport(0, 0, window_rect.right, window_rect.top);          
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}

	ImGuiIO& io = ImGui::GetIO();
	io.MouseDrawCursor = false;

	/// Switch back
	wglMakeCurrent(gl_context->m_hdc_devicectx, gl_context->m_oglrenderctx);

	/// Call original and dip
	return gasper::hooks::oswap_buffers(hdc);
}

void set_colors()
{
	ImColor mainColor = ImColor(int(15), int(135), int(105), 255);
	ImColor bodyColor = ImColor(int(24), int(24), int(24), 255);
	ImColor fontColor = ImColor(int(255), int(255), int(255), 255);

	ImGuiStyle& style = ImGui::GetStyle();

	ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
	ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
	ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
	ImVec4 frameBgColor = ImVec4(bodyColor.Value.x + 0.1f, bodyColor.Value.y + 0.1f, bodyColor.Value.z + 0.1f, bodyColor.Value.w + .1f);
	ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildRounding = 0.0f;
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 2.0f;
	style.ItemSpacing = ImVec2(4, 3);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 8.f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 1.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;

	style.Colors[ImGuiCol_Text] = fontColor;

	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = bodyColor;
	style.Colors[ImGuiCol_ChildBg] = ImVec4(.0f, .0f, .0f, .0f);
	style.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
	style.Colors[ImGuiCol_Border] = ImColor(200, 200, 200, 220);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = frameBgColor;
	style.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
	style.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
	style.Colors[ImGuiCol_TitleBg] = mainColor;
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = mainColor;
	style.Colors[ImGuiCol_MenuBarBg] = menubarColor;
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .05f, frameBgColor.y + .05f, frameBgColor.z + .05f, frameBgColor.w);
	style.Colors[ImGuiCol_ScrollbarGrab] = mainColor;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = mainColorHovered;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = mainColorActive;
	style.Colors[ImGuiCol_CheckMark] = mainColor;
	style.Colors[ImGuiCol_SliderGrab] = mainColorHovered;
	style.Colors[ImGuiCol_SliderGrabActive] = mainColorActive;
	style.Colors[ImGuiCol_Button] = mainColor;
	style.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
	style.Colors[ImGuiCol_ButtonActive] = mainColorActive;
	style.Colors[ImGuiCol_Header] = mainColor;
	style.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
	style.Colors[ImGuiCol_HeaderActive] = mainColorActive;


	style.Colors[ImGuiCol_ResizeGrip] = mainColor;
	style.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
	style.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
	style.Colors[ImGuiCol_PlotLines] = mainColor;
	style.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
	style.Colors[ImGuiCol_PlotHistogram] = mainColor;
	style.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}
