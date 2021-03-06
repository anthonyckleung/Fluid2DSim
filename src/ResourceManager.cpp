#include <iostream>
#include <sstream>
#include <fstream>

#include "SOIL.h"

#include "ResourceManager.h"

std::map<std::string, Texture>  ResourceManager::Textures;
std::map<std::string, Shader>   ResourceManager::Shaders;

Shader &ResourceManager::LoadShader(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader &ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

//Texture ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
//{
//	// TODO: Need to get texture from physics system. 
//	//       This is implemented later
//	Textures[name] = loadTextureFromFile(file, alpha);
//	return Textures[name];
//}

Texture &ResourceManager::LoadFluidTexture(std::string name, const std::shared_ptr<StableFluidsSys> &fluidSys)
{
	Textures.emplace(std::make_pair(name, fluidSys));
	Textures.at(name).GenerateFluid();
	return Textures.at(name);
}


//Texture &ResourceManager::GetTexture(std::string name)
//{
//    return Textures[name];
//}

Texture & ResourceManager::GetFluidTexture(std::string name)
{
	return Textures.at(name);
}

void ResourceManager::Clear()
{
	for (auto iter : Shaders) { glDeleteProgram(iter.second.m_ID); }
	//for (auto iter : Textures) { glDeleteProgram(iter.second.m_ID); }
	for (auto iter : Textures) { glDeleteTextures(1, &iter.second.m_ID); }
}

Shader ResourceManager::loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// Close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

//Texture ResourceManager::loadTextureFromFile(const GLchar * file, GLboolean alpha)
//{
//	// Create Texture object
//	Texture texture;
//	if (alpha)
//	{
//		texture.m_Internal_Format = GL_RGBA;
//		texture.m_Image_Format = GL_RGBA;
//	}
//	// Load image
//	int width, height;
//	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.m_Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
//	//std::cout << *(image) << std::endl;
//	// Now generate texture
//	texture.Generate(width, height, image);
//	// And finally free image data
//	SOIL_free_image_data(image);
//	return texture;
//}
