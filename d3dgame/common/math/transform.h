#pragma once
#include"mat4.h"
#include<cmath>

extern double pi;
extern double pi_unit;

template<typename T>
const mat4<T> LookAt(const Vec4<T>&pos,const Vec4<T>&target,const Vec4<T>& up = Vec4<T>(0,1,0,0)) 
{
	Vec4<T> w = (target - pos); w.normalize();
	Vec4<T> u = Cross(up, w);
	Vec4<T> v = Cross(w, u);
	mat4<T> ret;
	ret[0][0] = u[0]; ret[0][1] = v[0]; ret[0][2] = w[0]; ret[0][3] = 0;
	ret[1][0] = u[1]; ret[1][1] = v[1]; ret[1][2] = w[1]; ret[1][3] = 0;
	ret[2][0] = u[2]; ret[2][1] = v[2]; ret[2][2] = w[2]; ret[2][3] = 0;
	ret[3][0] = -(pos*u); ret[3][1] = -(pos*v); ret[3][2] = -(pos*w); ret[3][3] = 1;
	return ret;
};

template<typename T>
const mat4<T> Perspective(T ForAngleY, T AspectRatio, T NearZ, T FarZ)
{
	T cot = T(1)/tan(ForAngleY*T(0.5));
	T div = T(1) / (FarZ - NearZ);
	mat4<T> ret;
	ret[0][0] = T(1)*cot/AspectRatio; ret[0][1] = 0;		ret[0][2] = 0;		  ret[0][3] = 0;
	ret[1][0] = 0;					  ret[1][1] = cot;      ret[1][2] = 0;		  ret[1][3] = 0;
	ret[2][0] = 0;					  ret[2][1] = 0;		ret[2][2] = FarZ*div; ret[2][3] = 1;
	ret[3][0] = 0;					  ret[3][1] = 0;	    ret[3][2] = -FarZ*NearZ*div; ret[3][3] = 0;
	return ret;
}