/**
 * @file Config.h
 * @brief Tasty configuration.
 *
 * Copyright 2009 Ryan Chew
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

#pragma once

#ifndef __TASTY_CONFIG_H_
#define __TASTY_CONFIG_H_

// Chewable Headers //
#include <Chewable/Common/Config.h>

namespace Tasty
{
	//-----------------------------------------------------------------------------
	// Versioning

    #define TASTY_VERSION_MAJOR 0
    #define TASTY_VERSION_MINOR 1
    #define TASTY_VERSION_PATCH 0
	//-----------------------------------------------------------------------------

	//-----------------------------------------------------------------------------
	// Debug Configurations

	/// Define either DEBUG or _DEBUG to automatically define the value for TASTY_DEBUG.
	#if ( DEBUG || _DEBUG ) 
		#define TASTY_DEBUG	// Turn on debug.
	#endif

	#if ( STATIC_LIB || _STATIC_LIB )
		#define TASTY_STATIC_LIB  // Turn on STATIC LIB.
	#endif
	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// Graphics Configurations

	#define TASTY_GRAPHICS_DIRECTX9			0
	#define TASTY_GRAPHICS_DIRECTX10		1
	#define TASTY_GRAPHICS_OPENGL			2
	#define TASTY_GRAPHICS_DIRECTX			TASTY_GRAPHICS_DIRECTX9

	#define TASTY_GRAPHICS					TASTY_GRAPHICS_DIRECTX
	//-----------------------------------------------------------------------------
}

#endif // __TASTY_CONFIG_H_