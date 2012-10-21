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
 * @file Config.h
 * @brief Delectable configuration.
 */

#ifndef __DBL_CONFIG_H_
#define __DBL_CONFIG_H_

// Chewable Headers //
#include <cbl/Config.h>

namespace dbl
{
	/***** Versioning *****/
    #define DBL_VERSION_MAJOR 0
    #define DBL_VERSION_MINOR 2
    #define DBL_VERSION_PATCH 0

	/***** Debug Configurations *****/
	//! Define either DEBUG or _DEBUG to automatically define the value for DBL_DEBUG.
	#if ( DEBUG || _DEBUG ) 
		#define DBL_DEBUG	// Turn on debug.
	#endif

	#if ( STATIC_LIB || _STATIC_LIB )
		#define DBL_STATIC_LIB  // Turn on STATIC LIB.
	#endif
}

#endif // __DBL_CONFIG_H_
