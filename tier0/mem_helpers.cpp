//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "pch_tier0.h"
#include "mem_helpers.h"
#include <string.h>
#ifdef OSX
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

bool g_bInitMemory = true;

#ifdef POSIX
void DoApplyMemoryInitializations( void *pMem, int nSize )
{
}

size_t CalcHeapUsed()
{
	return 0;
}

#else

unsigned long g_dwFeeFee = 0xffeeffee;

// Generated by Mathematica.
unsigned char g_RandomValues[256] = {
	95, 126, 220, 71, 92, 179, 95, 219, 111, 150, 38, 155, 181, 62, 40, 231, 238,
	54, 47, 55, 186, 204, 64, 70, 118, 94, 107, 251, 199, 140, 67, 87, 86, 127,
	210, 41, 21, 90, 208, 24, 167, 204, 32, 254, 38, 51, 9, 11, 38, 33, 188, 104,
	0, 75, 119, 24, 122, 203, 24, 164, 250, 224, 241, 182, 213, 201, 173, 67,
	200, 255, 244, 227, 46, 219, 26, 149, 218, 132, 120, 154, 227, 244, 106, 198,
	109, 87, 150, 40, 16, 99, 169, 193, 100, 156, 78, 171, 246, 47, 84, 119, 10,
	52, 207, 171, 230, 90, 90, 127, 180, 153, 68, 140, 62, 14, 87, 57, 208, 154,
	116, 29, 131, 177, 224, 187, 51, 148, 142, 245, 152, 230, 184, 117, 91, 146,
	235, 153, 35, 104, 187, 177, 215, 131, 17, 49, 211, 244, 60, 152, 103, 248,
	51, 224, 237, 240, 51, 30, 10, 233, 253, 106, 252, 73, 134, 136, 178, 86,
	228, 107, 77, 255, 85, 242, 204, 119, 102, 53, 209, 35, 123, 32, 252, 210,
	43, 12, 136, 167, 155, 210, 71, 254, 178, 172, 3, 230, 93, 208, 196, 68, 235,
	16, 106, 189, 201, 177, 85, 78, 206, 187, 48, 68, 64, 190, 117, 236, 49, 174,
	105, 63, 207, 70, 170, 93, 6, 110, 52, 111, 169, 92, 247, 86, 10, 174, 207,
	240, 104, 209, 81, 177, 123, 189, 175, 212, 101, 219, 114, 243, 44, 91, 51,
	139, 91, 57, 120, 41, 98, 119 };

unsigned long g_iCurRandomValueOffset = 0;


void InitializeToFeeFee( void *pMem, int nSize )
{
	unsigned long *pCurDWord = (unsigned long*)pMem;
	int nDWords = nSize >> 2;
	while ( nDWords )
	{
		*pCurDWord = 0xffeeffee;
		++pCurDWord;
		--nDWords;
	}
	
	unsigned char *pCurChar = (unsigned char*)pCurDWord;
	int nBytes = nSize & 3;
	int iOffset = 0;
	while ( nBytes )
	{
		*pCurChar = ((unsigned char*)&g_dwFeeFee)[iOffset];
		++iOffset;
		--nBytes;
		++pCurChar;
	}			
}


void InitializeToRandom( void *pMem, int nSize )
{
	unsigned char *pOut = (unsigned char *)pMem;
	for ( int i=0; i < nSize; i++ )
	{
		pOut[i] = g_RandomValues[(g_iCurRandomValueOffset & 255)];
		++g_iCurRandomValueOffset;
	}
}


void DoApplyMemoryInitializations( void *pMem, int nSize )
{
	if ( !pMem )
		return;
	
	// If they passed -noinitmemory on the command line, don't do anything here.
	Assert( g_bInitMemory );

	// First time we get in here, remember all the settings.
	static bool bDebuggerPresent = Plat_IsInDebugSession();
	static bool bCheckedCommandLine = false;
	static bool bRandomizeMemory = false;
	if ( !bCheckedCommandLine )
	{
		bCheckedCommandLine = true;
		
		//APS
		char *pStr = (char*)Plat_GetCommandLineA();
		if ( pStr )
		{
			char tempStr[512];
			strncpy( tempStr, pStr, sizeof( tempStr ) - 1 );
			tempStr[ sizeof( tempStr ) - 1 ] = 0;
			_strupr( tempStr );
			
			if ( strstr( tempStr, "-RANDOMIZEMEMORY" ) )
				bRandomizeMemory = true;
			
			if ( strstr( tempStr, "-NOINITMEMORY" ) )
				g_bInitMemory = false;
		}
	}

	if ( bRandomizeMemory )
	{
		// They asked for it.. randomize all the memory.
		InitializeToRandom( pMem, nSize );
	}
	else
	{
		if ( bDebuggerPresent )
		{
			// Ok, it's already set to 0xbaadf00d, but we want something that will make floating-point #'s NANs.
			InitializeToFeeFee( pMem, nSize );
		}
		else
		{
#ifdef _DEBUG
			// Ok, it's already set to 0xcdcdcdcd, but we want something that will make floating-point #'s NANs.
			InitializeToFeeFee( pMem, nSize );
#endif
		}
	}
}

size_t CalcHeapUsed()
{
#if defined( _X360 )
	return 0;
#else
	_HEAPINFO	hinfo;
	int			heapstatus;
	intp			nTotal;

	nTotal = 0;
	hinfo._pentry = NULL;
	while( ( heapstatus = _heapwalk( &hinfo ) ) == _HEAPOK )
	{
		nTotal += (hinfo._useflag == _USEDENTRY) ? hinfo._size : 0;
	}

	switch (heapstatus)
	{
		case _HEAPEMPTY:
		case _HEAPEND:
			// success
			break;

		default:
			// heap corrupted
			nTotal = -1;
	}

	return nTotal;
#endif
}
#endif

