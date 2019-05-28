#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include "Shader.h"
#include "Texture.h"

class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, Shader>  Shaders;

	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	// Retrieves a stored sader
	static Shader   GetShader(std::string name);


private:
	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() {}
	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
};

#endif //RESOURCEMANAGER_H