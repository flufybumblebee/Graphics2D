#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <iterator>
#include <DirectXMath.h>

#pragma comment( lib,"d3d11.lib" )
#pragma comment( lib,"d3dcompiler.lib")

class Graphics
{
public:
	Graphics( HWND& hWnd )
	{
		// create swap chain descriptor
		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};

		// set swap chain descriptor
		SwapChainDesc.BufferCount							= 1u;
		SwapChainDesc.BufferDesc.Width						= Graphics::SCREEN_W;
		SwapChainDesc.BufferDesc.Height						= Graphics::SCREEN_H;
		SwapChainDesc.BufferDesc.Format						= DXGI_FORMAT_B8G8R8A8_UNORM;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator		= 1u;
		SwapChainDesc.BufferDesc.RefreshRate.Denominator	= 60u;
		SwapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.OutputWindow							= hWnd;
		SwapChainDesc.SampleDesc.Count						= 1u;
		SwapChainDesc.SampleDesc.Quality					= 0u;
		SwapChainDesc.Windowed								= TRUE;

		UINT swapCreateFlags = 0;
#ifdef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// create device and swap chain
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0u,
			D3D11_SDK_VERSION,
			&SwapChainDesc,
			&pSwapChain,
			&pDevice,
			nullptr,
			&pContext );

		// create handle to backbuffer
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

		// stuff
		pSwapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(LPVOID*)&pBackBuffer
			);

		// create a view on backbuffer that we can render to
		pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pRenderTargetView
			);
		//
		//// set backbuffer as the render target using created view
		//pContext->OMSetRenderTargets( 1, pRenderTargetView.GetAddressOf(), nullptr );
		//
		//// create viewport and set dimensions
		//D3D11_VIEWPORT viewport;
		//viewport.Width	= float( Graphics::SCREEN_W );
		//viewport.Height	= float( Graphics::SCREEN_H );
		//viewport.MinDepth = 0.0f;
		//viewport.MaxDepth = 1.0f;
		//viewport.TopLeftX = 0.0f;
		//viewport.TopLeftY = 0.0f;
		//
		//pContext->RSSetViewports( 1, &viewport );
		//
		//// create texture for cpu render target
		//D3D11_TEXTURE2D_DESC sysTexDesc;
		//sysTexDesc.Width				= Graphics::SCREEN_W;
		//sysTexDesc.Height				= Graphics::SCREEN_H;
		//sysTexDesc.MipLevels			= 1u;
		//sysTexDesc.ArraySize			= 1u;
		//sysTexDesc.Format				= DXGI_FORMAT_B8G8R8A8_UNORM;
		//sysTexDesc.SampleDesc.Count		= 1u;
		//sysTexDesc.SampleDesc.Quality	= 0u;
		//sysTexDesc.Usage				= D3D11_USAGE_DYNAMIC;
		//sysTexDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		//sysTexDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		//sysTexDesc.MiscFlags			= 0u;
		//
		//// create the texture
		//hr = pDevice->CreateTexture2D(
		//	&sysTexDesc,
		//	nullptr,
		//	&pSysBufferTexture
		//	);
		//
		//// shader
		//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//srvDesc.Format				= sysTexDesc.Format;
		//srvDesc.ViewDimension		= D3D11_SRV_DIMENSION_TEXTURE2D;
		//srvDesc.Texture2D.MipLevels = 1u;
		//
		//// create the resource view on the texture
		//hr = pDevice->CreateShaderResourceView(
		//	pSysBufferTexture.Get(),
		//	&srvDesc,
		//	&pSysBufferTextureView
		//	);
		//
		//// create pixel shader for framebuffer
		//hr = pDevice->CreatePixelShader(
		//	FramebufferShaders::FramebufferPSBytecode,
		//	sizeof( FramebufferShaders::FramebufferPSBytecode ),
		//	nullptr,
		//	&pPixelShader );
		//
		//// create vertex shader for framebuffer
		//hr = pDevice->CreateVertexShader(
		//	FramebufferShaders::FramebufferVSBytecode,
		//	sizeof( FramebufferShaders::FramebufferVSBytecode ),
		//	nullptr,
		//	&pVertexShader );
		//
		//
		//D3D11_BUFFER_DESC buffer_desc = {};
		//buffer_desc.Usage			= D3D11_USAGE_DEFAULT;
		//buffer_desc.ByteWidth		= sizeof( FSQVertex ) * 6;
		//buffer_desc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		//buffer_desc.CPUAccessFlags	= 0u;
		//
		//D3D11_SUBRESOURCE_DATA initData = {};
		//initData.pSysMem = vertices;
		//
		//hr = pDevice->CreateBuffer( 
		//	&buffer_desc,
		//	&initData,
		//	&pVertexBuffer
		//	);
		//		
		//// create input layout for fullscreen quad
		//const D3D11_INPUT_ELEMENT_DESC ied[] =
		//{
		//	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		//	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
		//};
		//
		//// Ignore the intellisense error "namespace has no member"
		//hr = pDevice->CreateInputLayout( 
		//	ied, 
		//	2u,
		//	FramebufferShaders::FramebufferVSBytecode,
		//	sizeof( FramebufferShaders::FramebufferVSBytecode ),
		//	&pInputLayout
		//	);
		//
		//// Create sampler state for fullscreen textured quad
		//D3D11_SAMPLER_DESC sampDesc = {};
		//sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_POINT;
		//sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;
		//sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
		//sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
		//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//sampDesc.MinLOD			= 0;
		//sampDesc.MaxLOD			= D3D11_FLOAT32_MAX;
		//
		//hr = pDevice->CreateSamplerState( &sampDesc, &pSamplerState );
	}
	Graphics( const Graphics& ) = delete;
	Graphics& operator = ( const Graphics& ) = delete;
	~Graphics() = default;

public:
	void BeginFrame()
	{
		const float RED		= 1.0f;
		const float GREEN	= 0.0f;
		const float BLUE	= 0.0f;
		const float COLOR[] = { RED, GREEN, BLUE, 1.0f };

		pContext->ClearRenderTargetView( 
			pRenderTargetView.Get(),
			COLOR
			);
	}
	void EndFrame()
	{
		// flip back/front buffers
		pSwapChain->Present( 1u, 0u );
	}
public:
	void PutPixel( int x, int y, int r, int g, int b )
	{

	}
	void DrawTriangle()
	{
	//////////////////////////////////////////////////////////////
	//                                                          //
		struct Vertex
		{
			float x;
			float y;

			float r;
			float g;
			float b;
			float a;
		};

		const Vertex VERTICES[] = {
			{ +0.0f, +0.5f, 1.0f,0.0f,0.0f,1.0f },
			{ +0.5f, -0.5f, 0.0f,1.0f,0.0f,1.0f },
			{ -0.5f, -0.5f, 0.0f,0.0f,1.0f,1.0f } };

		const unsigned short INDICES[] = {
			0,1,2 };
	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		const UINT VS_STRIDE = sizeof( Vertex );
		const UINT VS_OFFSET = 0u;

		// interface pointers
		Microsoft::WRL::ComPtr<ID3DBlob>			pBlobVS;
		Microsoft::WRL::ComPtr<ID3DBlob>			pBlobPS;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	pInputLayout;

	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		// create and set vertex buffer descriptor
		D3D11_BUFFER_DESC vertex_buffer_desc = {};
		vertex_buffer_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.ByteWidth			= sizeof( VERTICES );
		vertex_buffer_desc.CPUAccessFlags		= 0u;
		vertex_buffer_desc.MiscFlags			= 0u;
		vertex_buffer_desc.StructureByteStride	= sizeof( Vertex );
		vertex_buffer_desc.Usage				= D3D11_USAGE_DEFAULT;

		// create and set vertex subresourse descriptor
		D3D11_SUBRESOURCE_DATA vertex_subresource_desc = {};
		vertex_subresource_desc.pSysMem				= VERTICES;
		vertex_subresource_desc.SysMemPitch			= 0u;
		vertex_subresource_desc.SysMemSlicePitch	= 0u;
	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		// create and set index buffer descriptor
		D3D11_BUFFER_DESC index_buffer_desc = {};
		index_buffer_desc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.ByteWidth				= sizeof( INDICES );
		index_buffer_desc.CPUAccessFlags		= 0u;
		index_buffer_desc.MiscFlags				= 0u;
		index_buffer_desc.StructureByteStride	= sizeof( unsigned short );
		index_buffer_desc.Usage					= D3D11_USAGE_DEFAULT;

		// create and set index subresourse descriptor
		D3D11_SUBRESOURCE_DATA index_subresource_desc = {};
		index_subresource_desc.pSysMem			= INDICES;
		index_subresource_desc.SysMemPitch		= 0u;
		index_subresource_desc.SysMemSlicePitch = 0u;
	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		// create and set input element descriptor
		D3D11_INPUT_ELEMENT_DESC input_element_desc_0 = {};
		input_element_desc_0.AlignedByteOffset		= 0u;
		input_element_desc_0.Format					= DXGI_FORMAT_R32G32_FLOAT;
		input_element_desc_0.InputSlot				= 0u;
		input_element_desc_0.InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
		input_element_desc_0.InstanceDataStepRate	= 0u;
		input_element_desc_0.SemanticIndex			= 0u;
		input_element_desc_0.SemanticName			= "POSITION";

		// create and set input element descriptor
		D3D11_INPUT_ELEMENT_DESC input_element_desc_1 = {};
		input_element_desc_1.AlignedByteOffset		= 8u;
		input_element_desc_1.Format					= DXGI_FORMAT_R32G32B32A32_FLOAT;
		input_element_desc_1.InputSlot				= 0u;
		input_element_desc_1.InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
		input_element_desc_1.InstanceDataStepRate	= 0u;
		input_element_desc_1.SemanticIndex			= 0u;
		input_element_desc_1.SemanticName			= "COLOR";

		// create and set input element descriptor array
		const D3D11_INPUT_ELEMENT_DESC INPUT_ELEMENT_DESCS[] = { 
			input_element_desc_0,
			input_element_desc_1 };
	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		// create and set viewport
		D3D11_VIEWPORT viewport = {};
		viewport.Width		= 800.0f;
		viewport.Height		= 600.0f;
		viewport.MinDepth	= 0.0f;
		viewport.MaxDepth	= 1.0f;
		viewport.TopLeftX	= 0.0f;
		viewport.TopLeftY	= 0.0f;
	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		// copy data from shader files to blobs
		D3DReadFileToBlob( L"VertexShader.cso", &pBlobVS );
		D3DReadFileToBlob( L"PixelShader.cso", &pBlobPS );
	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		///////////////////////////////////////////////////////
		//                                                   //
		//                   SET UP DEVICE                   //
		//                                                   //
		///////////////////////////////////////////////////////

		pDevice->CreateBuffer( &vertex_buffer_desc, &vertex_subresource_desc, &pVertexBuffer );

		pDevice->CreateBuffer( &index_buffer_desc, &index_subresource_desc, &pIndexBuffer );
				
		pDevice->CreateVertexShader( pBlobVS->GetBufferPointer(),
			pBlobVS->GetBufferSize(), nullptr, &pVertexShader );

		pDevice->CreatePixelShader( pBlobPS->GetBufferPointer(),
			pBlobPS->GetBufferSize(), nullptr, &pPixelShader );		

		pDevice->CreateInputLayout(	
			INPUT_ELEMENT_DESCS,
			static_cast<UINT>(std::size( INPUT_ELEMENT_DESCS )),
			pBlobVS->GetBufferPointer(),
			pBlobVS->GetBufferSize(),
			&pInputLayout
			);

	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //
		///////////////////////////////////////////////////////
		//                                                   //
		//               INPUT ASSEMBLER STAGE               //
		//                                                   //
		///////////////////////////////////////////////////////

		pContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &VS_STRIDE, &VS_OFFSET );

		pContext->IASetIndexBuffer( pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u );

		///////////////////////////////////////////////////////
		//                                                   //
		//               VERTEX SHADER STAGE                 //
		//                                                   //
		///////////////////////////////////////////////////////			
		
		pContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );		

		///////////////////////////////////////////////////////
		//                                                   //
		//               INPUT ASSEMBLER STAGE               //
		//                                                   //
		///////////////////////////////////////////////////////

		pContext->IASetInputLayout( pInputLayout.Get() );		

		pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		///////////////////////////////////////////////////////
		//                                                   //
		//                 RASTERIZER STAGE                  //
		//                                                   //
		///////////////////////////////////////////////////////

		pContext->RSSetViewports( 1u, &viewport );

		///////////////////////////////////////////////////////
		//                                                   //
		//                PIXEL SHADER STAGE                 //
		//                                                   //
		///////////////////////////////////////////////////////
		
		pContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );

		///////////////////////////////////////////////////////
		//                                                   //
		//               OUTPUT MERGER STAGE                 //
		//                                                   //
		///////////////////////////////////////////////////////

		pContext->OMSetRenderTargets( 1u, pRenderTargetView.GetAddressOf(), nullptr );

		///////////////////////////////////////////////////////
		//                                                   //
		//                        DRAW                       //
		//                                                   //
		///////////////////////////////////////////////////////

		pContext->DrawIndexed( static_cast<UINT>(std::size( INDICES )), 0u, 0u );

	//                                                           //
	///////////////////////////////////////////////////////////////
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	//Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	//Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	//Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	//Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	//Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	//D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;

private:
	//// vertex format for the framebuffer fullscreen textured quad
	//struct FSQVertex
	//{
	//	float x, y, z;		// position
	//	float u, v;			// texcoords
	//};
private:
	static constexpr UINT SCREEN_W = 800u;
	static constexpr UINT SCREEN_H = 600u;
};