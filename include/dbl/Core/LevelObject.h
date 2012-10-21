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
 * @file LevelObject.h
 * @brief Level object.
 */

#ifndef __DBL_LEVELOBJECT_H_
#define __DBL_LEVELOBJECT_H_

// Delectable Headers //
#include "dbl/Delectable.h"

// Chewable Headers //
#include "cbl/Core/Object.h"

namespace dbl
{
	class DBL_API LevelObject :
		public cbl::Object
	{
	public:
		//! Protected constructor.
		//! We only want the ObjectManager instantiating this.
		//! @param	hashName	Object's hash name.
		LevelObject();
		// Destructor.
		virtual ~LevelObject();
		//! Object initialisation method.
		//! Called by the object manager when created.
		virtual void Initialise( void );
		//! Object shutdown method.
		//! Called by the object manager when destroyed.
		virtual void Shutdown( void );
	};
}

CBL_TYPE( dbl::LevelObject, LevelObject );

#endif // __DBL_LEVELOBJECT_H_
