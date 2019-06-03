#ifndef SIMULATION_H
#define SIMULATION_H
#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class StableFluidsSys;

enum SimState {
	SIM_ACTIVE,
	SIM_MENU,
	SIM_PAUSED
};

class Simulation
{
public:
	Simulation(int width, int height);
	~Simulation();

	SimState   m_State;

	float RenderTime = -1;
	
	void Initialize();              // Initialize simulation resources
	void ProcessInput();
	void Update(GLfloat dt);
	void Render();

private:
	int        m_Width, m_Height;
	GLfloat	   m_dt;               // Time step
	std::shared_ptr<StableFluidsSys> fluidsys;
};

#endif // SIMULATION_H