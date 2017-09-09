#pragma once
#include<iostream>
template<typename T>
class Vec4
{
public:
	Vec4(const T& x, const T& y, const T& z, const T& w)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	};
	Vec4(const Vec4&right) {
		*this = right;
	};
	Vec4() {
		for (int i = 0; i < 4; i++)
			v[i] = T();
	};
	const Vec4& operator=(const Vec4&right) {
		for (int i = 0; i < 4; i++)
			this->v[i] = right.v[i];
		return *this;
	};
	const Vec4 operator+(const Vec4& right)const
	{
		Vec4<T> ret;
		for (int i = 0; i < 4; i++)
			ret.v[i] = this->v[i] + right.v[i];
		return ret;
	};
	const Vec4 operator-()const
	{
		Vec4<T> ret(-this->x(), -this->y(), -this->z(), -this->w());
		return ret;
	}
	const Vec4 operator-(const Vec4&right)
	{
		Vec4<T> ret = *this + (-right);
		return ret;
	}
	const T	operator*(const Vec4& right)const {
		T ret = T(0);
		for (int i = 0; i < 4; i++)
			ret += this->v[i] * right.v[i];
		return ret;
	};
	const Vec4 operator*(const T&right)const {
		Vec4 ret;
		for (int i = 0; i < 4; i++)
			ret.v[i] = this->v[i] * right;
		return ret;
	};
	friend const Vec4 operator*(const T& left, const Vec4& right)
	{
		return right*left;
	}
	friend const Vec4 Cross(const Vec4&left, const Vec4&right)
	{
		T x = left.y()*right.z() - left.z()*right.y();
		T y = -(left.x()*right.z() - left.z()*right.x());
		T z = left.x()*right.y() - left.y()*right.x();
		return Vec4<T>(x, y, z, T(0));
	};
	friend const Vec4 Dot(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x()*right.x(), left.y()*right.y(), left.z()*right.z(), left.w()*right.w());
	}
	virtual ~Vec4() {};

	friend std::ostream& operator<<(std::ostream& out, const Vec4<T>& right)
	{
		std::cout;
		out << "x = " << right.x() << " y = " << right.y() << " z = " << right.z() << " w =" << right.w() << std::endl;
		return out;
	}

private:
	const T& x()const { return v[0]; };
	const T& y()const { return v[1]; };
	const T& z()const { return v[2]; };
	const T& w()const { return v[3]; };
	T v[4];	
};

typedef Vec4<double> Vec4d;
typedef Vec4<float>	 Vec4f;
typedef Vec4<int>    Vec4i;
