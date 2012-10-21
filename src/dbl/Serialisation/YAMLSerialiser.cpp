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
 * @file YAMLSerialiser.cpp
 * @brief YAML serialiser.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Serialisation/YAMLSerialiser.h"

// External Libraries //
#include <yaml-cpp/yaml.h>
#include <fstream>

using namespace dbl;

typedef cbl::Serialiser::StreamPtr StreamPtr;

YAMLSerialiser::YAMLSerialiser()
: mInlineContainer( false )
, mTraverseCount( 0 )
{
}

void YAMLSerialiser::OnOutput( StreamPtr s, const cbl::Char* filename )
{
	std::ofstream file;
	file.open( filename );

	if( !file.is_open() ) {
		LOG_ERROR( "Unable to write to file: " << filename );
		return;
	}

	file << (*(YAML::Emitter*)s).c_str();

	file.close();
	return;
}

StreamPtr YAMLSerialiser::Initialise( StreamPtr s, const cbl::Type*, const void* )
{
	(*(YAML::Emitter*)s)
		<< YAML::BeginDoc;

	return s;
}

StreamPtr YAMLSerialiser::Shutdown( StreamPtr s, const cbl::Type*, const void* )
{
	for( cbl::Uint32 i = 0; i < mTraverseCount; ++i )
		(*(YAML::Emitter*)s) << YAML::EndMap;

	mTraverseCount = 0;

	//(*(YAML::Emitter*)s) << YAML::EndDoc;

	return s;
}

StreamPtr YAMLSerialiser::TraverseStream( StreamPtr s, const cbl::Char* path )
{
	if( !s ) {
		LOG_ERROR( "Unable to traverse stream. No stream set." );
		return NULL;
	}

	YAML::Emitter& e = (*(YAML::Emitter*)s);

	e << YAML::BeginMap
		<< YAML::Key << path << YAML::Value;

	++mTraverseCount;
	return s;
}

StreamPtr YAMLSerialiser::BeginContainerEntry( StreamPtr s, const cbl::Type* keyType, const cbl::Type* )
{
	if( mInlineContainer ) {
		(*(YAML::Emitter*)s)
			<< YAML::Flow;
	}

	if( keyType ) {
		(*(YAML::Emitter*)s)
			<< YAML::BeginMap;
	}

	return s;
}

void YAMLSerialiser::EndContainerEntry( StreamPtr s, const cbl::Type* keyType, const cbl::Type* )
{
	if( keyType ) {
		(*(YAML::Emitter*)s)
			<< YAML::EndMap;
	}
}

StreamPtr YAMLSerialiser::BeginContainerKey( StreamPtr s, const cbl::Type* )
{
	(*(YAML::Emitter*)s)
		<< YAML::Key << "Key"
		<< YAML::Value;

	return s;
}

void YAMLSerialiser::EndContainerKey( StreamPtr, const cbl::Type* )
{
}

StreamPtr YAMLSerialiser::BeginContainerValue( StreamPtr s, const cbl::Type* keyType, const cbl::Type* )
{
	if( keyType ) {
		(*(YAML::Emitter*)s)
			<< YAML::Key << "Value"
			<< YAML::Value;
	}

	return s;
}

void YAMLSerialiser::EndContainerValue( StreamPtr, const cbl::Type*, const cbl::Type* )
{
}

StreamPtr YAMLSerialiser::BeginValue( StreamPtr s, const cbl::Type* type, const void* obj, const cbl::FieldAttr* attr, cbl::Entity::OPTIONS, bool outputType )
{
	if( outputType )
		(*(YAML::Emitter*)s) << YAML::LocalTag( type->Name.Text );

	if( type->ToString ) {
		cbl::String tempValue;
		type->ToString( tempValue, type, obj, attr );
		(*(YAML::Emitter*)s)
			<< tempValue;
		return NULL;
	}

	return s;
}

void YAMLSerialiser::EndValue( StreamPtr s, const cbl::Type* type, const void*, const cbl::FieldAttr*, cbl::Entity::OPTIONS opt, bool )
{
	if( ( !type->HasFields() || opt == cbl::Entity::O_IGNORE_FIELDS ) && type->ToString == NULL )
		(*(YAML::Emitter*)s) << YAML::Null;
}

StreamPtr YAMLSerialiser::BeginFields( StreamPtr s )
{
	(*(YAML::Emitter*)s)
		<< YAML::BeginMap;

	return s;
}

void YAMLSerialiser::EndFields( StreamPtr s )
{
	(*(YAML::Emitter*)s)
		<< YAML::EndMap;
}

StreamPtr YAMLSerialiser::BeginField( StreamPtr s, const cbl::Field* field )
{
	(*(YAML::Emitter*)s)
		<< YAML::Key << field->Name.Text
		<< YAML::Value;

	if( field->Container ) {
		(*(YAML::Emitter*)s)
			<< YAML::BeginSeq;

		mInlineContainer	= field->Attributes.Inline != 0;
	}

	return s;
}

void YAMLSerialiser::EndField( StreamPtr s, const cbl::Field* field )
{
	if( field->Container ) {
		(*(YAML::Emitter*)s)
			<< YAML::EndSeq;
	}
}

void YAMLSerialiser::OnStreamSet( void )
{
}

template<>
void cbl::ObjectManager::SaveObjectToFile<dbl::YAMLSerialiser>( const cbl::Char* file, cbl::ObjectPtr obj ) const
{
	if( !obj ) {
		LOG_ERROR( "No object to write to binary file: " << file );
		return;
	}

	std::ofstream fs;
	fs.open( file, std::ios_base::binary );

	if( !fs.is_open() ) {
		LOG_ERROR( "Unable to open object file for writing: " << file );
		return;
	}
	fs.close();

	YAML::Emitter e;
	YAMLSerialiser ys;
	ys
		.SetStream( e )
		.Serialise( *obj );
	ys.Output( file );

	LOG( "Object (" << obj->GetName() << ") saved to file: " << file );
}
