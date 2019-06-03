#include <iostream>
#include <cstdint>
#include <vector>
#include "MathUtilities.h"
#include "Texture.h"


//Texture::Texture()
//	: m_Width(0), m_Height(0), m_Internal_Format(GL_RGB), m_Image_Format(GL_RGB), m_Wrap_S(GL_REPEAT), 
//	  m_Wrap_T(GL_REPEAT), m_Filter_Min(GL_LINEAR), m_Filter_Max(GL_LINEAR)
//{
//	glGenTextures(1, &this->m_ID);
//}

Texture::Texture(const std::shared_ptr<StableFluidsSys> &fluidsys)
	: fluidsys(fluidsys)
{
	glGenTextures(1, &this->m_ID);
}


void Texture::Generate(GLuint width, GLuint height, unsigned char * data)
{
	// This generates a single texture
	this->m_Width = width;
	this->m_Height = height;

	// Create Texture

	int n = m_Width;
	std::vector<float> image(n*n * 3);

	/*for (int i = 0; i < n*n*3; i++)
	{
			image[i] = 255;
	}*/
	glBindTexture(GL_TEXTURE_2D, this->m_ID);
	
	/*glTexImage2D(GL_TEXTURE_2D, 0, this->m_Internal_Format, width, height, 0, 
		         this->m_Image_Format, GL_UNSIGNED_INT, &image[0]);*/
	// Set Texture wrap and filter modes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, n, n, 0, GL_RGB, GL_FLOAT, &image[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::GenerateFluid()
{
	const auto &d = fluidsys->GetDensities();
	GLuint n = fluidsys->GetGridLength();
	this->m_Width = n;
	this->m_Height = n;

	//std::vector<float> image(n*n*3);

	//for (int i = 0; i < n*n*3 ; i++)
	//{ 
	//	image[i] = 155;
	//}
	//std::cout << m_ID << std::endl;
	//std::cout << d(n*0.5, n*0.5) << std::endl;
	
	glBindTexture(GL_TEXTURE_2D, this->m_ID);

	// Set Texture wrap and filter modes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, n, n, 0, GL_RED, GL_FLOAT, d.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->m_ID);
}

void Texture::Update()
{
	const auto &d = fluidsys->GetDensities();
	GLuint n = fluidsys->GetGridLength();

	glBindTexture(GL_TEXTURE_2D, this->m_ID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, n, n, GL_RED, GL_FLOAT, d.data());
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, n, n, 0, GL_RED, GL_FLOAT, d.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}
