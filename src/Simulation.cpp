
#include "ResourceManager.h"
#include "SceneRenderer.h"
#include "Simulation.h"

SceneRenderer *Renderer;

Simulation::Simulation(GLuint width, GLuint height)
				: m_State(SIM_ACTIVE), m_Width(width), m_Height(height)
{
}

Simulation::~Simulation()
{
	delete Renderer;
}

void Simulation::Initialize()
{
	// Load shaders
	ResourceManager::LoadShader("src/shaders/particle.vert", "src/shaders/particle.frag", nullptr, "particle");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->m_Width), static_cast<GLfloat>(this->m_Height), 
		                              0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("particle").Use().SetInteger("image", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	// Load textures (Depends on Physics System)
	ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "face");
	// Set render-specific controls
	Renderer = new SceneRenderer(ResourceManager::GetShader("particle"));
}

void Simulation::ProcessInput()
{
}

void Simulation::Update(GLfloat dt)
{
	// Updates the particle texture
}

void Simulation::Render()
{
	Renderer->DrawParticle(ResourceManager::GetTexture("face"), glm::vec2(200, 200), 
		                   glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
