#ifndef __CBUFFERMAPPER_H_
#define __CBUFFERMAPPER_H_

#include "Graphics.h"

/// <summary>
/// Defines the type of shader the constant buffer is assigned to.
/// </summary>
enum ShaderType
{
	VertexShader,
	PixelShader
};

/// <summary>
/// Manages the mapping and unmapping of CBuffers for shader programs.
/// </summary>
/// <typeparam name="T">Passed in types must follow the 16 byte rule!</typeparam>
template <typename T>
class CBufferMapper
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer = nullptr;
	unsigned int m_uRegisterIndex;

	/// <summary>
	/// Initializes the Constant Buffer pointer for later use.
	/// </summary>
	void InitBuffer(ShaderType a_uTargetShader)
	{
		// Calculating the memory size in multiples of 
		// 16 by taking advantage of int division.
		unsigned int cBufferSize = sizeof(T);
		cBufferSize = ((cBufferSize + 15) / 16) * 16;

		// Creating the description of the CBuffer.
		D3D11_BUFFER_DESC cbDesc{};
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.ByteWidth = cBufferSize;
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		// Creating the buffer with the description struct.
		Graphics::GetDevice()->CreateBuffer(&cbDesc, 0, m_pConstantBuffer.GetAddressOf());

		// Creating the correct type of constant buffer.
		switch (a_uTargetShader)
		{
		case ShaderType::PixelShader:

			// Binding the buffer to the correct register.
			Graphics::GetContext()->PSSetConstantBuffers(
				m_uRegisterIndex,
				1,
				m_pConstantBuffer.GetAddressOf());

			break;

		case ShaderType::VertexShader:

			// Binding the buffer to the b0 slot for use.
			Graphics::GetContext()->VSSetConstantBuffers(
				m_uRegisterIndex,
				1,
				m_pConstantBuffer.GetAddressOf());

			break;
		}
	}

public:
	/// <summary>
	/// Constructs and initializes the CBufferMapper.
	/// </summary>
	CBufferMapper(unsigned int a_uRegisterIndex, ShaderType a_uTargetShader = ShaderType::VertexShader)
	{
		m_uRegisterIndex = a_uRegisterIndex;
		InitBuffer(a_uTargetShader);
	}

	/// <summary>
	/// Destructs instances of the CBufferMapper.
	/// </summary>
	~CBufferMapper()
	{
		m_pConstantBuffer.Reset();
	}

	/// <summary>
	/// Copy operator overload for the CBufferMapper.
	/// </summary>
	CBufferMapper& operator =(const CBufferMapper& other)
	{
		if (m_pConstantBuffer != nullptr)
		{
			m_pConstantBuffer.Reset();
		}

		m_uRegisterIndex = other.m_uRegisterIndex;
		m_pConstantBuffer = other.m_pConstantBuffer;

		return *this;
	}

	/// <summary>
	/// Copy constructor overload for the CBufferMapper.
	/// </summary>
	CBufferMapper(const CBufferMapper& other)
	{
		m_uRegisterIndex = other.m_uRegisterIndex;
		m_pConstantBuffer = other.m_pConstantBuffer;
	}

	/// <summary>
	/// Pulls out the memory address of the CBuffer
	/// and maps the passed in data to the buffer.
	/// </summary>
	/// <param name="a_CBufferData">The data being passed to the CBuffer.</param>
	void MapBufferData(T a_CBufferData)
	{
		// Creating a mapped subresource struct to hold the cbuffer GPU address.
		D3D11_MAPPED_SUBRESOURCE mapped{};

		// Actually grabbing the CBuffer's address.
		Graphics::GetContext()->Map(
			m_pConstantBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mapped
		);

		// Copying the data to the GPU.
		memcpy(mapped.pData, &a_CBufferData, sizeof(T));

		// Unmapping from the memory address.
		Graphics::GetContext()->Unmap(m_pConstantBuffer.Get(), 0);
	}
};

#endif //__CBUFFERMAPPER_H_