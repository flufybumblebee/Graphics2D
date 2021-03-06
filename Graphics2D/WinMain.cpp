#include <Windows.h>
#include <chrono>
#include <random>
#include <vector>

#include "Graphics.h"

// user defined windows procedure
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage( 69 );
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int WINAPI wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow)
{
	const auto CLASS_NAME	= L"Class Name";
	const auto WINDOW_NAME	= L"Window Name";
	const auto WINDOW_STYLE = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	
	// create window class
	WNDCLASS window_class;
	
	// set window class
	window_class.cbClsExtra		= 0;
	window_class.cbWndExtra		= 0;
	window_class.hbrBackground	= 0;
	window_class.hCursor		= 0;
	window_class.hIcon			= 0;
	window_class.hInstance		= hInstance;
	window_class.lpfnWndProc	= WindowProc;
	window_class.lpszClassName	= CLASS_NAME;
	window_class.lpszMenuName	= nullptr;
	window_class.style			= 0u;

	// register windows class
	RegisterClass( &window_class );

	// create client region rectangle
	RECT client_region;

	// set client region rectangle
	client_region.left		= 100L;
	client_region.right		= 900L;
	client_region.top		= 100L;
	client_region.bottom	= 700L;	

	// adjust window to accomodate client region rectangle and window style
	AdjustWindowRect( &client_region, WINDOW_STYLE, FALSE );

	// create window
	HWND hWnd = CreateWindow( 
		CLASS_NAME,
		WINDOW_NAME,
		WINDOW_STYLE,
		client_region.left,
		client_region.top,
		client_region.right - client_region.left,
		client_region.bottom - client_region.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr );

	// show window
	ShowWindow( hWnd, SW_SHOWDEFAULT );

	// create graphics object
	Graphics gfx( hWnd );

	std::chrono::steady_clock::time_point start;

	std::random_device rd;
	std::mt19937_64 mt( rd() );
	std::uniform_real_distribution<float> random_float_xy( -5.0f, 5.0f );
	std::uniform_real_distribution<float> random_float_z( 0.0f, 6.0f );
	std::uniform_real_distribution<float> random_float_angle( 0.0f, 1.0f );
	
	struct Vertex
	{
		float x;
		float y;
		float z;
	};

	struct Angle
	{
		float x;
		float y;
		float z;
	};

	std::vector<Vertex> vertices;
	std::vector<Angle> angles;

	const UINT NUM = 100;

	for (int i = 0; i < NUM; i++)
	{
		Vertex v = { 
			random_float_xy( mt ),
			random_float_xy( mt ),
			random_float_z( mt ) };
		vertices.emplace_back( v );
		Angle a = {
			random_float_angle( mt ),
			random_float_angle( mt ),
			random_float_angle( mt ) };
		angles.emplace_back( a );
	}

	while (true)
	{
		// message loop
		MSG msg;
		while ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			if (msg.message == WM_QUIT)
			{
				return static_cast<int>(msg.wParam);
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// set angle using time since start
		float angle = std::chrono::duration<float>( std::chrono::steady_clock::now() - start ).count();

		//float x = sin( angle );
		// graphics function calls
		gfx.BeginFrame();
		/*gfx.DrawCube( x,x,x, 0.0f, 0.0f, 0.5f );
		gfx.DrawCube( angle,angle,angle, x, x, x );*/
	
		for (int i = 0; i < NUM; i++)
		{			
			gfx.DrawCube( 
				vertices[i].x,
				vertices[i].y,
				vertices[i].z,
				angles[i].x + angle / 2.0f,
				angles[i].y + angle / 2.0f,
				angles[i].z + angle / 2.0f );
		}

		gfx.EndFrame();
	}

	return 0;
}