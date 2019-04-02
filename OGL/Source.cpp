#include <iostream>
#include <string>
#include <fstream>

#include "ShaderProcesser.h"

const int WND_WINDTH = 800;
const int WND_HEIGHT = 600;

GLuint VAO = 0;
GLuint VBO = 0;
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
}

void init_triangle()
{
	float vertices[] = {
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f		 
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(0 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
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

	GLFWwindow* wnd = glfwCreateWindow(WND_WINDTH, WND_HEIGHT, "OpenGL Wnd", nullptr, nullptr);
	glfwMakeContextCurrent(wnd);

	glfwSetWindowPos(wnd, 100, 100);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, WND_WINDTH, WND_HEIGHT);
	glfwSetWindowSizeCallback(wnd, framebuffer_size_callback);
	glfwSwapInterval(1);

	program = new ShaderProcesser("VertexShader.glsl", "FragmentShader.glsl");
	init_triangle();

	program->use();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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