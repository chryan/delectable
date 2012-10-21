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
 * @file Platform.h
 * @brief Delectable platform configuration.
 */

#ifndef __DBL_PLATFORM_H_
#define __DBL_PLATFORM_H_

// Chewable Headers //
#include <cbl/Platform.h>

// Delectable Headers //
#include "dbl/Config.h"

namespace dbl
{
	/***** API Settings *****/
	#if defined( DBL_STATIC_LIB )
		// Linux compilers don't have symbol import/export directives.
		#define DBL_API
		#define DBL_PRIVATE
	#else
		#if defined( DBL_EXPORT )
			#define DBL_API			CBL_API_EXPORT
		#else
			#if defined( __MINGW32__ )
				#define DBL_API
			#else
				#define DBL_API		CBL_API_IMPORT
			#endif
		#endif

		#define CBL_PRIVATE
	#endif
}

#endif // __DBL_PLATFORM_H_
