#include "ShaderProcesser.h"

ShaderProcesser::ShaderProcesser(const std::string& vertPath, const std::string& fragPath)
{
	std::string vertex_shader_source = read_file("VertexShader.glsl");
	std::string fragment_shader_source = read_file("FragmentShader.glsl");
	init(vertex_shader_source, fragment_shader_source);
}

ShaderProcesser::~ShaderProcesser()
{
}

std::string ShaderProcesser::read_file(const std::string& path)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::fstream::badbit);
	try
	{
		shaderFile.open(path.c_str());
		std::stringstream shaderStringStream;
		shaderStringStream << shaderFile.rdbuf();
		shaderCode = shaderStringStream.str();
	}
	catch (std::exception ex)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return shaderCode;
}

void ShaderProcesser::init(const std::string& vertSrcCode, const std::string& fragSrcCode)
{
	int success;
	char infoLog[512];

	// vertex shader
	GLint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* c_src_vert_code = vertSrcCode.c_str();
	glShaderSource(vertex_shader, 1, &c_src_vert_code, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	GLint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* c_src_frag_code = fragSrcCode.c_str();
	glShaderSource(fragment_shader, 1, &c_src_frag_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	m_program = glCreateProgram();
	glAttachShader(m_program, vertex_shader);
	glAttachShader(m_program, fragment_shader);
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void ShaderProcesser::use() const
{
	glUseProgram(m_program);
}

GLint ShaderProcesser::program() const
{
	return m_program;
}