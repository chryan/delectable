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
 * @file LevelManager_Serialisers.cpp
 * @brief Level manager specialised serialiser methods.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Core/LevelManager.h"
#include "dbl/Core/LevelObject.h"

YAML::Parser	sLocalParser;
std::ifstream	sLocalFileInStream;
std::ofstream	sLocalFileOutStream;

using namespace dbl;

template<>
void LevelManager::Load<YAMLDeserialiser>( const cbl::Char* file, bool unload )
{
	if( sLocalFileInStream.is_open() )
		sLocalFileInStream.close();

	sLocalFileInStream.open( file );
	if( !sLocalFileInStream.is_open() ) {
		LOG_ERROR( "Unable to open YAML file for reading: " << file );
		return;
	}

	try {
		sLocalParser.Load( sLocalFileInStream );
	}
	catch( const YAML::Exception& e ) {
		LOG_ERROR( e.what() );
		return;
	}

	CBL_DELETE( mDeserialiser );

	mDeserialiser = new YAMLDeserialiser();
	mDeserialiser->SetStream( sLocalParser );

	SetupLoad( file, unload );
}

template<>
void LevelManager::Save<YAMLSerialiser>( const cbl::Char* file ) const
{
	CBL_DELETE( mSerialiser );

	mLoadedLevel = file;
	const_cast<LevelManager*>(this)->OnLevelSaveBegin( mLoadedLevel );

	YAML::Emitter e;
	mSerialiser = new YAMLSerialiser();
	mSerialiser->SetStream( e );

	for( size_t i = 0; i < mLevelObjects.size(); ++i ) {
		if( cbl::ObjectPtr obj = Game.Objects.Get( mLevelObjects[i] ) )
			mSerialiser->Serialise( *obj );
	}

	((YAMLSerialiser*)(mSerialiser))->Output( mLoadedLevel.GetFullFile().c_str() );

	LOG( mLoadedLevel.GetFile() << " level saved." );

	const_cast<LevelManager*>(this)->OnLevelSaveEnd( mLoadedLevel );
}

template<>
void LevelManager::Load<cbl::BinaryDeserialiser>( const cbl::Char* file, bool unload )
{
	if( sLocalFileInStream.is_open() )
		sLocalFileInStream.close();

	sLocalFileInStream.open( file, std::ios_base::binary );
	if( !sLocalFileInStream.is_open() ) {
		LOG_ERROR( "Unable to open binary level file for reading: " << file );
		return;
	}

	CBL_DELETE( mDeserialiser );

	mDeserialiser = new cbl::BinaryDeserialiser();
	mDeserialiser->SetStream( sLocalFileInStream );

	SetupLoad( file, unload );
}

template<>
void LevelManager::Save<cbl::BinarySerialiser>( const cbl::Char* file ) const
{
	if( sLocalFileOutStream.is_open() )
		sLocalFileOutStream.close();

	sLocalFileOutStream.open( file, std::ios_base::binary );
	if( !sLocalFileOutStream.is_open() ) {
		LOG_ERROR( "Unable to open binary level file for writing: " << file );
		return;
	}

	CBL_DELETE( mSerialiser );

	mLoadedLevel = file;
	const_cast<LevelManager*>(this)->OnLevelSaveBegin( mLoadedLevel );

	mSerialiser = new cbl::BinarySerialiser();
	mSerialiser->SetStream( sLocalFileOutStream );
	
	for( size_t i = 0; i < mLevelObjects.size(); ++i ) {
		if( cbl::ObjectPtr obj = Game.Objects.Get( mLevelObjects[i] ) )
			mSerialiser->Serialise( *obj );
	}

	sLocalFileOutStream.close();

	LOG( mLoadedLevel.GetFile() << " level saved." );

	const_cast<LevelManager*>(this)->OnLevelSaveEnd( mLoadedLevel );
}

void LevelManager::_LoadDone( void )
{
	if( sLocalFileOutStream.is_open() )
		sLocalFileOutStream.close();

	if( sLocalFileInStream.is_open() )
		sLocalFileInStream.close();
}