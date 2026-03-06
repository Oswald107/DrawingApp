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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include"ShaderClass.h"
#include"Vertex.h"
#include"Callbacks.h"
#include"Stats.h"
#include"Color.h"
#include"Camera.h"
#include"Pen.h"
#include"Commands.h"
#include"Sliders.h"
#include"Layer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int width = 800;
const unsigned int height = 800;
const unsigned int drawingSpaceWidth = 700;
const unsigned int drawingSpaceHeight = 700;

GLuint indices[] =
{
	0, 1, 2,
	3, 2, 0
};

float rectangleVertices[] =
{
	// Coords        // TexCoords
	 float(drawingSpaceWidth)/2.0f, -float(drawingSpaceHeight)/2.0f,  1.0f, 0.0f,	1.0f, 0.0f, 0.0f,
	-float(drawingSpaceWidth)/2.0f, -float(drawingSpaceHeight)/2.0f,  0.0f, 0.0f,	0.0f, 1.0f, 0.0f,
	-float(drawingSpaceWidth)/2.0f,  float(drawingSpaceHeight)/2.0f,  0.0f, 1.0f,	0.0f, 0.0f, 1.0f,
	 float(drawingSpaceWidth)/2.0f,  float(drawingSpaceHeight)/2.0f,  1.0f, 1.0f,	1.0f, 1.0f, 1.0f
};

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
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
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
	glViewport(0, 0, width, height);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	

	// VAO, VBO SETUP
	GLuint VAO1, VBO1;

	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ALPHA BLENDING
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ETC
	Stats stats;
	Sliders sliders(window);
	Color color;
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 1.0f));
	float scale = 1.0f;
	Pen pen(&color);
	Layer layer(drawingSpaceWidth, drawingSpaceHeight, glm::vec2(0.0f, 0.0f));
	

	// FBO
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	unsigned int colorTex;
	glGenTextures(1, &colorTex);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, drawingSpaceWidth, drawingSpaceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

	// Check completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FBO not complete!\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// RECTANGLE VAO, VBO, EBO
	unsigned int rectVAO, rectVBO, rectEBO;
	glGenVertexArrays(1, &rectVAO);
	glBindVertexArray(rectVAO);

	glGenBuffers(1, &rectVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &rectEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	// SHADER
	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.Activate();
	//shaderProgram.Recompile("default.vert", "default.frag");
	Shader framebufferProgram("frame.vert", "frame.frag");
	//framebufferProgram.Recompile("frame.vert", "frame.frag");
	framebufferProgram.Activate();
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	
	camera.printMat();
	// MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		// bind the layer FBO
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, drawingSpaceWidth, drawingSpaceHeight);

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);
		// Use our default drawing shader
		shaderProgram.Activate();

		// UPDATE DIMENSIONS
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		if (camera.width != width and camera.height != height)
		{
			camera.width = width;
			camera.height = height;
			camera.update();
		}
		//std::cout << width << " " << height << std::endl;
		// METRICS
		stats.showStats(window);
		// PEN INPUTS
		pen.Inputs(window, camera, &layer);

		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO1);
		
		// CAMERA INPUTS
		camera.Inputs(window);
		layer.update();
		// Updates and exports the camera matrix to the Vertex Shader
		layer.Matrix(shaderProgram, "layerMatrix");


		//glEnable(GL_BLEND);
		// DRAW
		for (std::vector<Vertex> circle : pen.circles) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			glBufferData(GL_ARRAY_BUFFER, circle.size() * sizeof(Vertex), circle.data(), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_FAN, 0, circle.size());
		}
		//std::cout << layer.lines.size() << std::endl;
		for (std::vector<Vertex> line : layer.lines) {
			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			glBufferData(GL_ARRAY_BUFFER, line.size() * sizeof(Vertex), line.data(), GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, line.size());
		}

		//std::cout << layer.lines.size() << "\n";

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


		// If erasing:
		//glDisable(GL_BLEND);
		/*setShaderColor(bgR, bgG, bgB, 1.0f);
		drawEraseCircle();*/


		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);

		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the framebuffer rectangle
		framebufferProgram.Activate();
		camera.Matrix(framebufferProgram, "cameraMatrix");
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, colorTex);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
		// IMGUI SLIDERS
		sliders.addSliders(&color, &pen, &camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteFramebuffers(1, &FBO);
	shaderProgram.Delete();

	glDeleteVertexArrays(1, &rectVAO);
	glDeleteBuffers(1, &rectVBO);
	glDeleteBuffers(1, &rectEBO);
	framebufferProgram.Delete();

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

