/***************************************************************************************************/
/*                                               Deep Learning Developing Kit                                                   */
/*								        		 	           Math Library 	                                                              */
/*								        		 	                Vector  	                                                              */
/*                                                   www.tianshicangxie.com                                                        */
/*                                      Copyright © 2015-2018 Celestial Tech Inc.                                          */
/***************************************************************************************************/
#pragma once

// Header files
#include <iostream>
#include <vector>

#include "MathLibError.h"
#include "MathTool.hpp"
#include "Matrix.hpp"

/***************************************************************************************************/
// Namespace : MathLib
/// Provide basic mathematic support and calculation tools for different algorithms.
namespace MathLib
{
	// Type of Vector.
	enum class VectorType {
		Zero,
		Ones,
		Random,
		Identity
	};

	/***************************************************************************************************/
	// Class : Vector
	/// Implemented in std::vector.
	/// Specialized for mechine learning purpose.
	template<class T>
	class Vector
	{
	public: // Constructors

		// Default constructor
		/// Take no parameters.
		/// After default constructor and before use the Vector object, Init() should be involked.
		Vector(void);
		// Constructor (Using Size and Type)
		/// Specified the size of Vector.
		Vector(const size_t _n, const VectorType _type = VectorType::Zero);
		// Constructor (Using given Data)
		/// Using data from a given pointer, which is pointed to an array, to initialize the Vector.
		Vector(const std::initializer_list<int> & _list);

	public: // Initializing

		// Initializing function
		/// Initializing the Vector after defined by default constructor.
		void Init(const size_t _n, const VectorType _type = VectorType::Zero);

	public: // Arithmatic

		// Inner product function
		/// Calculate the inner product of two Vectors.
		static T InnerProduct(const Vector<T> & _first, const Vector<T> & _second);
		// Dot product function
		/// Dot product is simply another name of inner product.
		static T DotProduct(const Vector<T> & _first, const Vector<T> & _second);
		// Outer product function
		/// Calculate the outer product of two Vectors.
		static T OuterProduct(const Vector<T> & _first, const Vector<T> & _second);
		// Scalar product function
		/// Calculate the Scalar product of a Vector and a scalar.
		static T ScalarProduct(const T & _first, const Vector<T> & _second);

	public: // Quantification

		// Size function
		/// Return the size of the Vector.
		const size_t Size(void) const;
		// Sum function
		/// Add up all the element in the Vector.
		T Sum(void) const;
		// Average function
		/// Calculate the average value of all the element in the Vector.
		T Average(void) const;
		// Max function
		/// Get the value of the max element in the Vector.
		T Max(void) const;
		// Min function
		/// Get the value of the min element in the Vector.
		T Min(void) const;

	public: // Transformation

		//// Represent a Vector in form of Matrix.
		///// A Vector is interpreted as a 1xn Martix.
		//Matrix<T> Matrixfy(void);

	public: // Pointer

		// Pointer
		T * data() { return this->_data[0]; }
		// Const pointer
		const T * data() const { return this->_data[0]; }

	public: // Operator Overloading

		// "( )" operator
		/// Used for accessing the element in the Vector.
		inline T operator()(size_t _j) const
		{
			return this->_data[_j];
		}

		/// Used for referencing the element in the Vector.
		inline T & operator()(size_t _j)
		{
			return this->_data[_j];
		}

		// "<<" operator
		/// Used for streaming in format.
		friend std::ostream & operator<<(std::ostream& _outstream, Vector<T>& _vec)
		{
			_outstream << typeid(_vec).name() << " ";
			_outstream << "|";
			for (size_t j = 0; j < _vec.n; j++)
			{
				_outstream << _vec(j);
				if (j != _vec.n - 1)	_outstream << " ";
			}
			_outstream << "|" << std::endl;
			return _outstream;
		}

		// "=" operator
		Vector<T> & operator = (const Vector<T> & _other)
		{
			try
			{
				if (this->n != _other.n)
					throw unmatched_size();
				else
					if (this != &_other)
					{
						_data = _other._data;
						n = _other.n;
					}		
			}
			catch (std::exception& except) { ExceptionHandle(except); }
			return (*this);
		}

		// "+" operator
		/// Addition of two Vectors.
		Vector<T> operator + (const Vector<T> & _other) const
		{
			const Vector<T> & self = *this;
			Vector<T> temp(n);
			try
			{
				if (self.n != _other.n)
					throw unmatched_size();
				else
					for (size_t j = 0; j < self.n; j++)
						temp(j) = self(j) + _other(j);
			}
			catch (std::exception& except) { ExceptionHandle(except); }
			return temp;
		}

		/// Addition of a Vector and a scalar.
		/// Add scalar to each element in the Vector.
		Vector<T> operator + (const T & _other) const
		{
			const Vector<T> & self = *this;
			Vector<T> temp(n);
			for (size_t j = 0; j < self.n; j++)
				temp(j) = self(j) + _other;
			return temp;
		}

		// "+=" operator
		/// Add another Vector to this Vector.
		void operator += (const Vector<T> & _other)
		{
			Vector<T> & self = *this;
			try
			{
				if (self.n != _other.n)
					throw unmatched_size();
				else
					for (size_t j = 0; j < self.n; j++)
						self(j) = self(j) + _other(j);
			}
			catch (std::exception& except) { ExceptionHandle(except); }
		}


		/// Add another scalar to this Vector.
		void operator += (const T & _other)
		{
			Vector<T> & self = *this;
			for (size_t j = 0; j < self.n; j++)
				self(j) = self(j) + _other;
		}

		// "-" operator
		/// Substraction of two Vectors.
		Vector<T> operator - (const Vector<T> & _other) const
		{
			const Vector<T> & self = *this;
			Vector<T> temp(n);
			try
			{
				if (self.n != _other.n)
					throw unmatched_size();
				else
					for (size_t j = 0; j < self.n; j++)
						temp(j) = self(j) - _oth