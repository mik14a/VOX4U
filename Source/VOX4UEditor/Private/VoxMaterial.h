#pragma once

#include "CoreMinimal.h"

enum class EVoxMaterialType : uint8
{
	DIFFUSE = 0,	// _diffuse
	METAL,			// _metal
	GLASS,			// _glass
	EMIT			// _emit
};

struct FVoxMaterial
{
	EVoxMaterialType Type = EVoxMaterialType::DIFFUSE; // _type
	float Metallic;			// _metal
	float Roughness;		// _rough
	float Emissive;			// _flux
	float Specular;			// _spec	
	float Weight;			// _weight
	float Alpha;			// _alpha
	float IOR;				// _ri (_ior = _ri - 1.0)
	float Att;				// _att
	float Transparency;		// _trans
	float Phase;			// _g
};
