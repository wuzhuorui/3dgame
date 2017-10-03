//=============================================================================
// Lighting.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and lights geometry.
//=============================================================================

#include "LightHelper.fx"

void GetHeight(Wave w,float x,float z,float time,out float y,out float dx,out float dz)
{
	float  L  = w.L;
	float  S  = w.S;
	float  k  = w.k;
	float  A  = w.A;
	float  theta = (w.d1*x+w.d2*z)*2.*3.1415/L + time*2.*3.1415/L*S;
	float  sina  = sin(theta);
	float  cosa  = cos(theta);
	
	y = 2*A*pow((sina+1)/2,k);
	dx = k*w.d1*x*2.*3.1415/L *A*cosa*pow((sina+1)/2,k-1)*cosa;
	dz = k*w.d2*z*2.*3.1415/L *A*cosa*pow((sina+1)/2,k-1)*cosa;
}

cbuffer cbPerFrame
{
	DirectionalLight gDirLight;
	PointLight gPointLight;
	SpotLight gSpotLight;
	float3 gEyePosW;
	float  gTime;
	Wave wave[4];
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	Material gMaterial;
};

struct VertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	float Height[4];
	float dx[4];
	float dz[4];
	GetHeight(wave[0], vin.PosL.x,vin.PosL.z,gTime,Height[0],dx[0],dz[0]);
	GetHeight(wave[1], vin.PosL.x,vin.PosL.z,gTime,Height[1],dx[1],dz[1]);
	GetHeight(wave[2], vin.PosL.x,vin.PosL.z,gTime,Height[2],dx[2],dz[2]);

	vin.PosL.y = Height[0]+Height[1]+Height[2];
	float3 tnormal =normalize(float3(-dx[0]-dx[1]-dx[2],3,-dz[0]-dz[1]-dz[2]));
	
	vout.PosW    = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(tnormal, (float3x3)gWorldInvTranspose);
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	return vout;
}
  
float4 PS(VertexOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW); 

	float3 toEyeW = normalize(gEyePosW - pin.PosW);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;

	ComputeDirectionalLight(gMaterial, gDirLight, pin.NormalW, toEyeW, A, D, S);
	ambient += A;  
	diffuse += D;
	spec    += S;

	ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec    += S;

	//ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
	//ambient += A;

	//diffuse += D;
	//spec    += S;
	   
	float4 litColor = ambient + diffuse + spec;

	// Common to take alpha from diffuse material.
	litColor.a = gMaterial.Diffuse.a;
    return litColor;

}

technique11 WaterTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}



