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
 * @file DblRegistrar.h
 * @brief Delectable type registrar.
 */

#ifndef __DBL_DBLREGISTRAR_H_
#define __DBL_DBLREGISTRAR_H_

// Delectable Headers //
#include "dbl/Delectable.h"

namespace dbl
{
	//! Automatic delectable type registration class.
	class DBL_API DblRegistrar
	{
	/***** Public Static Methods *****/
	public:
		//! Register Delectable types in the CBL_ENT.
		static void RegisterDblTypes( void );
		
	/***** Public Methods *****/
	private:
		//! Constructor.
		DblRegistrar();
		
	/***** Private Static Members *****/
	private:
		static DblRegistrar _sInstance;	//!< Static instance to handle automatic registration.
	};
}

#endif // __DBL_DBLREGISTRAR_H_
