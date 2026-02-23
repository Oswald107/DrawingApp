
#include<filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"imgui/imgui.h"
#include"imgui/imconfig.h"
#include"imgui/imgui_impl_glfw.h"
#include"imgui/imgui_impl_opengl3.h"

#include"ShaderClass.h"
#include"Vertex.h"
#include"Callbacks.h"
#include"Stats.h"
#include"Color.h"
#include"Camera.h"
#include"Pen.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int width = 800;
const unsigned int height = 800;

void im(Color* color, Pen* pen, Camera* camera) {
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

void save(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		std::vector<unsigned char> pixels(width * height * 3);

		// Read pixels from OpenGL framebuffer
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

		// Flip vertically (OpenGL's origin is bottom-left)
		std::vector<unsigned char> flipped(width * height * 3);
		for (int y = 0; y < height; ++y)
		{
			memcpy(
				&flipped[y * width * 3],
				&pixels[(height - 1 - y) * width * 3],
				width * 3
			);
		}

		// Write PNG
		stbi_write_png("output.png", width, height, 3, flipped.data(), width * 3);
	}
}

int main()
{
	// BASIC STUFF
	// Initialize GLFW
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Init GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW!" << "\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	

	// SHADER
	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();


	// VAO, VBO SETUP
	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO1, VBO1;
	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO1);
	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// FBO
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	GLuint colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

	// Check completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FBO not complete!\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// METRICS
	Stats stats;


	// IMGUI
	// Create window with graphics context
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


	// ALPHA BLENDING
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	


	// ETC
	Color color;
	float radius = 5;
	std::vector<std::vector<Vertex>> circles;
	std::vector<std::vector<Vertex>> lines;
	std::vector<std::vector<Vertex>> redo;
	std::vector<std::vector<Vertex>> erase;
	double prevMouseX, prevMouseY, curMouseX, curMouseY;
	glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
	curMouseX = prevMouseX;
	curMouseY = prevMouseY;
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 1.0f));
	float scale = 1.0f;
	Pen pen(&color);
	bool held = false;
	bool held2 = false;


	int profile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);

	if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
		std::cout << "Core profile\n";
	if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		std::cout << "Compatibility profile\n";
	
	// MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		// UPDATE DIMENSIONS
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		if (camera.width != width and camera.height != height)
		{
			camera.width = width;
			camera.height = height;
			camera.update();
		}
		// METRICS
		stats.showStats(window);
		// PEN INPUTS
		pen.Inputs(window, camera);

		//FBO
		//glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Recompile("default.vert", "default.frag");
		shaderProgram.Activate();
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO1);
		
		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(shaderProgram, "camMatrix");


		//glEnable(GL_BLEND);
		// DRAW
		for (std::vector<Vertex> circle : pen.circles) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(Vertex), circle.data(), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_FAN, 0, circle.size());
		}

		for (std::vector<Vertex> line : pen.lines) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			glBufferData(GL_ARRAY_BUFFER, line.size() * sizeof(Vertex), line.data(), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, line.size());
		}

		std::cout << pen.erase.size() << "\n";

		//glDisable(GL_BLEND);
		for (std::vector<Vertex> e : pen.erase) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			glBufferData(GL_ARRAY_BUFFER, e.size() * sizeof(Vertex), e.data(), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_FAN, 0, e.size());
		}

		//glEnable(GL_BLEND);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, pen.currentLine.size() * sizeof(Vertex), pen.currentLine.data(), GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, pen.currentLine.size());


		
		// SAVE FUNCTION
		save(window);

		// Done drawing to texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// IMGUI SLIDERS
		im(&color, &pen, &camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		// If erasing:
		//glDisable(GL_BLEND);
		/*setShaderColor(bgR, bgG, bgB, 1.0f);
		drawEraseCircle();*/



		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, colorTex);
		/*drawFullscreenQuad();*/



		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	shaderProgram.Delete();
	glDeleteFramebuffers(1, &FBO);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

