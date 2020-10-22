#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>


template<typename T>
class Vector3
{
public:
	T x;
	T y;
	T z;

	Vector3 ();

	Vector3 (T _x, T _y, T _z);

	~Vector3 () = default;

	Vector3 operator+ (const Vector3 &v) const;

	Vector3 operator- (const Vector3 &v) const;

	Vector3 operator- () const;

	Vector3 operator* (const Vector3 &v) const;

	Vector3 operator* (T k) const;

	Vector3 operator! () const; // Normal

	double operator^ (const Vector3 &v) const; // cos(a) between two vectors

	Vector3 project (const Vector3 &v) const; // Projection of this vector on v
};



template<typename T>
Vector3<T>::Vector3 (T _x, T _y, T _z) :
	x (_x),
	y (_y),
	z (_z)
{}


template<typename T>
Vector3<T>::Vector3 () :
	x (0),
	y (0),
	z (0)
{}


template<typename T>
Vector3<T> Vector3<T>::operator+ (const Vector3<T> &v) const
{
	return Vector3 (x + v.x, y + v.y, z + v.z);
}


template<typename T>
Vector3<T> Vector3<T>::operator- (const Vector3<T> &v) const
{
	return Vector3 (x - v.x, y - v.y, z - v.z);
}


template<typename T>
Vector3<T> Vector3<T>::operator- () const
{
	return Vector3 (-x, -y, -z);
}


template<typename T>
Vector3<T> Vector3<T>::operator* (const Vector3<T> &v) const
{
	return Vector3 (x * v.x, y * v.y, z * v.z);
}


template<typename T>
Vector3<T> Vector3<T>::operator* (T k) const
{
	return Vector3 (x * k, y * k, z * k);
}


template<typename T>
Vector3<T> operator* (T k, const Vector3<T> &v)
{
	return Vector3<T> (v.x * k, v.y * k, v.z * k);
}


template<typename T>
Vector3<T> Vector3<T>::operator! () const
{
	return Vector3 (y, -x, 0);
}


template<typename T>
T scp (const Vector3<T> &v, const Vector3<T> &u) // Scalar product
{
	return v.x * u.x + v.y * u.y + v.z * u.z;
}


template<typename T>
double Vector3<T>::operator^ (const Vector3<T> &v) const
{
	return scp (*this, v) / (sqrt (x * x + y * y + z * z) * sqrt (v.x * v.x + v.y * v.y + v.z * v.z));
}


template<typename T>
Vector3<T> Vector3<T>::project (const Vector3<T> &v) const
{
	return v * (scp (*this, v) / scp (v, v));
}


#endif