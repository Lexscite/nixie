#include "../../stdafx.h"

#include "material.h"


namespace Nixie
{
	bool Material::Init(std::string vs_path, std::string ps_path, std::string texture_path)
	{
		shader_ = std::make_unique<Shader>();
		if (!shader_->Init(vs_path, ps_path))
		{
			return false;
		}

		texture_ = std::make_unique<Texture>();
		if (!texture_->Init(texture_path))
		{
			return false;
		}

		return true;
	}


	bool Material::Update(DirectX::SimpleMath::Matrix world_matrix)
	{
		if (!shader_->Update(world_matrix, texture_->GetTextureView()))
		{
			return false;
		}

		return true;
	}
}