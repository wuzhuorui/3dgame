#pragma once
#include<iostream>
#include"vec4.h"
template<typename T>
class mat4 
{
public:
	mat4() :val{ {1, 0, 0, 0}, { 0,1,0,0 }, { 0,0,1,0 }, {0,0,0,1}} {
	}
	mat4(const Vec4<T>& u, const Vec4<T>& v, const Vec4<T>& w, const Vec4<T>& q) :val{ {u[0],u[1],u[2],u[3]},{ v[0],v[1],v[2],v[3] },{ w[0],w[1],w[2],w[3] },{ q[0],q[1],q[2],q[3] } } {}
	const mat4& operator = (const mat4& right) {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				this->val[i][j] = right[i][j];
		return *this;
	}
	mat4(const mat4& right) {
		*this = right;
	}
	const mat4 operator*(const mat4& right)const
	{
		mat4 ret;
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				T& tmp = ret.val[row][col] = T(0);
				for (int interval = 0; interval < 4; interval++)
				{
					tmp += this->val[row][interval] * right[interval][col];
				}
			}
		}
		return ret;
	}
	const mat4& transport() {
		for(int i = 0 ; i < 4;i++)
			for (int j = i + 1; j < 4; j++)
			{
				std::swap(this->val[i][j], this->val[j][i]);
			}
		return *this;
	}
	friend const Vec4<T> operator*(const Vec4<T>&left, const mat4<T>& right) {
		Vec4<T> ret;
		for (int pos = 0; pos < 4; pos++)
		{
			ret[pos] = T(0);
			for (int interval = 0; interval < 4; interval++)
			{
				ret[pos] += left[interval] * right[interval][pos];
			}
		}
		return ret;
	}
	friend std::ostream& operator<<(std::ostream& output, const mat4& right)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				output << right.val[i][j] << " ";
			}
			output << std::endl;
		}
		return output;
	}
	const T* operator[](int index)const { return val[index]; }
private:
	T val[4][4];
};