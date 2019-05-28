#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



// Hosts a shader object, a single vertex array object
// and a rendering and initialization function
class SceneRenderer
{
public:
	SceneRenderer(Shader &shader);
	~SceneRenderer();

	void    DrawParticle(Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), 
		               GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader  m_shader;
	GLuint  m_quadVAO;

	void    initRenderData();
};

#endif// PARTICLERENDERER_H

