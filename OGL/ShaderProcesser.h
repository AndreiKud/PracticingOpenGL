#ifndef SHADER_PROCESSER
#define SHADER_PROCESSER

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class ShaderProcesser
{
public:
	ShaderProcesser(const std::string& vertPath, const std::string& fragPath);
	~ShaderProcesser();

	void use() const;
	GLint program() const;

private:
	void init(const std::string& vertSrcCode, const std::string& fragSrcCode);
	std::string read_file(const std::string& path);

private:
	GLuint m_program = 0;
};


#endif // !SHADER_PROCESSER