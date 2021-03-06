#pragma once

#include "Core/Common.h"
#include <d3d12.h>


typedef enum ECONSTANT_BUFFER_BIND_FLAG {
	CONSTANT_BUFFER_BIND_FLAG_DYNAMIC,
	CONSTANT_BUFFER_BIND_FLAG_STATIC
} ECONSTANT_BUFFER_BIND_FLAG;

typedef enum ETEXTURE_USAGE {
	TEXTURE_USAGE_SHADER_RESOURCE	= 0x00000001,
	TEXTURE_USAGE_UNORDERED_ACCESS	= 0x00000010,
	TEXTURE_USAGE_RENDER_TARGET		= 0x00000100
};

typedef struct CONSTANT_BUFFER_DESC {
	uint						ByteSize;
	ECONSTANT_BUFFER_BIND_FLAG	BindFlag;
} CONSTANT_BUFFER_DESC;

typedef struct TEXTURE_DESC {
	uint			Width;
	uint			Height;
	uint			Depth;
	uint			ArraySize;
	uint			MipCount;
	DXGI_FORMAT		Format;
	ETEXTURE_USAGE	Usage;
} TEXTURE_DESC;


typedef struct DEPTH_STENCIL_DESC {
	uint		Width;
	uint		Height;
	DXGI_FORMAT Format;
} DEPTH_STENCIL_DESC;

typedef struct SUBRESOURCE_DATA {
	void* Pointer;
	uint Depth;
	uint Width;
	uint Subresource;
} SUBRESOURCE_DATA;


ForceInline bool operator == (const CONSTANT_BUFFER_DESC& l, const CONSTANT_BUFFER_DESC& r)
{
	return (l.ByteSize == r.ByteSize && l.BindFlag == r.BindFlag);
}

ForceInline bool operator != (const CONSTANT_BUFFER_DESC& l, const CONSTANT_BUFFER_DESC& r)
{
	return !(l == r);
}

ForceInline bool operator == (const TEXTURE_DESC& l, const TEXTURE_DESC& r)
{
	return (
		l.Width		== r.Width && 
		l.Height	== r.Height && 
		l.Depth		== r.Depth && 
		l.ArraySize == r.ArraySize && 
		l.MipCount	== r.MipCount && 
		l.Format	== r.Format && 
		l.Usage		== r.Usage
		);
}

ForceInline bool operator != (const TEXTURE_DESC& l, const TEXTURE_DESC& r)
{
	return !(l == r);
}

ForceInline bool IsTextureDesc2D(const TEXTURE_DESC& Desc)
{
	return Desc.Depth == 0;
}

ForceInline bool IsTextureDesc3D(const TEXTURE_DESC& Desc)
{
	return Desc.Depth > 0;
}

ForceInline D3D12_RESOURCE_DESC TranslateConstantBufferDesc(const CONSTANT_BUFFER_DESC& cbDesc)
{
	return {
		D3D12_RESOURCE_DIMENSION_BUFFER,
		256,
		(cbDesc.ByteSize & ~255),
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		{ 1, 1 },
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE
	};
}

ForceInline D3D12_RESOURCE_DESC TranslateTextureDesc(const TEXTURE_DESC& Desc)
{
	D3D12_RESOURCE_DIMENSION ResourceDimension = (D3D12_RESOURCE_DIMENSION)0;
	uint DepthAndArraySize = 1;

	if (Desc.ArraySize > 0 && Desc.Depth > 0)
	{
		// Log error here
	}
	else if (Desc.ArraySize > 0 && Desc.Depth == 0)
	{
		ResourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		DepthAndArraySize = Desc.ArraySize;
	}
	else if (Desc.ArraySize == 0 && Desc.Depth > 0)
	{
		ResourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
		DepthAndArraySize = Desc.Depth;
	}

	D3D12_RESOURCE_FLAGS ResourceFlags = (D3D12_RESOURCE_FLAGS)0;

	if (Desc.Usage & TEXTURE_USAGE_SHADER_RESOURCE)
	{
		ResourceFlags |= D3D12_RESOURCE_FLAG_NONE;
	}
	if (Desc.Usage & TEXTURE_USAGE_RENDER_TARGET)
	{
		ResourceFlags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	}
	if (Desc.Usage & TEXTURE_USAGE_UNORDERED_ACCESS)
	{
		ResourceFlags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	return {
		ResourceDimension,
		0,
		Desc.Width,
		Desc.Height,
		DepthAndArraySize,
		Desc.MipCount,
		Desc.Format,
		{ 1, 1 },
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		ResourceFlags
	};
}


ForceInline D3D12_RESOURCE_DESC D3D12CreateVertexBufferDesc(uint ByteWidth)
{
	return {
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		ByteWidth,
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		{ 1, 1 },
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE
	};
}

ForceInline D3D12_RESOURCE_DESC D3D12CreateIndexBufferDesc(uint ByteWidth)
{
	return {
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		ByteWidth,
		1,
		1,
		1,
		DXGI_FORMAT_R32_UINT,
		{ 1, 1 },
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE
	};
}
