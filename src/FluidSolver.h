#ifndef FLUID_SOLVER_H
#define FLUID_SOLVER_H

#include "MathUtilities.h"



class FluidSolver
{
public:
	~FluidSolver() = default;

	virtual void solve() = 0;
};

#endif //FLUID_SOLVER_H