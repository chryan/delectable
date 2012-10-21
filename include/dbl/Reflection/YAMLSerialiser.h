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
 * @file YAMLSerialiser.h
 * @brief YAML serialiser.
 */

#ifndef __DBL_YAMLSERIALISER_H_
#define __DBL_YAMLSERIALISER_H_

// Delectable Headers //
#include "dbl/Delectable.h"

// Chewable Headers //
#include "cbl/Reflection/NodeSerialiser.h"

namespace dbl
{
	//! YAML Serialiser implementation.
	class DBL_API YAMLSerialiser :
		public cbl::NodeSerialiser
	{
	/***** Using Declarations *****/
	public:
		using cbl::Serialiser::StreamPtr;
		
	/***** Public Methods *****/
	public:
		//! Constructor.
		YAMLSerialiser();
		
	/***** Protected Methods *****/
	private:
		//! Initialise the stream (implementation specific).
		virtual StreamPtr Initialise( StreamPtr s, const cbl::Type* type, const void* obj );
		//! Shutdown the stream (implementation specific).
		virtual StreamPtr Shutdown( StreamPtr s, const cbl::Type* type, const void* obj );
		//! Traverse the stream specified by the path.
		virtual StreamPtr TraverseStream( StreamPtr s, const cbl::Char* path );
		//! Virtual method to begin writing a container entry.
		virtual StreamPtr BeginContainerEntry( StreamPtr s, const cbl::Type* keyType, const cbl::Type* valType );
		//! Virtual method to end writing a container entry.
		virtual void EndContainerEntry( StreamPtr s, const cbl::Type* keyType, const cbl::Type* valType );
		//! Virtual method to begin writing a key for a container.
		virtual StreamPtr BeginContainerKey( StreamPtr s, const cbl::Type* keyType );
		//! Virtual method to end writing a key for a container.
		virtual void EndContainerKey( StreamPtr s, const cbl::Type* keyType );
		//! Virtual method to begin writing a value for a container.
		virtual StreamPtr BeginContainerValue( StreamPtr s, const cbl::Type* keyType, const cbl::Type* valType );
		//! Virtual method to end writing a value for a container.
		virtual void EndContainerValue( StreamPtr s, const cbl::Type* keyType, const cbl::Type* valType );
		//! Virtual method to start writing data of a type.
		virtual StreamPtr BeginValue( StreamPtr s, const cbl::Type* type, const void* obj, const cbl::FieldAttr* attr, cbl::Entity::OPTIONS opt, bool outputType );
		//! Virtual method to end writing data of a type.
		virtual void EndValue( StreamPtr s, const cbl::Type* type, const void* obj, const cbl::FieldAttr* attr, cbl::Entity::OPTIONS opt, bool outputType );
		//! Virtual method to begin writing fields.
		virtual StreamPtr BeginFields( StreamPtr s );
		//! Virtual method to end writing fields.
		virtual void EndFields( StreamPtr s );
		//! Virtual method to begin writing a field.
		virtual StreamPtr BeginField( StreamPtr s, const cbl::Field* field );
		//! Virtual method to end writing a field.
		virtual void EndField( StreamPtr s, const cbl::Field* field );
		//! Output serialised data to a file.
		virtual void OnOutput( StreamPtr s, const cbl::Char* filename );
		//! Called when the stream has been set.
		virtual void OnStreamSet( void );
		
	/***** Private Members *****/
	private:
		bool			mInlineContainer;	//!< Flag to indicate if the field container has the inline flag set.
		cbl::Uint32		mTraverseCount;		//!< Node traversal count.
	};
}

namespace cbl
{
	template<>
	DBL_API void ObjectManager::SaveObjectToFile<dbl::YAMLSerialiser>( const Char* file, ObjectPtr obj ) const;
}

#endif // __DBL_YAMLSERIALISER_H_
