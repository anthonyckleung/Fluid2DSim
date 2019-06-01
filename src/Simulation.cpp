#include <iostream>
#include <memory>
#include "ResourceManager.h"
#include "SceneRenderer.h"
#include "StableFluidsSys.h"
#include "Simulation.h"

SceneRenderer *Renderer;

Simulation::Simulation(int width, int height)
	: m_State(SIM_ACTIVE), m_Width(width), m_Height(height)
{
}

Simulation::~Simulation()
{
	delete Renderer;
}

void Simulation::Initialize()
{
	std::shared_ptr<StableFluidsSys> fluidsys(nullptr);
	fluidsys = std::make_shared<StableFluidsSys>(this->m_Height, this->m_Width);

	// Load shaders
	ResourceManager::LoadShader("src/shaders/particle.vert", "src/shaders/particle.frag", nullptr, "scene");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->m_Width), static_cast<GLfloat>(this->m_Height), 
		                              0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("scene").Use().SetInteger("image", 0);
	ResourceManager::GetShader("scene").SetMatrix4("projection", projection);

	// Load textures (Depends on Physics System)
	//ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadFluidTexture("fluid", fluidsys);

	// Set render-specific controls
	Renderer = new SceneRenderer(ResourceManager::GetShader("scene"));
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
	/*Renderer->DrawScene(ResourceManager::GetTexture("face"), glm::vec2(0, 0), 
		                   glm::vec2(400, 400), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));*/
	Renderer->DrawScene(ResourceManager::GetFluidTexture("fluid"), glm::vec2(0, 0), 
		                   glm::vec2(400, 400), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}
