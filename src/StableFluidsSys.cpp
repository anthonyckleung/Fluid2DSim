#include <iostream>
#include <assert.h>
#include <omp.h>
#include "StableFluidsSys.h"



StableFluidsSys::StableFluidsSys(int & rows, int & cols,
	                             const float& diff, const float& visc,
	                             bool use_advect, bool use_project):
	m_d(rows, cols ), 
	m_u(rows, cols - 1 ),
	m_v(rows-1 , cols),
	m_dSource(rows, cols),
	m_uFlowSource(rows, cols - 1),
	m_vFlowSource(rows - 1, cols)

{
	assert(rows == cols);
	m_N            = rows;
	m_Diff         = diff;
	m_Visc         = visc;
	m_UseAdvect    = use_advect;
	m_UseProject   = use_project;

	
	int n = rows * 0.5;
	int row = 0;
	int col = rows * 0.5;
	InitializeFluidSource(col, row); // Temporary

}


StableFluidsSys::~StableFluidsSys()
{
}


void StableFluidsSys::InitializeFluidSource(int &x, int &y)
{
	//double upVelocity = 3.5;
	//double outVelocity = 0.f;
	int N = GetGridLength();

	int left   = x - N * 0.02;
	int right  = x + N * 0.02;
	int top    = y - N * 0.02;
	int bottom = y + N * 0.25;

	for (int i = y; i <= bottom ; ++i)
	{
		//if (i < 0 || i > m_N) { continue; }
		for (int j = left; j <= right ;  ++j)
		{
			//if (j < 0 || j > m_N) { continue; }
			this->m_d(i, j) = 1.1f;
		}
	}

	left = x - N * 0.08;
	right = x + N * 0.08;
	top = y - N * 0.4;
	bottom = y + N * 0.8;

	m_vFlowSource.fill(0.0002);

	for (int i = y; i <= bottom; ++i)
	{
		//if (i < 0 || i > m_N) { continue; }
		for (int j = left; j <= right; ++j)
		{
			//if (j < 0 || j > m_N) { continue; }
			//if (i < 0.5*N) { this->m_vFlowSource(i, j) = -0.01f; }
			//else if (i > 0.5*N) { this->m_vFlowSource(i, j) = -0.01f; }
			this->m_vFlowSource(i, j) = i*0.0003f;
		}
	}


	//m_uFlowSource.fill(0.0f);
	//m_vFlowSource.fill(0.01f);
	m_dSource.fill(0.f);
	//m_dSource = m_d;
	m_dPrevious.setZero();
	m_u= m_uFlowSource;
	m_v= m_vFlowSource;
	m_uPrevious.setZero();
	m_vPrevious.setZero();
}

void StableFluidsSys::SWAP(ArrayXs *& x1, ArrayXs *& x2)
{
	auto *temp = x1;
	x1 = x2;
	x2 = temp;
}

void StableFluidsSys::SetBndCondns(int N, int b, ArrayXs * x)
{
	// Continuity boundary conditions
	for (int i = 1; i <= N; i++)
	{
		(*x)(0  , i  ) = b == 1 ? -(*x)(1, i) : (*x)(1, i);
		(*x)(N, i  ) = b == 1 ? -(*x)(N, i) : (*x)(N, i);
		(*x)(i  , 0  ) = b == 2 ? -(*x)(i, 1) : (*x)(i, 1);
		(*x)(0  , N) = b == 2 ? -(*x)(1, N) : (*x)(1, N);
	}
	(*x)(0  , 0  ) = 0.5 * ((*x)(1, 0  ) + (*x)(0  , 1));
	(*x)(0  , N+1) = 0.5 * ((*x)(1, N+1) + (*x)(0  , 1));
	(*x)(N+1, 0  ) = 0.5 * ((*x)(N, 0  ) + (*x)(N+1, 1));
	(*x)(N+1, N+1) = 0.5 * ((*x)(N, N+1) + (*x)(N+1, 1));
}

void StableFluidsSys::SetBndCondnsU(int N, ArrayXs * u)
{
	// Boundary Conditions
	for (int i = 1; i <= N; i++)
	{
		(*u)(i, 0) = -(*u)(i, 1);       // Left-most column
		(*u)(i, N) = -(*u)(i, N - 1);  // Right-most column
	}
	// Corners
	(*u)(0, 0) = 0.5 * ((*u)(1, 0) + (*u)(0, 1));
	(*u)(0, N - 1) = 0.5 * ((*u)(1, N - 1) + (*u)(0, N - 2));
	(*u)(N, 0) = 0.5 * ((*u)(N - 1, 0) + (*u)(N, 1));
	(*u)(N, N - 1) = 0.5 * ((*u)(N - 1, N - 1) + (*u)(N, N - 2));
}

void StableFluidsSys::SetBndCondnsV(int N, ArrayXs * v)
{
	for (int j = 1; j <= N; j++)
	{
		(*v)(0, j) = -(*v)(1, j);      // Top
		(*v)(N, j) = -(*v)(N - 1, j);  // Bottom 
	}

	(*v)(0, 0) = 0.5 * ((*v)(1, 0) + (*v)(0, 1));
	(*v)(0, N) = 0.5 * ((*v)(1, N) + (*v)(0, N - 1));
	(*v)(N - 1, 0) = 0.5 * ((*v)(N - 2, 0) + (*v)(N - 1, 1));
	(*v)(N - 1, N) = 0.5 * ((*v)(N - 2, N) + (*v)(N - 1, N));
}


void StableFluidsSys::StepSystem(const float & dt)
{	
	int N = GetPhysicalLength();
	
	VelocityStep(N, &m_u, &m_v, &m_uPrevious, &m_vPrevious, dt);
	DensityStep(N, &m_d, &m_dPrevious, &m_u, &m_v, m_Diff, dt);
}


const int StableFluidsSys::GetPhysicalLength()
{
	return m_N - 2;
}

const int StableFluidsSys::GetGridLength()
{
	return m_N;
}

ArrayXs const StableFluidsSys::GetDensities()
{
	auto &d = m_d;
	return d;
}

const ArrayXs StableFluidsSys::GetHorizontalVelocities()
{
	return m_u;
}

const ArrayXs StableFluidsSys::GetVerticalVelocities()
{
	return m_v;
}

float StableFluidsSys::interpolateD(ArrayXs * d, float i, float j)
{
	int N = GetPhysicalLength();
	float i1 = CLAMP((int)i, 0, N);
	float i2 = i1 + 1;
	float j1 = CLAMP((int)j, 0, N);
	float j2 = j1 + 1;
	float s = CLAMP(i - i1, 0, 1);
	float t = CLAMP(j - j1, 0, 1);
	float d1 = (1 - s)*(*d)(i1, j1) + s * (*d)(i2, j1);
	float d2 = (1 - s)*(*d)(i1, j2) + s * (*d)(i2, j2);
	float dfinal = (1 - t)*d1 + t * d2;
	return dfinal;
}

float StableFluidsSys::interpolateU(ArrayXs * u, float i, float j)
{
	// Note the i index should be CLAMP-ed to [0, m_N], while j index should be 
	// CLAMP-ed to [0, m_N-1], since we have to use (i + 1) and (j + 1)
	int N = GetPhysicalLength();
	float i1 = CLAMP((int)i, 0, N);
	float i2 = i1 + 1;
	float j1 = CLAMP((int)(j - 0.5), 0, N - 1);
	float j2 = j1 + 1;
	float s = CLAMP(i - i1, 0, 1);
	float t = CLAMP(j - j1 - 0.5, 0, 1);
	float u1 = (1 - s)*(*u)(i1, j1) +s * (*u)(i2, j1);
	float u2 = (1 - s)*(*u)(i1, j2) +s * (*u)(i2, j2);
	
	float ufinal = (1 - t)*u1 + t * u2;

	return ufinal;
}

float StableFluidsSys::interpolateV(ArrayXs * v, float i, float j)
{
	int N = GetPhysicalLength();
	float i1 = CLAMP((int)(i - 0.5), 0, N - 1);
	float i2 = i1 + 1;
	float j1 = CLAMP((int)j, 0, N);
	float j2 = j1 + 1;
	float s = CLAMP(i - i1 - 0.5, 0, 1);
	float t = CLAMP(j - j1, 0, 1);
	float v1 = (1 - s)*(*v)(i1, j1) + s * (*v)(i2, j1);
	float v2 = (1 - s)*(*v)(i1, j2) + s * (*v)(i2, j2);
	float vfinal = (1 - t)*v1 + t * v2;

	return vfinal;
}

void StableFluidsSys::VelocityStep(int N, ArrayXs * u, ArrayXs * v, ArrayXs * u0, ArrayXs * v0, float dt)
{
	AddSource(N, u, &m_uFlowSource, dt);
	AddSource(N, v, &m_vFlowSource, dt);

	SWAP(u0, u);   DiffuseU(N, u, u0, m_Visc, dt);
	SWAP(v0, v);   DiffuseV(N, v, v0, m_Visc, dt);
	Project(N, u, v, u0, v0);

	SWAP(u0, u);   AdvectU(N, u, u0, u0, v0, dt);
	SWAP(v0, v);   AdvectV(N, v, v0, u0, v0, dt);
	Project(N, u, v, u0, v0);
}

void StableFluidsSys::DensityStep(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float diff, float dt)
{
	AddSource(N, x, &m_dSource, dt);
	SWAP(x0, x);   DiffuseD(N, x, x0, diff, dt);
	SWAP(x0, x);   AdvectD(N, x, x0, u, v, dt);
}

void StableFluidsSys::AddSource(int N, ArrayXs * x, ArrayXs * x0, float dt)
{
	//int size = (N + 2)*(N + 2);
	*x = *x + (*x0) * (dt);
}

void StableFluidsSys::DiffuseD(int N, ArrayXs * x, ArrayXs * x0, float diff, float dt)
{
	scalar a = diff * dt * N * N;
	*x = *x0;

	for (int k = 0; k < 4; k++)
	{
	#pragma omp parallel for
		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= N; j++) // IMPORTANT: DO NOT MODIFY THE LOOP ORDER
			{
				//Do diffuse for ([1, N], [1, N])
				(*x)(i, j) = ((*x0)(i, j) + a * ((*x)(i + 1, j) + (*x)(i - 1, j) + (*x)(i, j + 1) + (*x)(i, j - 1))) / (1.0 + 4.0*a);
			}
		}
	}
}

void StableFluidsSys::DiffuseU(int N, ArrayXs* x, ArrayXs* x0, float diff, float dt)
{
	assert((*x0 == *x0).all());

	float a = diff * dt * N * N;
	*x = *x0;

	for (int k = 0; k < 4; k++)
	{
		#pragma omp parallel for
		for (int i = 1; i <= N; i++)
		{
			for (int j = 0; j <= N; j++) // IMPORTANT: LOOP ORDER
			{
				// Do diffuse for ([1, N], [0, N-1]), 
				// note the case when (j == 0) or (j == N) need special treatment
				if (j == 0) {// Left side
					(*x)(i, j) = ((*x0)(i, j) + a * ((*x)(i + 1, j) + (*x)(i - 1, j) + 
						                             (*x)(i, j + 1))) / (1.0 + 3.0*a);
				}
				else if (j == N) { // Right side
					(*x)(i, j) = ((*x0)(i, j) + a * ((*x)(i + 1, j) + (*x)(i - 1, j) + 
						                             (*x)(i, j - 1))) / (1.0 + 3.0*a);
				}
				else {
					(*x)(i, j) = ((*x0)(i, j) + a * ((*x)(i + 1, j) + (*x)(i - 1, j) + 
						                            (*x)(i, j + 1) + (*x)(i, j - 1))) / (1.0 + 4.0*a);
				}
			}
		}
	}
}

void StableFluidsSys::DiffuseV(int N, ArrayXs * x, ArrayXs * x0, float diff, float dt)
{
	//assert((*x0 == *x0).all());

	scalar a = diff * dt * N * N;
	*x = *x0;

	for (int k = 0; k < 4; ++k)
	{
		#pragma omp parallel for
		for (int i = 0; i <= N; ++i)
		{
			for (int j = 1; j <= N; ++j) // IMPORTANT: LOOP ORDER
			{
				if (i == 0) {
					(*x)(i, j) = ((*x0)(i, j) + a * ((*x)(i + 1, j) + (*x)(i, j + 1) + 
						                             (*x)(i, j - 1))) / (1.0 + 3.0*a);
				}
				else if (i == N) {
					(*x)(i, j) = ((*x0)(i, j) + a * ((*x)(i - 1, j) + (*x)(i, j + 1) + 
						                             (*x)(i, j - 1))) / (1.0 + 3.0*a);
				}
				else {
					(*x)(i, j) = ((*x0)(i, j) + a * ((*x)(i + 1, j) + (*x)(i - 1, j) + 
						                             (*x)(i, j + 1) + (*x)(i, j - 1))) / (1.0 + 4.0*a);
				}
			}
		}
	}
}

void StableFluidsSys::AdvectD(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float dt)
{
	float dt0 = dt * N;
	float gv, gu;

	// Advect for ([1, N], [1, N])
    #pragma omp parallel for
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			gv = interpolateV(v, i, j);
			float bi = i - dt0 * gv;

			gu = interpolateU(u, i, j);
			float bj = j - dt0 * gu;

			(*x)(i, j) = interpolateD(x0, bi, bj);
		}
	}
}

void StableFluidsSys::AdvectU(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float dt)
{
	float dt0 = dt * N;
	float gv, gu;
	//cout << "Compute advectU... " << endl;
    #pragma omp parallel for
	for (int i = 1; i <= N; i++)
	{
		for (int j = 0; j <= N; j++)
		{
			// Add the origin of U grid to the coordinate before sampling, for example, sample at (i + 0, j + 0.5) 
			// when you need backtracing the old velocity at (i, j).
			// Now you have the backward-traced velocity, minus it from the current position (i + 0, j + 0.5), then sample the velocity again.
			gv = interpolateV(v, i, j + 0.5);
			float bi = i - dt0 * gv;

			gu = interpolateU(u, i, j + 0.5);
			float bj = j + 0.5 - dt0 * gu;

			(*x)(i, j) = interpolateU(x0, bi, bj);
		}
	}
}

void StableFluidsSys::AdvectV(int N, ArrayXs * x, ArrayXs * x0, ArrayXs * u, ArrayXs * v, float dt)
{
	//cout << "Compute advectV... " << endl;
	float dt0 = dt * N;
	float gv, gu;
    #pragma omp parallel for
	for (int i = 0; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			gv = interpolateV(v, i + 0.5, j);
			float bi = i + 0.5 - dt0 * gv;

			gu = interpolateU(u, i + 0.5, j);
			float bj = j - dt0 * gu;

			(*x)(i, j) = interpolateV(x0, bi, bj);
		}
	}
}

void StableFluidsSys::Project(int N, ArrayXs * u, ArrayXs * v, ArrayXs * u0, ArrayXs * v0)
{
	ArrayXs div(N + 2, N + 2);   // The divergence of the velocity field
	ArrayXs p(N + 2, N + 2);     // Pressure

	div.setZero();
	p.setZero();

	float h = 1.0 / N;

	// Set solid boundary conditions, 0 the most top and bottom row / left and right column of u0, v0
	// i.e., set the velocities that live normal to a solid cell to zero
	//for (int i = 0; i <= N + 1; i++) {// u0 cells (green dots in handout)
	//	(*u0)(i, 0) = 0;   //left column
	//	(*u0)(i, N) = 0;   //right column
	//}
	//for (int j = 0; j <= N; j++) {
	//	(*u0)(0, j) = 0;  //top row
	//	(*u0)(N + 1, j) = 0;   //bottom row
	//}

	//for (int i = 0; i <= N; i++) {// v0 cells (red dots in handout)
	//	(*v0)(i, 0) = 0;   //left column
	//	(*v0)(i, N + 1) = 0;   //right column
	//}
	//for (int j = 0; j <= N + 1; j++) {
	//	(*v0)(0, j) = 0;  //top row
	//	(*v0)(N, j) = 0;   //bottom row
	//}
	SetBndCondns(N, 0, &div); SetBndCondns(N, 0, &p);

	// Compute divergence of the velocity field, note the divergence field is available from ([1, N], [1, N])
    #pragma omp parallel for
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			div(i, j) = ((*u0)(i, j) - (*u0)(i, j - 1) + (*v0)(i, j) - (*v0)(i - 1, j)) / h;
		}
	}

	// Solve for pressure inside the region ([1, N], [1, N])    
	for (int k = 0; k < 10; k++)
	{
		#pragma omp parallel for
		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= N; j++) // IMPORTANT: DO NOT MODIFY THE LOOP ORDER
			{
				// Separate by numerator and denominator s.t. p(i,j) = A/B, where
				// A = -div(i,j)*h*h;   B = 4;
				scalar A = -div(i, j) *h *h;
				int B = 4;
				//Check if it is the top-most row
				if ((i - 1) == 0) { B--; }
				else if ((i - 1) != 0) { A += p(i - 1, j); }

				//Check if it is the bottom-most row
				if ((i + 1) == (N + 1)) { B--; }
				else if ((i + 1) != N + 1) { A += p(i + 1, j); }

				//Check if it is the left-most column
				if ((j - 1) == 0) { B--; }
				else if ((j - 1) != 0) { A += p(i, j - 1); }

				//Check if it is the right-most column
				if ((j + 1) == (N + 1)) { B--; }
				else if ((j + 1) != (N + 1)) { A += p(i, j + 1); }

				assert(B != 0);
				p(i, j) = A / B;
			}
		}
	}

	(*u) = (*u0);
	(*v) = (*v0);

	// Apply pressure to correct velocities ([1, N], [1, N)) for u, ([1, N), [1, N]) for v  
    #pragma omp parallel for
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j < N; j++)
		{
			(*u)(i, j) -= (p(i, j + 1) - p(i, j)) / (h);

			(*v)(j, i) -= (p(j + 1, i) - p(j, i)) / (h);
		}
	}
	
	SetBndCondnsU(N, u);
	SetBndCondnsV(N, v);

}


