
#pragma once

#if defined( DXVK )

#include "../dxvk-native-1.9.2b/include/native/directx/d3d9.h"
#include "togl/dxabstract.h"
#include "togl/dxabstract_types.h"

typedef void* VD3DHWND;
typedef void* VD3DHANDLE;

inline void toglGetClientRect( void *hWnd, RECT *destRect )
{
	// the only useful answer this call can offer, is the size of the canvas.
	// actually getting the window bounds is not useful.
	// so, see if a D3D device is up and running, and if so,
	// dig in and find out its backbuffer size and use that.

	uint width, height;	
#ifdef USE_SDL
    SDL_GetWindowSize( hWnd, &width, &height );
#else
#error "Implete ME!"
#endif

	Assert( width!=0 && height!=0 );

	destRect->left = 0;
	destRect->top = 0;
	destRect->right = width;
	destRect->bottom = height;		
}

#else
	//USE_ACTUAL_DX
	#ifdef WIN32
		#ifdef _X360
			#include "d3d9.h"
			#include "d3dx9.h"
		#else
			#include <windows.h>
			#include "../../dx9sdk/include/d3d9.h"
			#include "../../dx9sdk/include/d3dx9.h"
		#endif
	#endif

typedef HWND VD3DHWND;

#endif // DXVK

#define	GLMPRINTF(args)	
#define	GLMPRINTSTR(args)
#define	GLMPRINTTEXT(args)
