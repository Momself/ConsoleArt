/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	           Math Library 	                                                              */
/*								        		 	                Matrix   	                                                              */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/
#pragma once

// Headerfiles
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

#include "MathLibError.h"
#include "MathTool.hpp"
#include "Vector.hpp"

/***************************************************************************************************/
// Namespace : MathLib
/// Provide basic mathematic support and calculation tools for different algorithms.
namespace MathLib
{
	// Type of Matrix.
	enum class MatrixType {
		Zero,
		Ones,
		Random,
		Identity
	};

	// Size of Matrix.
	struct Size
	{
		Size() = default;
		Size(size_t _m, size_t _n) : m(_m), n(_n) {}
		size_t m;
		size_t n;
	};

	/***************************************************************************************************/
	// Class : Matrix
	/// Implemented in std::vector.
	/// Specialized for mechine learning purpose.
	template<class T>
	class Matrix
	{
	public: // Constructors

		// Default constructor
		/// Take no parameters.
		/// After default constructor and before use the Matrix object, Init() should be involked.
		Matrix(void);
		// Constructor (Using Size and Type)
		/// Specified the size of Matrix.
		Matrix(const size_t _m, const size_t _n, const MatrixType _type = MatrixType::Zero);
		// Constructor (Using given Data)
		/// Using data from a given pointer, which is pointed to a 2D array, to initialize the Matrix.
		Matrix(const std::initializer_list<int> & _list);
		// Copy constructor
		Matrix(const Matrix& _mat);

		~Matrix() {
			for (size_t i = 0; i < _data.size(); i++)
			{
				_data.at(i).clear();
			}
			_data.clear();
		}

	public: // Initializing

		// Initializing function
		/// Initializing the Matrix after defined by default constructor.
		void Init(const size_t _m, const size_t _n, const MatrixType _type = MatrixType::Zero);

	public: // Quantification

		// Size function
		/// Return the size of the Matrix.
		inline const size_t ColumeSize(void) const { return m; }
		inline const size_t RowSize(void) const { return n; }
		inline const Size GetSize(void) const { return size; }
		// Sum function
		/// Add up all the element in the Matrix.
		const T Sum(void) const;
		// Average function
		/// Calculate the average value of all the element in the Matrix.
		const T Average(void) const;
		// Max function
		/// Get the value of the max element in the Matrix.
		const T Max(void) const;
		// Min function
		/// Get the value of the min element in the Matrix.
		const T Min(void) const;

	public: // Advanced Quantification

		//	Determinant function
		/// Calcutate the determinant of the Matrix.
		const T Determinant(void) const;
		// Trace
		/// Calcutate the trace of the Matrix.
		const T Trace(void) const;
		// Cofactor function
		/// Calcutate the cofactor of the Matrix.
		const T Cofactor(const size_t _i, const size_t _j) const;
		// Algebraic Cofactor function
		/// Calcutate the algebraic cofactor of the Matrix.
		const T AlgebraicCofactor(const size_t _i, const size_t _j) const;
		// Rank function
		/// Calcutate the rank of the Matrix.
		const unsigned int Rank(void) const;
		// 1-Norm
		/// Calcutate the 1-norm of the Matrix.
		const T OneNorm(void) const;
		// Forbenivs Norm
		/// Calcutate the Forbenivs norm of the Matrix.
		const T ForbenivsNorm(void) const;
		// P-Norm
		/// Calcutate the P-Norm of the Matrix.
		const T PNorm(const unsigned int _p) const;
		
	public: // Transformation

		// Clear
		void Clear(void);
		// Gaussian Elimination
		const Matrix<T> GaussianElimination(void) const;
		// Transposition matrix
		const Matrix<T> Transpostion(void) const;
		// Adjoint matrix
		const Matrix<T> Adjoint(void) const;
		// Inverse matrix
		const Matrix<T> Inverse(void) const;

	private: // Inner woking functions

		// Swap two columns
		void SwapColumn(const size_t _i, const size_t _j);
		// Resize the matrix
		void Resize(const size_t _m, const size_t _n);

	public: // Pointers

		// Pointer
		T * Data() { return this->_data[0]; }
		// Const