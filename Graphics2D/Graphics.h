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
	}
	Graphics( const Graphics& ) = delete;
	Graphics& operator = ( const Graphics& ) = delete;
	~Graphics() = default;

public:
	void BeginFrame()
	{
		const float RED		= 1.0f;
		const float GREEN	= 1.0f;
		const float BLUE	= 1.0f;
		const float COLOR[] = { RED, GREEN, BLUE, 1.0f };

		pContext->ClearRenderTargetView( 
			pRenderTargetView.Get(),
			COLOR );
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
	void DrawCube( float angle )
	{
	//////////////////////////////////////////////////////////////
	//                                                          //
		struct Vertex
		{
			float x;
			float y;
			float z;
		};		

		Vertex v0 = { -1.0f,+1.0f,-1.0f }; // top    left  front
		Vertex v1 = { +1.0f,+1.0f,-1.0f }; // top    right front
		Vertex v2 = { +1.0f,-1.0f,-1.0f }; // bottom right front
		Vertex v3 = { -1.0f,-1.0f,-1.0f }; // bottom left  front
		Vertex v4 = { -1.0f,+1.0f,+1.0f }; // top    left  back
		Vertex v5 = { +1.0f,+1.0f,+1.0f }; // top    right back
		Vertex v6 = { +1.0f,-1.0f,+1.0f }; // bottom right back
		Vertex v7 = { -1.0f,-1.0f,+1.0f }; // bottom left  back

		const Vertex VERTICES[] = {	v0,v1,v2,v3,v4,v5,v6,v7	};

		const unsigned short INDICES[] = {
			0,1,2,	0,2,3, // front
			0,3,7,	0,7,4, // left
			5,4,7,	5,7,6, // back
			1,5,6,	1,6,2, // right
			4,5,1,	4,1,0, // top
			3,2,6,	3,6,7  // bottom
			};

	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //

		struct ConstantBuffer
		{
			DirectX::XMMATRIX transform;
		};
			
		const ConstantBuffer TRANSFORMATION_MATRIX = {
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationX( angle * 2.0f ) *
				DirectX::XMMatrixRotationY( angle ) *
				DirectX::XMMatrixRotationZ( angle )	* 
				DirectX::XMMatrixScaling( 1.0f, 1.0f, 1.0f ) * 
				DirectX::XMMatrixTranslation( 0.0f, 0.0f, 4.0f) *
				DirectX::XMMatrixPerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 10.0f ) ) };

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} colors[6];
		};

		const ConstantBuffer2 COLORS =
		{
			{
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 0.0f},
				{1.0f, 0.0f, 1.0f},
				{0.0f, 1.0f, 1.0f}
			}
		};
		
	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //

		const UINT VS_STRIDE = sizeof( Vertex );
		const UINT VS_OFFSET = 0u;

		// interface pointers
		Microsoft::WRL::ComPtr<ID3DBlob>			pBlobVS;
		Microsoft::WRL::ComPtr<ID3DBlob>			pBlobPS;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		pConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		pConstantBuffer2;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	pVertexShader;

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

		// create and set vertex subresourse data
		D3D11_SUBRESOURCE_DATA vertex_subresource_data = {};
		vertex_subresource_data.pSysMem				= VERTICES;
		vertex_subresource_data.SysMemPitch			= 0u;
		vertex_subresource_data.SysMemSlicePitch	= 0u;

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

		// create and set index subresourse data
		D3D11_SUBRESOURCE_DATA index_subresource_data = {};
		index_subresource_data.pSysMem			= INDICES;
		index_subresource_data.SysMemPitch		= 0u;
		index_subresource_data.SysMemSlicePitch = 0u;

	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //

		// create and set input element descriptor
		D3D11_INPUT_ELEMENT_DESC input_element_desc = {};
		input_element_desc.AlignedByteOffset		= 0u;
		input_element_desc.Format					= DXGI_FORMAT_R32G32B32_FLOAT;
		input_element_desc.InputSlot				= 0u;
		input_element_desc.InputSlotClass			= D3D11_INPUT_PER_VERTEX_DATA;
		input_element_desc.InstanceDataStepRate		= 0u;
		input_element_desc.SemanticIndex			= 0u;
		input_element_desc.SemanticName				= "POSITION";

		// create and set input element descriptor array
		const D3D11_INPUT_ELEMENT_DESC INPUT_ELEMENT_DESCS[] = { 
			input_element_desc };

	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //

		D3D11_BUFFER_DESC constant_buffer_desc = {};
		constant_buffer_desc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
		constant_buffer_desc.ByteWidth				= sizeof( TRANSFORMATION_MATRIX );
		constant_buffer_desc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
		constant_buffer_desc.MiscFlags				= 0u;
		constant_buffer_desc.StructureByteStride	= 0u;
		constant_buffer_desc.Usage					= D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA constant_buffer_subresource_data = {};
		constant_buffer_subresource_data.pSysMem = &TRANSFORMATION_MATRIX;

	//                                                           //
	///////////////////////////////////////////////////////////////
	//                                                           //

		D3D11_BUFFER_DESC constant_buffer_2_desc = {};
		constant_buffer_2_desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		constant_buffer_2_desc.ByteWidth			= sizeof( COLORS );
		constant_buffer_2_desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		constant_buffer_2_desc.MiscFlags			= 0u;
		constant_buffer_2_desc.StructureByteStride	= 0u;
		constant_buffer_2_desc.Usage				= D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA constant_buffer_2_subresource_data = {};
		constant_buffer_2_subresource_data.pSysMem = &COLORS;

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
		//                     SET DEVICE                    //
		//                                                   //
		///////////////////////////////////////////////////////

		pDevice->CreateBuffer( 
			&vertex_buffer_desc,
			&vertex_subresource_data,
			&pVertexBuffer );

		pDevice->CreateBuffer(
			&index_buffer_desc,
			&index_subresource_data,
			&pIndexBuffer );

		pDevice->CreateBuffer(
			&constant_buffer_desc,
			&constant_buffer_subresource_data,
			&pConstantBuffer );

		pDevice->CreateBuffer(
			&constant_buffer_2_desc,
			&constant_buffer_2_subresource_data,
			&pConstantBuffer2 );
				
		pDevice->CreateVertexShader( 
			pBlobVS->GetBufferPointer(),
			pBlobVS->GetBufferSize(),
			nullptr,
			&pVertexShader );

		pDevice->CreatePixelShader(
			pBlobPS->GetBufferPointer(),
			pBlobPS->GetBufferSize(),
			nullptr,
			&pPixelShader );		

		pDevice->CreateInputLayout(	
			INPUT_ELEMENT_DESCS,
			static_cast<UINT>(std::size( INPUT_ELEMENT_DESCS )),
			pBlobVS->GetBufferPointer(),
			pBlobVS->GetBufferSize(),
			&pInputLayout );

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

		pContext->VSSetConstantBuffers( 0u, 1u, pConstantBuffer.GetAddressOf() );

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

		pContext->PSSetConstantBuffers( 0u, 1u, pConstantBuffer2.GetAddressOf() );

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
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pContext;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;

private:
	static constexpr UINT SCREEN_W = 800u;
	static constexpr UINT SCREEN_H = 600u;
};