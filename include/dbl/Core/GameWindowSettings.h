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
 * @file GameWindowSettings.h
 * @brief Application window settings structure.
 */

#ifndef __DBL_GAMEWINDOWSETTINGS_H_
#define __DBL_GAMEWINDOWSETTINGS_H_

// Delectable Headers //
#include "dbl/Delectable.h"

namespace dbl
{
#if CBL_PLATFORM == CBL_PLATFORM_WIN32
	// Windows defines a void* handle (HWND)
	typedef void *		GameWindowHandle;
#endif

	//! Game window styles.
	//! Use bitwise OR to combine options.
	struct DBL_API GameWindowStyle
	{
		bool Close;			//!< Close button
		bool Minimize;		//!< Minimize button
		bool Fullscreen;	//!< Fullscreen mode (this flag and all others are mutually exclusive)

		GameWindowStyle()
			: Close( true ), Minimize( false ), Fullscreen( false ) {}
	};

	//! Game window dimensions.
	struct DBL_API GameWindowSize
	{
		//! Constructor.
		//! @param	width		Window width.
		//! @param	height		Window height.
		//! @param	bpp			Window bits per pixel.
		GameWindowSize( cbl::Uint32 width = 0, cbl::Uint32 height = 0, cbl::Uint16 bpp = 16 )
			: Width( width ), Height( height ), BitsPerPixel( bpp ) {}

		cbl::Uint32	Width;			//!< Window width.
		cbl::Uint32	Height;			//!< Window height.
		cbl::Uint16	BitsPerPixel;	//!< Window bits per pixel.

		bool operator == ( const GameWindowSize & rhs ) const {
			return Width == rhs.Width && Height == rhs.Height && BitsPerPixel == rhs.BitsPerPixel;
		}
	};

	//! Game window settings structure.
	struct DBL_API GameWindowSettings
	{
		//! Constructor.
		//! @param	title		Window title.
		//! @param	width		Window width.
		//! @param	height		Window height.
		//! @param	bpp			Window bits per pixel.
		//! @param	style		Window style.
		GameWindowSettings( const cbl::Char * title = "Delectable Window",
			cbl::Uint32 width = 1024, cbl::Uint32 height = 768,
			cbl::Uint16 bpp = 16 )
			: Title( title )
			, Dimensions( width, height, 0 )
			, Resolution( width, height, bpp )
			, AspectRatio( 0.0f )
			{}

		cbl::String			Title;			//!< Window title.
		GameWindowStyle		Style;			//!< Window style.
		GameWindowSize		Dimensions;		//!< Actual window dimensions. (This changes depending on the actual window client size that is set)
		GameWindowSize		Resolution;		//!< Window target resolution. (This is always constant)
		cbl::Real			AspectRatio;
	};
}

CBL_TYPE( dbl::GameWindowStyle, GameWindowStyle );
CBL_TYPE( dbl::GameWindowSize, GameWindowSize );
CBL_TYPE( dbl::GameWindowSettings, GameWindowSettings );

#endif // __DBL_GAMEWINDOWSETTINGS_H_
