#pragma once
#include <math.h>
#include <cassert>
#include "Vector.h"

namespace Havtorn 
{
	template <class T>
	class Matrix4x4 {
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();
		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T> &matrix);
		// () operator for accessing element (row, column) for read/write or read, respectively.
		T &operator()(const int row, const int column);
		const T &operator()(const int row, const int column) const;
		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(T angle_in_radians);
		static Matrix4x4<T> CreateRotationAroundY(T angle_in_radians);
		static Matrix4x4<T> CreateRotationAroundZ(T angle_in_radians);
		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T> &matrix_to_transpose);
		Matrix4x4<T> operator+(const Matrix4x4<T> &matrix);
		Matrix4x4<T> &operator+=(const Matrix4x4<T> &matrix);
		Matrix4x4<T> operator-(const Matrix4x4<T> &matrix);
		Matrix4x4<T> &operator-=(const Matrix4x4<T> &matrix);
		Matrix4x4<T> operator*(const Matrix4x4<T> &matrix);
		Matrix4x4<T> &operator*=(const Matrix4x4<T> &matrix);
		//Vector4<T> operator*(const Vector4<T> &vector);
		Matrix4x4<T> &operator=(const Matrix4x4<T> &matrix);
		bool &operator==(const Matrix4x4<T> &matrix) const;

	private:
		T Matrix[4][4];
	};

	template <class T>
	Matrix4x4<T>::Matrix4x4<T>() {
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				if (row == column) {
					Matrix[row][column] = 1;
				} else {
					Matrix[row][column] = 0;
				}
			}
		}
	}

	template <class T>
	Matrix4x4<T>::Matrix4x4<T>(const Matrix4x4<T> &matrix) {
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				Matrix[row][column] = matrix(row + 1, column + 1);
			}
		}
	}

	template <class T>
	T &Matrix4x4<T>::operator()(const int row, const int column) {
		assert(row != 0 && column != 0);
		return Matrix[row-1][column-1];
	}

	template <class T>
	const T &Matrix4x4<T>::operator()(const int row, const int column) const {
		assert(row != 0 && column != 0);
		return Matrix[row-1][column-1];
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T angle_in_radians) {
		Matrix4x4<T> matrix = Matrix4x4<T>();
		matrix(2, 2) = (T)cos(static_cast<double>(angle_in_radians));
		matrix(2, 3) = (T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 2) = -(T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 3) = (T)cos(static_cast<double>(angle_in_radians));
		return matrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T angle_in_radians) {
		Matrix4x4<T> matrix = Matrix4x4<T>();
		matrix(1, 1) = (T)cos(static_cast<double>(angle_in_radians));
		matrix(1, 3) = -(T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 1) = (T)sin(static_cast<double>(angle_in_radians));
		matrix(3, 3) = (T)cos(static_cast<double>(angle_in_radians));
		return matrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T angle_in_radians) {
		Matrix4x4<T> matrix = Matrix4x4<T>();
		matrix(1, 1) = (T)cos(static_cast<double>(angle_in_radians));
		matrix(1, 2) = (T)sin(static_cast<double>(angle_in_radians));
		matrix(2, 1) = -(T)sin(static_cast<double>(angle_in_radians));
		matrix(2, 2) = (T)cos(static_cast<double>(angle_in_radians));
		return matrix;
	}

	// Static function for creating a transpose of a matrix.
	template <class T>
	Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T> &matrix_to_transpose) {
		Matrix4x4<T> matrix = Matrix4x4<T>();
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				matrix(row + 1, column + 1) = matrix_to_transpose(column + 1, row + 1);
			}
		}
		return matrix;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4<T> &matrix) {
		Matrix4x4<T> matrix_result = Matrix4x4<T>();
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				matrix_result(row + 1, column + 1) = Matrix[row][column] + matrix(row + 1, column + 1);
			}
		}
		return matrix_result;
	}

	template <class T>
	Matrix4x4<T> &Matrix4x4<T>::operator+=(const Matrix4x4<T> &matrix) {
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				Matrix[row][column] += matrix(row + 1, column + 1);
			}
		}
		return *this;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::operator-(const Matrix4x4<T> &matrix) {
		Matrix4x4<T> matrix_result = Matrix4x4<T>();
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				matrix_result(row + 1, column + 1) = Matrix[row][column] - matrix(row + 1, column + 1);
			}
		}
		return matrix_result;
	}
	template <class T>
	Matrix4x4<T> &Matrix4x4<T>::operator-=(const Matrix4x4<T> &matrix) {
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				Matrix[row][column] -= matrix(row + 1, column + 1);
			}
		}
		return *this;
	}

	template <class T>
	Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T> &matrix) {
		Matrix4x4<T> matrix_result = Matrix4x4<T>();
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				matrix_result(row + 1, column + 1) = Matrix[row][0] * matrix(1, column + 1);
				matrix_result(row + 1, column + 1) += Matrix[row][1] * matrix(2, column + 1);
				matrix_result(row + 1, column + 1) += Matrix[row][2] * matrix(3, column + 1);
				matrix_result(row + 1, column + 1) += Matrix[row][3] * matrix(4, column + 1);
			}
		}
		return matrix_result;
	}

	template <class T>
	Matrix4x4<T> &Matrix4x4<T>::operator*=(const Matrix4x4<T> &matrix) {
		Matrix4x4<T> matrix_result = Matrix4x4<T>();
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				matrix_result(row + 1, column + 1) = Matrix[row][0] * matrix(1, column + 1);
				matrix_result(row + 1, column + 1) += Matrix[row][1] * matrix(2, column + 1);
				matrix_result(row + 1, column + 1) += Matrix[row][2] * matrix(3, column + 1);
				matrix_result(row + 1, column + 1) += Matrix[row][3] * matrix(4, column + 1);
			}
		}
		*this = matrix_result;
		return *this;
	}

	//template <class T>
	//Vector4<T> Matrix4x4<T>::operator*(const Vector4<T> &vector) {
	//	Vector4<T> vector_result;
	//	Vector4<T> temp1 = Vector4<T>(_matrix[0][0], _matrix[1][0], _matrix[2][0], _matrix[3][0]);
	//	Vector4<T> temp2 = Vector4<T>(_matrix[0][1], _matrix[1][1], _matrix[2][1], _matrix[3][1]);
	//	Vector4<T> temp3 = Vector4<T>(_matrix[0][2], _matrix[1][2], _matrix[2][2], _matrix[3][2]);
	//	Vector4<T> temp4 = Vector4<T>(_matrix[0][3], _matrix[1][3], _matrix[2][3], _matrix[3][3]);
	//	vector_result.x = vector.Dot(temp1);
	//	vector_result.y = vector.Dot(temp2);
	//	vector_result.z = vector.Dot(temp3);
	//	vector_result.w = vector.Dot(temp4);
	//	return vector_result;
	//}

	template<class T>
	Vector4<T> operator*(Matrix4x4<T> matrix, Vector4<T> vector) {
		Vector4<T> vector_result;
		Vector4<T> temp1 = Vector4<T>(matrix(1, 1), matrix(2, 1), matrix(3, 1), matrix(4, 1));
		Vector4<T> temp2 = Vector4<T>(matrix(1, 2), matrix(2, 2), matrix(3, 2), matrix(4, 2));
		Vector4<T> temp3 = Vector4<T>(matrix(1, 3), matrix(2, 3), matrix(3, 3), matrix(4, 3));
		Vector4<T> temp4 = Vector4<T>(matrix(1, 4), matrix(2, 4), matrix(3, 4), matrix(4, 4));
		vector_result._x = vector.Dot(temp1);
		vector_result._y = vector.Dot(temp2);
		vector_result._z = vector.Dot(temp3);
		vector_result._w = vector.Dot(temp4);
		return vector_result;
	}

	template<class T>
	Vector4<T> operator*(Vector4<T> vector, Matrix4x4<T> matrix) {
		Vector4<T> vector_result;
		Vector4<T> temp1 = Vector4<T>(matrix(1, 1), matrix(2, 1), matrix(3, 1), matrix(4, 1));
		Vector4<T> temp2 = Vector4<T>(matrix(1, 2), matrix(2, 2), matrix(3, 2), matrix(4, 2));
		Vector4<T> temp3 = Vector4<T>(matrix(1, 3), matrix(2, 3), matrix(3, 3), matrix(4, 3));
		Vector4<T> temp4 = Vector4<T>(matrix(1, 4), matrix(2, 4), matrix(3, 4), matrix(4, 4));
		vector_result._x = vector.Dot(temp1);
		vector_result._y = vector.Dot(temp2);
		vector_result._z = vector.Dot(temp3);
		vector_result._w = vector.Dot(temp4);
		return vector_result;
	}

	template <class T>
	Matrix4x4<T> &Matrix4x4<T>::operator=(const Matrix4x4<T> &matrix) {
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				Matrix[row][column] = matrix(row + 1, column + 1);
			}
		}
		return *this;
	}

	template <class T>
	bool &Matrix4x4<T>::operator==(const Matrix4x4<T> &matrix) const {
		bool equal = true;
		for (unsigned int row = 0; row < 4; ++row) {
			for (unsigned int column = 0; column < 4; ++column) {
				if (Matrix[row][column] != matrix(row + 1, column + 1)) {
					equal = false;
				}
			}
		}
		return equal;
	}
}