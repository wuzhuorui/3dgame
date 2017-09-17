#pragma once
#include<iostream>
template<typename T>
class Vec2
{
public:
	Vec2(const T& x, const T& y)
	{
		v[0] = x;
		v[1] = y;
	};
	Vec2(const Vec2&right) {
		*this = right;
	};
	Vec2() {
		for (int i = 0; i < 2; i++)
			v[i] = T();
	};
	const Vec2& operator=(const Vec2&right) {
		for (int i = 0; i < 2; i++)
			this->v[i] = right.v[i];
		return *this;
	};
	const Vec2 operator+(const Vec2& right)const
	{
		Vec2<T> ret;
		for (int i = 0; i < 2; i++)
			ret.v[i] = this->v[i] + right.v[i];
		return ret;
	};
	const Vec2 operator-()const
	{
		Vec2<T> ret(-this->x(), -this->y());
		return ret;
	}
	const Vec2 operator-(const Vec2&right)const
	{
		Vec2<T> ret = *this + (-right);
		return ret;
	}
	const T	operator*(const Vec2& right)const {
		T ret = T(0);
		for (int i = 0; i < 2; i++)
			ret += this->v[i] * right.v[i];
		return ret;
	};
	const Vec2 operator*(const T&right)const {
		Vec3 ret;
		for (int i = 0; i <2; i++)
			ret.v[i] = this->v[i] * right;
		return ret;
	};
	const Vec2 normalize()
	{
		T div = 1 / sqrt((double)(v[0] * v[0] + v[1] * v[1]));
		*this = *this*div;
		return *this;
	}
	friend const Vec2 operator*(const T& left, const Vec2& right)
	{
		return right*left;
	}
	friend const Vec2 Dot(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x()*right.x(), left.y()*right.y());
	}
	~Vec2() {};

	friend std::ostream& operator<<(std::ostream& out, const Vec2<T>& right)
	{
		out << "x = " << right.x() << " y = " << right.y() << std::endl;
		return out;
	}
	const T& operator[] (int index) const {
		return v[index];
	}
	T& operator[](int index)
	{
		return v[index];
	}
	void SetX(const T& x) { v[0] = x; }
	void SetY(const T& y) { v[1] = y; }
	const T& x()const { return v[0]; };
	const T& y()const { return v[1]; };
private:
	T v[2];
};

typedef Vec2<double> Vec2d;
typedef Vec2<float>	 Vec2f;
typedef Vec2<int>    Vec2i;

typedef Vec2f Point2f;
typedef Vec2f Dir2f;
typedef Vec2i Point2i;