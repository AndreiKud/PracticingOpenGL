#include <iostream>
#include <string>
#include <fstream>

#include <soil/src/SOIL.h>

#include "ShaderProcesser.h"
#include "Configuration.h"

GLuint VAO = 0;
GLuint VBO = 0;
GLuint texture = 0;
ShaderProcesser* program = nullptr;

void error_handler(int err_code, const char* msg)
{
	std::cout << "Err no. " << err_code << ": " << msg << std::endl;
	glfwTerminate();
	exit(1);
}

void framebuffer_size_callback(GLFWwindow* wnd, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* wnd)
{
	if (glfwGetKey(wnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(wnd, GLFW_TRUE);
	}

	// comment3
}

void init_triangle()
{
	GLfloat vertices[] = {
		// Positions	  // Colors		    // Texture Coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // Top Left
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

void init_textures()
{
	int width, height;
	unsigned char* image = SOIL_load_image(Config::IMG_CONTAINER_PATH.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// here 2nd parameter is for generating mipmaps so i will try it with zooming
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void render(GLFWwindow* wnd)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void set_uni()
{
	GLfloat time = static_cast<GLfloat>(glfwGetTime());
	GLfloat greenValue = sin(time) / 2 + 0.5f;
	GLint location = glGetUniformLocation(program->program(), "uniColor");
	glUniform4f(location, 0.0f, greenValue, 0.5f, 1.0f);
	program->use();
}

int main()
{
	glfwSetErrorCallback(error_handler);
	GLenum res = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* wnd = glfwCreateWindow(Config::WND_WINDTH, Config::WND_HEIGHT, "OpenGL Wnd", nullptr, nullptr);
	glfwMakeContextCurrent(wnd);

	glfwSetWindowPos(wnd, 100, 100);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, Config::WND_WINDTH, Config::WND_HEIGHT);
	glfwSetWindowSizeCallback(wnd, framebuffer_size_callback);
	glfwSwapInterval(1);

	program = new ShaderProcesser(Config::VERT_SHADER_PATH, Config::FRAG_SHADER_PATH);
	
	init_triangle();
	init_textures();

	program->use();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	while (!glfwWindowShouldClose(wnd))
	{
		process_input(wnd);

		set_uni();
		render(wnd);

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	glfwTerminate();
	delete program;

	return 0;
}