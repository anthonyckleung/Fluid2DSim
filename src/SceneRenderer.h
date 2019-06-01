#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "StableFluidsSys.h"


// Hosts a shader object, a single vertex array object
// and a rendering and initialization function
class SceneRenderer
{
public:
	SceneRenderer(Shader &shader);
	//SceneRenderer(Shader &shader, GLuint width, GLuint height);
	~SceneRenderer();

	void    DrawScene(Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), 
		               GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

	void    DrawFluidScene(const Texture &fluidTexture);

private:
	Shader  m_shader;
	GLuint  m_quadVAO;

	void    initRenderData();

	GLuint m_Width;
	GLuint m_Height;
};

#endif// PARTICLERENDERER_H

