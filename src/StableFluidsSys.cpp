#include <assert.h>


#include "StableFluidsSys.h"



StableFluidsSys::StableFluidsSys(int rows, int cols,
	                             const double& diff, const double& visc,
	                             bool use_advect, bool use_project):
	m_d(rows + 2, cols + 2, 0.f), 
	m_u(rows + 2, cols + 1, 0.f),
	m_v(rows + 1, cols + 2, 0.f)
{
	assert(rows == cols);
	m_N            = rows;
	m_Diff         = diff;
	m_Visc         = visc;
	m_UseAdvect    = use_advect;
	m_UseProject   = use_project;

}


StableFluidsSys::~StableFluidsSys()
{
}

void StableFluidsSys::StepSystem(const double & dt)
{
}
