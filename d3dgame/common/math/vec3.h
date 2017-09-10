#pragma once
#include<iostream>
template<typename T>
class Vec3
{
public:
	Vec3(const T& x, const T& y, const T& z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	};
	Vec3(const Vec3&right) {
		*this = right;
	};
	Vec3() {
		for (int i = 0; i < 3; i++)
			v[i] = T();
	};
	const Vec3& operator=(const Vec3&right) {
		for (int i = 0; i < 3; i++)
			this->v[i] = right.v[i];
		return *this;
	};
	const Vec3 operator+(const Vec3& right)const
	{
		Vec3<T> ret;
		for (int i = 0; i < 3; i++)
			ret.v[i] = this->v[i] + right.v[i];
		return ret;
	};
	const Vec3 operator-()const
	{
		Vec3<T> ret(-this->x(), -this->y(), -this->z());
		return ret;
	}
	const Vec3 operator-(const Vec3&right)const
	{
		Vec3<T> ret = *this + (-right);
		return ret;
	}
	const T	operator*(const Vec3& right)const {
		T ret = T(0);
		for (int i = 0; i < 3; i++)
			ret += this->v[i] * right.v[i];
		return ret;
	};
	const Vec3 operator*(const T&right)const {
		Vec3 ret;
		for (int i = 0; i <3; i++)
			ret.v[i] = this->v[i] * right;
		return ret;
	};
	const Vec3 normalize()
	{
		T div = 1 / sqrt((double)(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
		*this = *this*div;
		return *this;
	}
	friend const Vec3 operator*(const T& left, const Vec3& right)
	{
		return right*left;
	}
	friend const Vec3 Cross(const Vec3&left, const Vec3&right)
	{
		T x = left.y()*right.z() - left.z()*right.y();
		T y = -(left.x()*right.z() - left.z()*right.x());
		T z = left.x()*right.y() - left.y()*right.x();
		return Vec3<T>(x, y, z);
	};
	friend const Vec3 Dot(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x()*right.x(), left.y()*right.y(), left.z()*right.z());
	}
	virtual ~Vec3() {};

	friend std::ostream& operator<<(std::ostream& out, const Vec3<T>& right)
	{
		out << "x = " << right.x() << " y = " << right.y() << " z = " << right.z() << std::endl;
		return out;
	}
	const T& operator[] (int index) const {
		return v[index];
	}
	T& operator[](int index)
	{
		return v[index];
	}
private:
	const T& x()const { return v[0]; };
	const T& y()const { return v[1]; };
	const T& z()const { return v[2]; };
	T v[3];
};

typedef Vec3<double> Vec3d;
typedef Vec3<float>	 Vec3f;
typedef Vec3<int>    Vec3i;

typedef Vec3f Point3f;
typedef Vec3f Dir3f;