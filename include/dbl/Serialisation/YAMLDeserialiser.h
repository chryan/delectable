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
 * @file YAMLDeserialiser.h
 * @brief YAML deserialiser.
 */

#ifndef __DBL_YAMLDESERIALISER_H_
#define __DBL_YAMLDESERIALISER_H_

// Delectable Headers//
#include "dbl/Delectable.h"

// Chewable Headers //
#include "cbl/Serialisation/TreeDeserialiser.h"

// External Libraries //
#include <yaml-cpp/yaml.h>

namespace dbl
{
	//! YAML Deserialiser implementation.
	class DBL_API YAMLDeserialiser :
		public cbl::TreeDeserialiser
	{
	/***** Using Declarations *****/
	public:
		using cbl::Deserialiser::StreamPtr;
		
	/***** Public Methods *****/
	public:
		//! Constructor.
		YAMLDeserialiser();
		//! Check if the stream has ended.
		virtual bool IsStreamEnded( void ) const;
		//! Get the next value type. Does not advanced the stream.
		virtual bool GetValueType( StreamPtr s, cbl::String& type ) const;
		
	/***** Protected Methods *****/
	protected:
		//! Initialise the stream (implementation specific).
		virtual StreamPtr Initialise( StreamPtr s, const cbl::Type* type, void* obj );
		//! Shutdown the stream (implementation specific).
		virtual StreamPtr Shutdown( StreamPtr s, const cbl::Type* type, void* obj );
		//! Traverse the stream specified by the path.
		virtual StreamPtr TraverseStream( StreamPtr s, const cbl::Char* path );
		//! Virtual method to begin writing a container entry.
		virtual StreamPtr BeginContainerEntry( StreamPtr s, const cbl::Type* keyType, const cbl::Type* valType );
		//! Virtual method to end writing a container entry.
		virtual void EndContainerEntry( StreamPtr s, const cbl::Type* keyType, const cbl::Type* valType );
		//! Get the next container key stream.
		virtual StreamPtr GetContainerKeyStream( StreamPtr s ) const;
		//! Get the next container value stream.
		virtual StreamPtr GetContainerValueStream( StreamPtr s, bool hasKey ) const;
		//! Virtual method to start writing data of a type.
		virtual StreamPtr BeginValue( StreamPtr s, const cbl::Type* type, void * obj, const cbl::FieldAttr* attr );
		//! Virtual method to end writing data of a type.
		virtual void EndValue( StreamPtr s, const cbl::Type* type, void * obj, const cbl::FieldAttr* attr );
		//! Virtual method to begin writing fields.
		virtual StreamPtr BeginFields( StreamPtr s );
		//! Virtual method to end writing fields.
		virtual void EndFields( StreamPtr s );
		//! Virtual method to begin writing a field.
		virtual StreamPtr BeginField( StreamPtr s, const cbl::Field* field );
		//! Virtual method to end writing a field.
		virtual void EndField( StreamPtr s, const cbl::Field* field );
		//! Called when the stream has been set.
		virtual void OnStreamSet( void );
		
	/***** Private Members *****/
	private:
		bool				mHasDocument;	//!< Flag indicating if stream has a new YAML document.
		YAML::Node			mRoot;			//!< Current root YAML node.
		YAML::Iterator		mBeginIt;		//!< YAML begin iterator for field containers.
		YAML::Iterator		mEndIt;			//!< YAML end iterator for field containers.
	};
}

namespace cbl
{
	template<>
	DBL_API ObjectPtr ObjectManager::LoadObjectFromFile<dbl::YAMLDeserialiser>( const Char* file, const Char* name, bool init );
}

#endif // __DBL_YAMLDESERIALISER_H_
