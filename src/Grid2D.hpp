#ifndef GRID2D_H
#define GRID2D_H
#include <iostream>
#include "MathUtilities.h"

/**
 * Representation of a two-dimensional grid of values.
 * TODO: Generalize this is any-dimensions.
 */
template<typename T>
class Grid2D {
public:
	Grid2D(std::size_t rows, std::size_t cols, T initial_value = 0)
		: m_rows(rows), m_cols(cols), m_coordDim(2)
	{
		m_data.resize(rows, cols);
		m_data.fill(initial_value);
	}

	std::size_t rows() const {
        return m_rows;
    }

	std::size_t cols() const {
        return m_rows;
    }

	std::size_t coordDim() const {
		return m_coordDim;
	}

	void clear() {
		m_data.setZero();
	}

	// Returns a pointer to the data array m_data via Eigen data() method
	void displayGrid() {
		std::cout << m_data << std::endl;
	}

	ArrayXs grid() {
		return m_data;
	}


	// Returns the data m_data via Eigen data() method
	T * data() {
		return m_data.data();
	}

    // Provide convenient access to Eigen array entry, which allows
	// one to use Eigen array operations.
    T operator() (const std::size_t i, const std::size_t j) const {
        return m_data(i, j);
    }

	// Provide convenient access to Eigen array entry, which allows
	// one to use Eigen array operations.
    T& operator() (const std::size_t i, const std::size_t j) {
        return m_data(i, j);
    }



private:
	std::size_t    m_rows;
	std::size_t    m_cols;
	std::size_t    m_coordDim; //Coordinate Dimensions
    ArrayXs        m_data;
};

#endif // GRID2D_HPP