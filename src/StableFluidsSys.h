#ifndef STABLE_FLUIDS_SYS_H
#define STABLE_FLUIDS_SYS_H

//#include "MathUtilities.h"

#include "Grid2D.hpp"
class StableFluidsSys
{
public:
	StableFluidsSys(int rows, int cols,
		            const float& diff = 0.0001, const float& visc = 0.00000001,
		            bool use_advect = true, bool use_project = true);

	~StableFluidsSys();

	// Solve the system forward in time by dt
	void StepSystem(const float & dt);

	// Temp. method to initialize fluid scene
	void InitializeFluidSource(int x, int y);

	ArrayXs GetDensities() ;

	
	int             m_N;
	
protected:
	// Convenient grid access
	//static   double & d(ArrayXs * d, int i, int j) { return (*d)(i, j); } // Density grid

	//static   double & u(ArrayXs * u, int i, int j) { return (*u)(i, j); } // u-velocity grid
	//static   double & v(ArrayXs * v, int i, int j) { return (*v)(i, j); } // v-velocity grid

	bool            m_UseAdvect;
	bool            m_UseProject;
	double          m_Diff;
	double          m_Visc;
	
	Grid2D<float>  m_d;  // Density of particles
	Grid2D<float>  m_u;  // u (Hori.) velocity
	Grid2D<float>  m_v;  // v (Vert.) velocity
	

	
};

#endif // STABLE_FLUIDS_SYS_H