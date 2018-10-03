#ifndef VOODOO_TEXTURE_H_
#define VOODOO_TEXTURE_H_

#include "directx_manager.h"

namespace voodoo
{
	class Texture
	{
	public:
		Texture(ID3D11Resource* resource, ID3D11ShaderResourceView* shader_resource_view);
		~Texture();

		ID3D11ShaderResourceView* GetShaderResourceView();

	private:
		ID3D11Resource* resource_;
		ID3D11ShaderResourceView* shader_resource_view;
	};
}

#endif