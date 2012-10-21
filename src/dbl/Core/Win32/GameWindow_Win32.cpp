/* This source file is part of the Delectable Engine.
 * For the latest info, please visit http://delectable.googlecode.com/
 *
 * Copyright (c) 2009-2012 Ryan Chew
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file GameWindow_Win32.cpp
 * @brief 32-bit Windows specific implementations.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Input/MouseManager.h"
#include "dbl/Core/GameWindow.h"

// External Dependencies //
#include <windows.h>

using namespace dbl;

void GameWindow::PopulateResolutions( void )
{
	mAvailableResolutions.clear();

	DWORD count = 0;
	DEVMODE devmode;
	memset( &devmode, 0, sizeof( devmode ) );
	DISPLAY_DEVICE device;
	memset( &device, 0, sizeof( DISPLAY_DEVICE ) );
	device.cb = sizeof( device );

	// Get the primary display adapter.
	cbl::String primaryDeviceName;
	while( ::EnumDisplayDevices( NULL, count, &device, 0 ) != 0 ) {
		if( device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE ) {
			primaryDeviceName = device.DeviceName;
			break;
		}
		++count;
	}
	
	memset( &device, 0, sizeof( device ) );
	device.cb = sizeof( DISPLAY_DEVICE );
	count = 0;
	// Get the primary monitor.
	while( ::EnumDisplayDevices( primaryDeviceName.c_str(), count, &device, 0 ) != 0 ) {
		if( device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE ) {
			primaryDeviceName = device.DeviceName;
			break;
		}
		++count;
	}

	count = 0;
	while( ::EnumDisplaySettings( primaryDeviceName.c_str(), count, &devmode ) != 0 ) {
		if( sMinimumResolutionX > 0 && sMinimumResolutionY > 0 &&
			( devmode.dmPelsWidth < sMinimumResolutionX ||
			devmode.dmPelsHeight < sMinimumResolutionY ) ) {
			++count;
			continue;
		}

		GameWindowSize dims(
			cbl::Uint32( devmode.dmPelsWidth ),
			cbl::Uint32( devmode.dmPelsHeight ),
			cbl::Uint16( devmode.dmBitsPerPel )
		);

		if( dims.BitsPerPixel >= 16 ) {
			bool add = true;
			for( cbl::Uint32 i = 0; i < mAvailableResolutions.size(); ++i ) {
				if( mAvailableResolutions[i] == dims ) {
					add = false;
					break;
				}
			}
			if( add )
				mAvailableResolutions.push_back( dims );
		}

		++count;
	}
}

void GameWindow::SetCurrentDesktopResolution( void )
{
	mDesktopResolution.Width		= cbl::Uint32( ::GetSystemMetrics( SM_CXSCREEN ) );
	mDesktopResolution.Height		= cbl::Uint32( ::GetSystemMetrics( SM_CYSCREEN ) );
	mDesktopResolution.BitsPerPixel	= cbl::Uint16( ::GetDeviceCaps( GetDC(NULL), BITSPIXEL ) );
}

void GameWindow::GetWorkableArea( cbl::Uint32& left, cbl::Uint32& top, cbl::Uint32& right, cbl::Uint32& bottom, GameWindowStyle& style )
{
	RECT area;
	::SystemParametersInfo( SPI_GETWORKAREA, 0, &area, 0 );
	left	= area.left;
	top		= area.top;
	right	= area.right;
	bottom	= area.bottom;

	if( style.Fullscreen )
		return;

	left	+= ::GetSystemMetrics( SM_CXSIZEFRAME );
	right	-= ::GetSystemMetrics( SM_CXSIZEFRAME );
	top		+= ::GetSystemMetrics( SM_CYSMSIZE );
	bottom	-= ::GetSystemMetrics( SM_CYSIZEFRAME );

	RECT rc = area;
	::AdjustWindowRect( &rc, GameWindow::GetSystemWindowStyle( style ), FALSE );

	// Strip left.
	left	+= ( area.left - rc.left );
	// Strip right.
	right	-= ( rc.right - area.right );
	// Strip top.
	top		+= ( area.top - rc.top );
	// Strip bottom.
	bottom	-= ( rc.bottom - area.bottom );
}

void GameWindow::GetWorkableAreaCenter( cbl::Uint32& x, cbl::Uint32& y, GameWindowStyle& style )
{
	cbl::Uint32 wl, wt, wr, wb;
	GetWorkableArea( wl, wt, wr, wb, style );

	x = ( wl + wr ) / 2;
	y = ( wt + wb ) / 2;
}

void GameWindow::GetBestClientWindowDimensions( cbl::Uint32 targetw, cbl::Uint32 targeth, GameWindowStyle& style, cbl::Uint32& bestw, cbl::Uint32& besth )
{
	cbl::Uint32 wl, wt, wr, wb;
	GetWorkableArea( wl, wt, wr, wb, style );

	cbl::Uint32 workw = wr - wl, workh = wb - wt;

	cbl::Int32 diffx = cbl::Int32( workw ) - cbl::Int32( targetw );
	cbl::Int32 diffy = cbl::Int32( workh ) - cbl::Int32( targeth );

	// Fits.
	if( diffx >= 0 && diffy >= 0 ) {
		bestw = targetw;
		besth = targeth;
		return;
	}

	if( diffx < diffy ) {
		bestw = workw;
		besth = cbl::Math::Round<cbl::Uint32>( targeth * cbl::Real( workw ) / cbl::Real( targetw ) );
	}
	else {
		besth = workh;
		bestw = cbl::Math::Round<cbl::Uint32>( targetw * cbl::Real( workh ) / cbl::Real( targeth ) );
	}
}

cbl::Uint32 GameWindow::GetSystemWindowStyle( GameWindowStyle& style )
{
	// Set the window style.
	DWORD w32style = WS_VISIBLE | WS_CAPTION;

	if( style.Minimize )	w32style |= WS_MINIMIZEBOX;
	if( style.Close )		w32style |= WS_SYSMENU;

	return w32style;
}
