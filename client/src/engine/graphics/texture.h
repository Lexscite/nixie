#ifndef NIXIE_TEXTURE_H_
#define NIXIE_TEXTURE_H_

#pragma once

#include "directx_manager.h"


namespace nixie
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool Init(std::string file_path);

		ID3D11ShaderResourceView* GetTextureView();

	private:
		ID3D11Resource* texture_;
		ID3D11ShaderResourceView* texture_view_;
	};
}

#endif
