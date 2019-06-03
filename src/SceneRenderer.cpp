#include "Texture.h"
#include "Shader.h"

#include "SceneRenderer.h"



SceneRenderer::SceneRenderer(Shader &shader)
{
	this->m_shader = shader;
	this->initRenderData();
}

//SceneRenderer::SceneRenderer(Shader &shader, GLuint width, GLuint height)
//{
//	this->m_shader = shader;
//	this->initRenderData();
//	this->m_Height = height;
//	this->m_Width = width;
//}


SceneRenderer::~SceneRenderer()
{
	glDeleteVertexArrays(1, &this->m_quadVAO);
}

void SceneRenderer::DrawScene(Texture & texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{   
	// Prepare transformations
	this->m_shader.Use();

	glm::mat4 model;
	// First translate (transformations are: scale happens first,
	// then rotation and then finall translation happens; reversed order)
	model = glm::translate(model, glm::vec3(position, 0.0f));  
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale

	this->m_shader.SetMatrix4("model", model);
	
	//------------------------------------------------
	//* Replace this block to render fluid texture
	// Render textured quad
	this->m_shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();
	//------------------------------------------------

	glBindVertexArray(this->m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


void SceneRenderer::initRenderData()
{
	// Configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	//GLfloat vertices[] = {
	//	// Pos                         // Tex
	//	-0.5f * m_Width, 0.5f * m_Height,  0.0f, 0.0f,
	//	0.5f * m_Width, -0.5f * m_Height,  1.0f, 1.0f,
	//	-0.5f *m_Width, -0.5f * m_Height, 0.0f, 1.0f,

	//	-0.5f *m_Width, 0.5f * m_Height,  0.0f, 0.0f,
	//	0.5f * m_Width, 0.5f * m_Height,   1.0f, 0.0f,
	//	0.5f *m_Width, -0.5f * m_Height,  1.0f, 1.0f
	//};

	glGenVertexArrays(1, &this->m_quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->m_quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
