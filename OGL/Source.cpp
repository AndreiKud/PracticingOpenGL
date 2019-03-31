#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

const int WND_WINDTH = 800;
const int WND_HEIGHT = 600;

GLuint shader_program = 0;
GLuint VAOs[3]{ 0, 0, 0 };
GLuint VBO = 0;
GLuint fragment_shader = 0;
GLuint vertex_shader = 0;

GLuint color_offset = 0;

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

	if (glfwGetKey(wnd, GLFW_KEY_UP) == GLFW_PRESS)
	{
		color_offset += 1;
		if (color_offset == 3) {
			color_offset = 0;
		}

		glBindVertexArray(VAOs[color_offset]);
	}

	if (glfwGetKey(wnd, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		color_offset -= 1;
		if (color_offset == -1) {
			color_offset = 2;
		}

		glBindVertexArray(VAOs[color_offset]);
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

	glGenVertexArrays(3, VAOs);

	glBindVertexArray(VAOs[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(0 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));

	glBindVertexArray(VAOs[1]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(0 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(GL_FLOAT)));

	glBindVertexArray(VAOs[2]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(0 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(3, VAOs);
}

void render(GLFWwindow* wnd)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

char* readFile(const char* filePath) {
	FILE* shaderFile{};
	errno_t err = fopen_s(&shaderFile, filePath, "rb");
	if (err || shaderFile == NULL) {
		return nullptr;
	}

	fseek(shaderFile, 0L, SEEK_END);
	unsigned long shaderFileSize = ftell(shaderFile);
	rewind(shaderFile);
	char* buffer = (char*)malloc(shaderFileSize + 1);
	if (buffer == nullptr) {
		fclose(shaderFile);
		return nullptr;
	}

	fread(buffer, shaderFileSize, 1, shaderFile);
	buffer[shaderFileSize] = '\0';
	fclose(shaderFile);

	return buffer;
}

void init_shaders()
{
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertex_shader_source = readFile("VertexShader.glsl");
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragment_shader_source = readFile("FragmentShader.glsl");
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	delete[] vertex_shader_source;
	delete[] fragment_shader_source;
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

	init_shaders();
	init_triangle();

	glUseProgram(shader_program);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAOs[0]);
	while (!glfwWindowShouldClose(wnd))
	{
		process_input(wnd);

		render(wnd);

		glfwSwapBuffers(wnd);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}