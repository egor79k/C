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

	Vector3 operator- (const Vector3 &v) const;

	Vector3 operator* (const Vector3 &v) const;

	Vector3 operator! () const;

	double operator^ (const Vector3 &v) const;
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
Vector3<T> Vector3<T>::operator- (const Vector3<T> &v) const
{
	return Vector3 (x - v.x, y - v.y, z - v.z);
}



template<typename T>
Vector3<T> Vector3<T>::operator* (const Vector3<T> &v) const
{
	return Vector3 (x * v.x, y * v.y, z * v.z);
}



template<typename T>
Vector3<T> Vector3<T>::operator! () const
{
	return Vector3 (y, -x, 0);
}



template<typename T>
double Vector3<T>::operator^ (const Vector3<T> &v) const
{
	return (x * v.x + y * v.y + z * v.z) / (sqrt (x * x + y * y + z * z) * sqrt (v.x * v.x + v.y * v.y + v.z * v.z));
}


#endif