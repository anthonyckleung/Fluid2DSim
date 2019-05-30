#ifndef SIMULATION_H
#define SIMULATION_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

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

	SimState		m_State;

	// Initialize simulation resources
	void Initialize();
	void ProcessInput();
	void Update(GLfloat dt);
	void Render();

private:
	int			m_Width, m_Height;
	GLfloat			m_dt;               // Time step
	//StableFluidsSys fluidSys;
};

#endif // SIMULATION_H