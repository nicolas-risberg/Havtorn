#pragma once
#include <math.h>
#include <cassert>
#include "Vector.h"
#include "Matrix4x4.h"

namespace Havtorn 
{
	template <class T>
	class Matrix3x3 {
	public:
		// Creates the identity matrix.
		Matrix3x3<T>();
		// Copy Constructor.
		Matrix3x3<T>(const Matrix3x3<T> &matrix);
		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T> &matrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T &operator()(const int row, const int column);
		const T &operator()(const int row, const int column) const;
		// Static functions for creating rotation matrices.
		static Matrix3x3<T> create_rotation_around_x(T angle_in_radians);
		static Matrix3x3<T> create_rotation_around_y(T angle_in_radians);
		static Matrix3x3<T> create_rotation_around_z(T angle_in_radians);
		// Static function for creating a transpose of a matrix.
		static Matrix3x3<T> transpose(const Matrix3x3<T> &matrix_to_transpose);
		// Operator overloads
		Matrix3x3<T> operator+(const Matrix3x3<T> &matrix);
		Matrix3x3<T> &operator+=(const Matrix3x3<T> &matrix);
		Matrix3x3<T> operator-(const Matrix3x3<T> &matrix);
		Matrix3x3<T> &operator-=(const Matrix3x3<T> &matrix);
		Matrix3x3<T> operator*(const Matrix3x3<T> &matrix);
		Matrix3x3<T> &operator*=(const Matrix3x3<T> &matrix);
		Matrix3x3<T> &operator=(const Matrix3x3<T> &matrix);
		bool &operator==(const Matrix3x3<T> &matrix) const;

	private:
		T _matrix[3][3];
	};

	template <class T>
	Matrix3x3<T>::Matrix3x3() {
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				if (row == column) {
					_matrix[row][column] = 1;
				} else {
					_matrix[row][column] = 0;
				}
			}
		}
	}

	template <class T>
	Matrix3x3<T>::Matrix3x3(const Matrix3x3<T> &matrix) {
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				_matrix[row][column] = matrix(row + 1, column + 1);
			}
		}
	}

	template <class T>
	Matrix3x3<T>::Matrix3x3(const Matrix4x4<T> &matrix) {
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				_matrix[row][column] = matrix(row + 1, column + 1);
			}
		}
	}

	template <class T>
	T &Matrix3x3<T>::operator()(const int row, const int column) {
		//return const_cast<T&>(static_cast<const T[][]&>(*this->_matrix)[row][column]);
		assert(row != 0 && column != 0);
		return _matrix[row-1][column-1];
	}

	template <class T>
	const T &Matrix3x3<T>::operator()(const int row, const int column) const {
		assert(row != 0 && column != 0);
		return _matrix[row-1][column-1];
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::create_rotation_around_x(T angle_in_radians) {
		Matrix3x3<T> matrix = Matrix3x3<T>();
		matrix(2, 2) = (T)cos(static_cast<double>(angle_in_radians));
		matrix(2, 3) = (T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 2) = -(T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 3) = (T)cos(static_cast<double>(angle_in_radians));
		return matrix;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::create_rotation_around_y(T angle_in_radians) {
		Matrix3x3<T> matrix = Matrix3x3<T>();
		matrix(1, 1) = (T)cos(static_cast<double>(angle_in_radians));
		matrix(1, 3) = -(T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 1) = (T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 3) = (T)cos(static_cast<double>(angle_in_radians));
		return matrix;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::create_rotation_around_z(T angle_in_radians) {
		Matrix3x3<T> matrix = Matrix3x3<T>();
		matrix(1, 1) = (T)cos(static_cast<double>(angle_in_radians));
		matrix(1, 2) = (T)sin(static_cast<double>(angle_in_radians));
		matrix(2, 1) = -(T)sin(static_cast<double>(angle_in_radians));
		matrix(2, 2) = (T)cos(static_cast<double>(angle_in_radians));
		return matrix;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::transpose(const Matrix3x3<T> &matrix_to_transpose) {
		Matrix3x3<T> matrix = Matrix3x3<T>();
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				matrix(row + 1, column + 1) = matrix_to_transpose(column + 1, row + 1);
			}
		}
		return matrix;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::operator+(const Matrix3x3<T> &matrix) {
		Matrix3x3<T> matrix_result = Matrix3x3<T>();
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				matrix_result(row + 1, column + 1) = _matrix[row][column] + matrix(row + 1, column + 1);
			}
		}
		return matrix_result;
	}

	template <class T>
	Matrix3x3<T> &Matrix3x3<T>::operator+=(const Matrix3x3<T> &matrix) {
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				_matrix[row][column] += matrix(row + 1, column + 1);
			}
		}
		return *this;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::operator-(const Matrix3x3<T> &matrix) {
		Matrix3x3<T> matrix_result = Matrix3x3<T>();
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				matrix_result(row + 1, column + 1) = _matrix[row][column] - matrix(row + 1, column + 1);
			}
		}
		return matrix_result;
	}

	template <class T>
	Matrix3x3<T> &Matrix3x3<T>::operator-=(const Matrix3x3<T> &matrix) {
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				_matrix[row][column] -= matrix(row + 1, column + 1);
			}
		}
		return *this;
	}

	template <class T>
	Matrix3x3<T> Matrix3x3<T>::operator*(const Matrix3x3<T> &matrix) {
		Matrix3x3<T> matrix_result = Matrix3x3<T>();
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				matrix_result(row + 1, column + 1) = _matrix[row][0] * matrix(1, column + 1);
				matrix_result(row + 1, column + 1) += _matrix[row][1] * matrix(2, column + 1);
				matrix_result(row + 1, column + 1) += _matrix[row][2] * matrix(3, column + 1);
			}
		}
		return matrix_result;
	}

	template <class T>
	Matrix3x3<T> &Matrix3x3<T>::operator*=(const Matrix3x3<T> &matrix) {
		Matrix3x3<T> matrix_result = Matrix3x3<T>();
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				matrix_result(row + 1, column + 1) = _matrix[row][0] * matrix(1, column + 1);
				matrix_result(row + 1, column + 1) += _matrix[row][1] * matrix(2, column + 1);
				matrix_result(row + 1, column + 1) += _matrix[row][2] * matrix(3, column + 1);
			}
		}
		*this = matrix_result;
		return *this;
	}

	template<class T>
	Vector3<T> operator*(Matrix3x3<T> matrix, Vector3<T> vector) {
		Vector3<T> vector_result;
		Vector3<T> temp1 = Vector3<T>(matrix(1, 1), matrix(2, 1), matrix(3, 1));
		Vector3<T> temp2 = Vector3<T>(matrix(1, 2), matrix(2, 2), matrix(3, 2));
		Vector3<T> temp3 = Vector3<T>(matrix(1, 3), matrix(2, 3), matrix(3, 3));
		vector_result._x = vector.Dot(temp1);
		vector_result._y = vector.Dot(temp2);
		vector_result._z = vector.Dot(temp3);
		return vector_result;
	}

	template<class T>
	Vector3<T> operator*(Vector3<T> vector, Matrix3x3<T> matrix) {
		Vector3<T> vector_result;
		Vector3<T> temp1 = Vector3<T>(matrix(1, 1), matrix(2, 1), matrix(3, 1));
		Vector3<T> temp2 = Vector3<T>(matrix(1, 2), matrix(2, 2), matrix(3, 2));
		Vector3<T> temp3 = Vector3<T>(matrix(1, 3), matrix(2, 3), matrix(3, 3));
		vector_result._x = vector.Dot(temp1);
		vector_result._y = vector.Dot(temp2);
		vector_result._z = vector.Dot(temp3);
		return vector_result;
	}

	template<class T>
	Matrix3x3<T> &Matrix3x3<T>::operator=(const Matrix3x3<T> &matrix) {
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				_matrix[row][column] = matrix(row + 1, column + 1);
			}
		}
		return *this;
	}

	template<class T>
	bool &Matrix3x3<T>::operator==(const Matrix3x3<T> &matrix) const {
		bool equal = true;
		for (unsigned int row = 0; row < 3; ++row) {
			for (unsigned int column = 0; column < 3; ++column) {
				if (_matrix[row][column] != matrix(row + 1, column + 1)) {
					equal = false;
				}
			}
		}
		return equal;
	}
}