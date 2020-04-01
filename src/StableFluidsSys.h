#ifndef STABLE_FLUIDS_SYS_H
#define STABLE_FLUIDS_SYS_H

#include "MathUtilities.h"

enum SolverType {
	CPU,
	OMP,
	CUDA
};

class StableFluidsSys
{
public:
	StableFluidsSys(int & rows, int & cols,
		            const float& diff = 0.0000, const float& visc = 0.0000,
		            bool use_advect = true, bool use_project = true);

	~StableFluidsSys();

	// Temp. method to initialize fluid scene
	void InitializeFluidSource(int &x, int &y);
	
	void StepSystem( const float & dt ); // Solve the system forward in time by dt
	

	const int GetPhysicalLength();
	const int GetGridLength();

	const ArrayXs GetDensities() ;

	const ArrayXs GetHorizontalVelocities();
	const ArrayXs GetVerticalVelocities();

	
protected:

	void SWAP(ArrayXs *& x1, ArrayXs *& x2);

	void SetBndCondns(int N, int b, ArrayXs * x);
	void SetBndCondnsU(int N, ArrayXs * x);
	void SetBndCondnsV(int N, ArrayXs * x);

	// Interpolator
	float interpolateD(ArrayXs * d, float i, float j);
	float interpolateU(ArrayXs * u, float i, float j);
	float interpolateV(ArrayXs * v, float i, float j);

	void VelocityStep(int N, ArrayXs * u, ArrayXs * v, ArrayXs * u0, ArrayXs * v0, float dt);
	void DensityStep(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float diff, float dt);
	
	void AddSource(int N, ArrayXs * x, ArrayXs * x0, float dt);

	void DiffuseD(int N, ArrayXs * x, ArrayXs * x0, float diff, float dt);
	void DiffuseU(int N, ArrayXs * x, ArrayXs * x0, float diff, float dt);
	void DiffuseV(int N, ArrayXs * x, ArrayXs * x0, float diff, float dt);

	void AdvectD(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float dt);
	void AdvectU(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float dt);
	void AdvectV(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float dt);

	void Project(int N, ArrayXs * u, ArrayXs * v, ArrayXs * u0, ArrayXs * v0);

	

	// Vector Fields
	ArrayXs   m_d;           // Particle density field
	ArrayXs   m_u;           // u (Hori.) velocity field
	ArrayXs   m_v;           // v (Vert.) velocity field

	ArrayXs   m_dPrevious;
	ArrayXs   m_uPrevious;
	ArrayXs   m_vPrevious;
	
	ArrayXs   m_dSource;
	ArrayXs   m_uFlowSource;
	ArrayXs   m_vFlowSource;

	/*ArrayXs   m_uAfterDiffusion;
	ArrayXs   m_vAfterDiffusion;
	ArrayXs   m_uAfterAdvect;
	ArrayXs   m_vAfterAdvect;*/

	// Parameters
	int       m_N;           // Length of grid
	float     m_Diff;        // Diffusion coefficient
	float     m_Visc;        // Viscosity coefficient
	bool      m_UseAdvect;
	bool      m_UseProject;
	
	
};

#endif // STABLE_FLUIDS_SYS_H