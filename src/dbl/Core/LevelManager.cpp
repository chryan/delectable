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
 * @file LevelManager.cpp
 * @brief Level manager component.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Core/LevelManager.h"
#include "dbl/Core/LevelObject.h"

using namespace dbl;

LevelManager::LevelManager( cbl::Game& game )
: cbl::DrawableGameComponent( game )
, MaxLoadBatchTime( 1.0f )
, mSerialiser( NULL )
, mDeserialiser( NULL )
, mUnloadWait( 0 )
{
	// We use this for waiting for the objects to unload.
	Enabled = false;
	// We use this for the actual incremental loading.
	Visible = false;
}

LevelManager::~LevelManager()
{
}

void LevelManager::Initialise( void )
{
}

void LevelManager::Shutdown( void )
{
	mLevelObjects.clear();

	CBL_DELETE( mSerialiser );
	CBL_DELETE( mDeserialiser );
}

void LevelManager::Update( const cbl::GameTime& )
{
	if( --mUnloadWait == 0 ) {
		Enabled = false;
		if( mDeserialiser ) {
			Visible = true;
		}
	}
}

void LevelManager::Draw( const cbl::GameTime& )
{
	if( !mDeserialiser ) {
		LOG_ERROR( "Deserialiser was not initialised." );
		Visible = false;
		return;
	}

	// Perform the incremental loading.
	cbl::Stopwatch timer;
	timer.Start();
	while( !mDeserialiser->IsStreamEnded() && ( timer.GetElapsedTime().TotalSeconds() < MaxLoadBatchTime ) ) {
		Game.Objects.DeserialiseObject<LevelObject>( *mDeserialiser );
	}

	// The loading is completed.
	if( mDeserialiser->IsStreamEnded() ) {
		OnLevelLoadEnd( mLoadedLevel );
		Visible = false;
		CBL_DELETE( mDeserialiser );
		LOG( mLoadedLevel.GetFile() << " level loaded." );
		_LoadDone();
	}
}

void LevelManager::Unload( void )
{
	mUnloadWait = 2;
	Enabled = true;

	if( mLevelObjects.size() > 0 ) {
		for( size_t i = 0; i < mLevelObjects.size(); ++i )
			Game.Objects.Destroy( mLevelObjects[i] );
		OnLevelUnload( mLoadedLevel );
		mLevelObjects.clear();
	}
}

void LevelManager::SetupLoad( const cbl::Char* file, bool unload )
{
	if( unload )
		Unload();

	mLoadedLevel = file;
	OnLevelLoadBegin( mLoadedLevel );
}

void LevelManager::Add( LevelObject* obj )
{
	cbl::ObjectID id = obj->GetID();
	for( size_t i = 0; i < mLevelObjects.size(); ++i ) {
		if( mLevelObjects[i] == id ) {
			LOG_ERROR( "Attemping to re-add an existing object to the list! Object: " << obj->GetName() );
			return;
		}
	}
	mLevelObjects.push_back( id );
}

void LevelManager::Remove( LevelObject* obj )
{
	if( mLevelObjects.size() > 0 ) {
		cbl::ObjectID id = obj->GetID();
		for( size_t i = 0; i < mLevelObjects.size(); ++i ) {
			if( id == mLevelObjects[i] ) {
				mLevelObjects[i] = mLevelObjects.back();
				mLevelObjects.pop_back();
				return;
			}
		}
		LOG_ERROR( "Attemping to remove an object that doesn't exist in the list! Object: " << obj->GetName() );
	}
}
