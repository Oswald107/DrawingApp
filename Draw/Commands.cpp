#include "Commands.h"
#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

void save(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
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