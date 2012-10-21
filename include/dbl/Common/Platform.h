/**
 * @file Platform.h
 * @brief Tasty platform configuration.
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

#ifndef __TASTY_PLATFORM_H_
#define __TASTY_PLATFORM_H_

// Tasty Headers //
#include <Tasty/Common/Config.h>

// Chewable Headers //
#include <Chewable/Common/Platform.h>

namespace Tasty
{
	//----------------------------------------------------------------------------
	// API Settings

	#if defined( TASTY_STATIC_LIB )
		// Linux compilers don't have symbol import/export directives.
		#define TASTY_API
		#define TASTY_PRIVATE
	#else
		#if defined( TASTY_EXPORT )
			#define TASTY_API			CBL_API_EXPORT
		#else
			#if defined( __MINGW32__ )
				#define TASTY_API
			#else
				#define TASTY_API		CBL_API_IMPORT
			#endif
		#endif

		#define CBL_PRIVATE
	#endif
	//----------------------------------------------------------------------------
}

#endif // __TASTY_PLATFORM_H_