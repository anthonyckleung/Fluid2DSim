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
	static std::map<std::string, Texture> Textures;

	static Shader  LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
	// Retrieves a stored sader
	static Shader  &GetShader(std::string name);
	// Loads (and generates) a texture from file
	static Texture LoadTexture(const GLchar *file, GLboolean alpha, std::string name); // TODO: Can't do this right now
	// Retrieves a stored texture
	static Texture &GetTexture(std::string name);
	// Properly de-allocates all loaded resources
	static void    Clear();


private:
	// Private constructor, that is we do not want any actual resource manager objects. 
	// Its members and functions should be publicly available (static).
	ResourceManager() {}
	static Shader  loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
	static Texture loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif //RESOURCEMANAGER_H