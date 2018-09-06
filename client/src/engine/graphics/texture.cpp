#include "../../stdafx.h"

#include "texture.h"

namespace Nixie
{
	Texture::Texture()
	{
		texture_view_ = nullptr;
	}

	bool Texture::Init(const wchar_t* file_path)
	{
		HRESULT hr;

		hr = DirectX::CreateWICTextureFromFile(
			D3D::GetSingleton()->GetDevice(),
			file_path,
			&texture_,
			&texture_view_);
		if (FAILED(hr))
			return false;

		return true;
	}

	void Texture::Release()
	{
		if (texture_view_)
		{
			texture_view_->Release();
			texture_view_ = nullptr;
		}
	}

	ID3D11ShaderResourceView* Texture::GetTextureView()
	{
		return texture_view_;
	}
}