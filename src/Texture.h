#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <array>
#include "StableFluidsSys.h"
#include <GL/glew.h>


class Texture
{
public:
	//Texture();
	Texture(const std::shared_ptr<StableFluidsSys> &fluidsys);

	// Generates texture from image data
	void Generate(GLuint width, GLuint height, unsigned char* data);
	void GenerateFluid();

	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;

	// Update Texture
	void Update();

	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	//std::array<GLuint, textures> m_IDs;
	GLuint   m_ID;
	// Texture image dimensions
	GLuint   m_Width, m_Height; // Width and height of loaded image in pixels
	// Texture Format
	GLuint   m_Internal_Format; // Format of texture object
	GLuint   m_Image_Format ;    // Format of loaded image
	// Texture configuration
	GLuint   m_Wrap_S;          // Wrapping mode on S axis
	GLuint   m_Wrap_T;          // Wrapping mode on T axis
	GLuint   m_Filter_Min;      // Filtering mode if texture pixels < screen pixels
	GLuint   m_Filter_Max;      // Filtering mode if texture pixels > screen pixels
private:
	std::shared_ptr<StableFluidsSys> fluidsys;
};

#endif //TEXTURE_H