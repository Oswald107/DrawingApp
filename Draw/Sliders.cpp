#include "Sliders.h"
#include"imgui/imgui_impl_glfw.h"
#include"imgui/imgui_impl_opengl3.h"

Sliders::Sliders(GLFWwindow* window)
{
	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void Sliders::addSliders(Color* color, Pen* pen, Camera* camera) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Hello");
	if (ImGui::SliderFloat("R", &(*color).red, 0.0f, 1.0f, "Value = %.2f")) {

	}
	if (ImGui::SliderFloat("G", &(*color).green, 0.0f, 1.0f, "Value = %.2f")) {

	}
	if (ImGui::SliderFloat("B", &(*color).blue, 0.0f, 1.0f, "Value = %.2f")) {

	}
	if (ImGui::SliderFloat("Opacity", &(*color).alpha, 0.0f, 1.0f, "Value = %.1f")) {

	}
	if (ImGui::SliderFloat("Size", &(*pen).radius, 0.0f, 100.0f, "Value = %.2f")) {

	}
	if (ImGui::SliderFloat("Hue", &(*color).hue, 0.0f, 100.0f, "Hue = %.1f")) {
		(*color).recalculateHue();
	}
	if (ImGui::SliderFloat("Value", &(*color).value, 0.0f, 100.0f, "Value = %.1f")) {
		(*color).recalculateValue();
	}
	if (ImGui::SliderFloat("Saturation", &(*color).saturation, 0.0f, 100.0f, "Saturationn = %.1f")) {
		(*color).recalculateSaturation();
	}
	if (ImGui::SliderFloat("Scale", &(*camera).scale, 1.0f, 10.0f, "Scale = %.1f")) {
		(*camera).update();
	}
	ImGui::Text("ImGui is working!");
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}