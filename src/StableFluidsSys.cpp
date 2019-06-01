#include <iostream>
#include <assert.h>


#include "StableFluidsSys.h"



StableFluidsSys::StableFluidsSys(int rows, int cols,
	                             const float& diff, const float& visc,
	                             bool use_advect, bool use_project):
	m_d(rows , cols , 0.f), 
	m_u(rows , cols , 0.f),
	m_v(rows , cols , 0.f)
{
	assert(rows == cols);
	m_N            = rows;
	m_Diff         = diff;
	m_Visc         = visc;
	m_UseAdvect    = use_advect;
	m_UseProject   = use_project;

	InitializeFluidSource(m_N * 0.5, m_N * 0.5);
}


StableFluidsSys::~StableFluidsSys()
{
}

void StableFluidsSys::StepSystem(const float & dt)
{
}

void StableFluidsSys::InitializeFluidSource(int x, int y)
{
	double upVelocity = 3.5;
	double outVelocity = 0.f;

	int left = x - m_N*0.1;
	int right = x + m_N * 0.1;
	int top = y - m_N * 0.1;
	int bottom = y + m_N * 0.1;

	//std::cout << m_N << ", " << m_N * 0.5 << ", " << x << ", " <<  y << std::endl;
	for (int i = left; i <= right - 1; ++i)
	{
		//if (i < 0 || i > m_N) { continue; }
		for (int j = top; j <= bottom - 1; ++j)
		{
			//if (j < 0 || j > m_N) { continue; }
			m_d(i, j) = 100.f;
		}
	}

	//ArrayXs d = m_d.grid();
	//std::cout << d << std::endl;
	//m_d.displayGrid();
	
}

ArrayXs StableFluidsSys::GetDensities() 
{
	// TODO: insert return statement here
	ArrayXs d = m_d.grid();
	return d;
}
