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
		/// D