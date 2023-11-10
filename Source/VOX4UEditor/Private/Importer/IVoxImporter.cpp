// Copyright 2016-2023 mik14a / Admix Network. All Rights Reserved.

#include "IVoxImporter.h"
#include "VoxMaterial.h"

DEFINE_LOG_CATEGORY_STATIC(LogVoxImporter, Log, All)

void IVoxImporter::LoadPalette(const FVoxChunkRgba& InRgba, TArray<FColor>& OutPalette) {
	const auto& Palettes = InRgba.Palettes;
	auto EnablePalette = 0 != Palettes.Num();
	if (EnablePalette) {
		for (const auto& color : Palettes) {
			OutPalette.Add(FColor(color.R, color.G, color.B, color.A));
		}
	} else {
		for (const auto& color : FVox::DefaultPalette) {
			OutPalette.Add(FColor(color));
		}
	}
}

void IVoxImporter::LoadMaterial(const FVox::MaterialT& InMaterials, TArray<FVoxMaterial>& OutMaterials)
{
	for (const auto& InMaterial : InMaterials) {
		auto Material = FVoxMaterial(ForceInit);
		for (const auto& property : InMaterial.Value.Properties) {
			auto& Key = property.Key;
			auto& Value = property.Value;
			if (0 == Key.Compare("_type")) {
				if (0 == Value.Compare("_diffuse")) Material.Type = EVoxMaterialType::DIFFUSE;
				else if (0 == Value.Compare("_metal")) Material.Type = EVoxMaterialType::METAL;
				else if (0 == Value.Compare("_glass")) Material.Type = EVoxMaterialType::GLASS;
				else if (0 == Value.Compare("_emit")) Material.Type = EVoxMaterialType::EMIT;
			} else if (0 == Key.Compare("_metal")) Material.Metallic = FCString::Atof(*Value);
			else if (0 == Key.Compare("_rough")) Material.Roughness = FCString::Atof(*Value);
			else if (0 == Key.Compare("_flux")) Material.Emissive = FCString::Atof(*Value);
			else if (0 == Key.Compare("_spec")) Material.Specular = FCString::Atof(*Value);
			else if (0 == Key.Compare("_weight")) Material.Weight = FCString::Atof(*Value);
			else if (0 == Key.Compare("_alpha")) Material.Alpha = FCString::Atof(*Value);
			else if (0 == Key.Compare("_ri")) Material.IOR = FCString::Atof(*Value);
			else if (0 == Key.Compare("_att")) Material.Att = FCString::Atof(*Value);
			else if (0 == Key.Compare("_trans")) Material.Transparency = FCString::Atof(*Value);
			else if (0 == Key.Compare("_g")) Material.Phase = FCString::Atof(*Value);
			else UE_LOG(LogVoxImporter, Warning, TEXT("Unsupported Material property: %s"), *Key);
		}
		OutMaterials.Emplace(Material);
	}
}
