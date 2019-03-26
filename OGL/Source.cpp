#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void error_handler(int err_code, const char* msg)
{
	std::cout << "Err no. " << err_code << ": " << msg << std::endl;
	glfwTerminate();
	exit(1);
}

int main()
{
	glfwSetErrorCallback(error_handler);
	GLenum res = glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* wnd = glfwCreateWindow(800, 600, "OpenGL Wnd", nullptr, nullptr);
	glfwMakeContextCurrent(wnd);

	glfwSetWindowPos(wnd, 100, 100);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(wnd))
	{
		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	return 0;
}