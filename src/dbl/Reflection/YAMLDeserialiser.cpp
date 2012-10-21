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
 * @file YAMLDeserialiser.cpp
 * @brief YAML deserialiser.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers//
#include "dbl/Reflection/YAMLDeserialiser.h"

// External Libraries //
#include "yaml-cpp/yaml.h"

using namespace dbl;

typedef cbl::Deserialiser::StreamPtr StreamPtr;

YAMLDeserialiser::YAMLDeserialiser()
: mHasDocument( false )
{
}

bool YAMLDeserialiser::IsStreamEnded( void ) const
{
	return !mHasDocument;
}

bool YAMLDeserialiser::GetValueType( StreamPtr s, cbl::String& type ) const
{
	const cbl::String& tag = ((YAML::Node*)s)->Tag();
	if( tag.length() > 0 ) {
		type = tag.c_str() + 1;
		return true;
	}
	return false;
}

StreamPtr YAMLDeserialiser::Initialise( StreamPtr s, const cbl::Type* type, void * )
{
	if( mHasDocument ) {
		cbl::String typeStr;
		if( GetValueType( &mRoot, typeStr ) ) {
			const cbl::Type* targetType = CBL_ENT.Types.Get( cbl::CName( typeStr.c_str() ) );
			if( targetType && targetType->IsType( type->Name ) )
				return &mRoot;
		}
		mHasDocument = (*(YAML::Parser*)s).GetNextDocument( mRoot );
	}
	return NULL;
}

StreamPtr YAMLDeserialiser::Shutdown( StreamPtr s, const cbl::Type*, void * )
{
	mHasDocument = (*(YAML::Parser*)s).GetNextDocument( mRoot );
	return s;
}

StreamPtr YAMLDeserialiser::TraverseStream( StreamPtr s, const cbl::Char* path )
{
	try { return const_cast<YAML::Node*>(((YAML::Node*)s)->FindValue( path )); }
	catch( const YAML::Exception& e ) { LOG_ERROR( e.what() ); }
	return NULL;
}

StreamPtr YAMLDeserialiser::BeginContainerEntry( StreamPtr, const cbl::Type*, const cbl::Type* )
{
	if( mBeginIt == mEndIt )
		return NULL;

	// All our containers are supposed to be in a list.
	return const_cast<YAML::Node*>(&(*mBeginIt));
}

void YAMLDeserialiser::EndContainerEntry( StreamPtr, const cbl::Type*, const cbl::Type* )
{
	++mBeginIt;
}

StreamPtr YAMLDeserialiser::GetContainerKeyStream( StreamPtr s ) const
{
	try {
		return const_cast<YAML::Node*>( ((YAML::Node*)s)->FindValue( "Key" ) );
	} catch( const YAML::Exception& e ) {
		LOG_ERROR( e.what() );
	}
	return NULL;
}

StreamPtr YAMLDeserialiser::GetContainerValueStream( StreamPtr s, bool hasKey ) const
{
	if( !hasKey ) return s;

	try {
		return const_cast<YAML::Node*>( ((YAML::Node*)s)->FindValue( "Value" ) );
	} catch( const YAML::Exception& e ) {
		LOG_ERROR( e.what() );
	}
	return NULL;
}

StreamPtr YAMLDeserialiser::BeginValue( StreamPtr s, const cbl::Type* type, void * obj, const cbl::FieldAttr* attr )
{
	if( type->FromString ) {
		cbl::String value;
		if( ((YAML::Node*)s)->GetScalar( value ) ) {
			type->FromString( value.c_str(), type, obj, attr );
			return NULL; // We handled the value.
		}
	}
	return s;
}

void YAMLDeserialiser::EndValue( StreamPtr, const cbl::Type*, void *, const cbl::FieldAttr* )
{
}

StreamPtr YAMLDeserialiser::BeginFields( StreamPtr s )
{
	return s;
}

void YAMLDeserialiser::EndFields( StreamPtr )
{
}

StreamPtr YAMLDeserialiser::BeginField( StreamPtr s, const cbl::Field* field )
{
	try {
		YAML::Node* node = const_cast<YAML::Node*>(((YAML::Node*)s)->FindValue( field->Name.Text ));
		if( node && field->Container ) {
			if( node->Type() == YAML::NodeType::Sequence ) {
				mBeginIt	= node->begin();
				mEndIt		= node->end();
			}
			else {
				LOG_ERROR( "Field container (" << field->Name.Text << ") is not a YAML sequence." );
			}
		}
		return node;

	} catch( const YAML::Exception& e ) {
		LOG_ERROR( e.what() );
	}

	return NULL;
}

void YAMLDeserialiser::EndField( StreamPtr, const cbl::Field* )
{
	//mFieldIterator;
}

void YAMLDeserialiser::OnStreamSet( void )
{
	mHasDocument = (*(YAML::Parser*)mStream).GetNextDocument( mRoot );
}

template<>
cbl::ObjectPtr cbl::ObjectManager::LoadObjectFromFile<YAMLDeserialiser>( const cbl::Char* file, const cbl::Char* name, bool init )
{
	std::ifstream fs;
	fs.open( file, std::ios_base::binary );

	if( !fs.is_open() ) {
		LOG_ERROR( "Unable to open object file for reading: " << file );
		return NULL;
	}

	YAML::Parser parser;

	try { parser.Load( fs ); }
	catch( const YAML::Exception& e ) {
		LOG_ERROR( e.what() );
		fs.close();
		return NULL;
	}

	YAMLDeserialiser yd;
	yd.SetStream( parser );

	ObjectPtr newObj = NULL;
	bool success = yd.DeserialisePtr( newObj );
	if( success ) {
		if( name ) newObj->mName = name;
		success = Add( newObj );
		if( !success ) {
			CBL_ENT.Delete( newObj );
			newObj = NULL;
		}
	}

	if( success ) {
		LOG( "Object (" << newObj->GetName() << ") loaded from file: " << file );
	} else {
		LOG_ERROR( "Unable to deserialise from binary file: " << file );
	}

	fs.close();

	if( init )
		InitObject( newObj );

	return newObj;
}