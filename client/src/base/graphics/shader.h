#ifndef COLORSHADER_H
#define COLORSHADER_H

#pragma once

#include <fstream>
#include <string>

#include "d3d.h"

using namespace DirectX;

class Shader
{
private:
	struct MatrixBufferData
	{
		XMMATRIX world_matrix;
		XMMATRIX view_matrix;
		XMMATRIX projection_matrix;
	};

public:
	Shader();

	bool Init();
	void Release();
	bool Render();

private:
	bool InitShader(WCHAR* file_path);
	void OutputShaderErrorMessage(ID3D10Blob* error_message, WCHAR* shader_path);

	bool SetShaderParameters(XMMATRIX world_matrix, XMMATRIX view_matrix, XMMATRIX projection_matrix);
	void RenderShader();

private:
	ID3D11VertexShader* vertex_shader_;
	ID3D11PixelShader* pixel_shader_;
	ID3D11InputLayout* layout_;
	ID3D11Buffer* matrix_buffer_;
};

#endif