#ifndef CONFIGURATION
#define CONFIGURATION

#include <string>

class Config
{
public:
	static const int WND_WINDTH = 800;
	static const int WND_HEIGHT = 600;
	static const std::string VERT_SHADER_PATH;
	static const std::string FRAG_SHADER_PATH;
	static const std::string IMG_CONTAINER_PATH;
};

const std::string Config::VERT_SHADER_PATH = "VertexShader.glsl";
const std::string Config::FRAG_SHADER_PATH = "FragmentShader.glsl";
const std::string Config::IMG_CONTAINER_PATH = "imgs/container.jpg";

#endif // !CONFIGURATION
