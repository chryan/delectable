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
 * @file test_LevelManager.cpp
 * @brief Level manager unit tests.
 */

// Precompiled Headers //
#include <dbl/StdAfx.h>

// Delectable Headers //
#include <dbl/Core/LevelManager.h>
#include <cbl/Core/Game.h>

// Google Test //
#include <gtest/gtest.h>

#pragma warning( disable: 4355 )

using namespace dbl;

class LMPartTest : public cbl::ObjectPart
{
public:
	cbl::Uint32		Value;
};

CBL_TYPE( LMPartTest, LMPartTest );

class LevelManagerGameTest : public cbl::Game
{
public:
	LevelManager	LM;

	explicit LevelManagerGameTest( const cbl::Char* name )
	: cbl::Game( name )
	, LM( *this )
	, Counter( 5 )
	, SaveBeginDone( false )
	, SaveEndDone( false )
	, LoadBeginDone( false )
	, LoadEndDone( false )
	, Binary( false )
	{
		LM.MaxLoadBatchTime = DBL_MAX;

		Components.Add( &LM );
		Services.Add( &LM );
	}

	virtual void Initialise( void )
	{
		cbl::Game::Initialise();

		LM.OnLevelLoadBegin	+= dbl::E::LevelLoadBegin::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelLoadBegin)>(this);
		LM.OnLevelLoadEnd	+= dbl::E::LevelLoadEnd::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelLoadEnd)>(this);
		LM.OnLevelSaveBegin	+= dbl::E::LevelSaveBegin::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelSaveBegin)>(this);
		LM.OnLevelSaveEnd	+= dbl::E::LevelSaveEnd::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelSaveEnd)>(this);

		{
			for( size_t i = 0; i < 20; ++i ) {
				LevelObject* obj = Objects.Create<LevelObject>("LMObject0");
				ASSERT_TRUE( obj != NULL );
				LMPartTest* lmpt = obj->Parts.Add<LMPartTest>();
				ASSERT_TRUE( lmpt != NULL );
				lmpt->Value = i;
			}
			LM.Save<YAMLSerialiser>( "lmobjs.yaml" );
			LM.Save<cbl::BinarySerialiser>( "lmobjs.bin" );
		}

		Objects.DestroyAll();
	}

	virtual void Shutdown( void )
	{
		LM.OnLevelSaveEnd	+= dbl::E::LevelSaveEnd::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelSaveEnd)>(this);
		LM.OnLevelSaveBegin	+= dbl::E::LevelSaveBegin::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelSaveBegin)>(this);
		LM.OnLevelLoadEnd	-= dbl::E::LevelLoadEnd::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelLoadEnd)>(this);
		LM.OnLevelLoadBegin	-= dbl::E::LevelLoadBegin::Method<CBL_E_METHOD(LevelManagerGameTest,OnLevelLoadBegin)>(this);

		cbl::Game::Shutdown();
	}

	virtual void Update( const cbl::GameTime& time ) {
		cbl::Game::Update( time );

		if( --Counter == 0 ) {
			CheckNoObjects();
			if( Binary )
				LM.Load<cbl::BinaryDeserialiser>( "lmobjs.bin" );
			else
				LM.Load<YAMLDeserialiser>( "lmobjs.yaml" );
		}

		// Timeout if we took too long.
		if( time.Total.TotalSeconds() > 5.0 )
			this->Exit();
	}

	void OnLevelSaveBegin( const cbl::FileInfo& ) {
		SaveBeginDone = true;
	}

	void OnLevelSaveEnd( const cbl::FileInfo& ) {
		SaveEndDone = true;
	}

	void OnLevelLoadBegin( const cbl::FileInfo& ) {
		LoadBeginDone = true;
	}

	void OnLevelLoadEnd( const cbl::FileInfo& ) {
		this->Exit();
		LoadEndDone = true;
		for( size_t i = 0; i < 20; ++i ) {
			char name[255];
			sprintf( name, "LMObject%d", i );
			ASSERT_TRUE( Objects.Get<LevelObject>(name) != NULL );
		}
	}

	void CheckNoObjects( void ) {
		for( size_t i = 0; i < 20; ++i ) {
			char name[255];
			sprintf( name, "LMObject%d", i );
			ASSERT_TRUE( Objects.Get<LevelObject>(name) == NULL );
		}
	}

	cbl::Int32	Counter;
	bool		SaveBeginDone;
	bool		SaveEndDone;
	bool		LoadBeginDone;
	bool		LoadEndDone;
	bool		Binary;
};

namespace cbl
{
	struct DummAccessLM {};
	template<>
	DummAccessLM* EntityManager::New<DummAccessLM>( void ) const
	{
		this->EntityManager::~EntityManager();
		this->EntityManager::EntityManager();
		cbl::CblRegistrar::RegisterCblTypes();
		dbl::DblRegistrar::RegisterDblTypes();
		return NULL;
	}
}
void ForceReconstructEntityManager_LM( void )
{
	using namespace cbl;
	const_cast<EntityManager*>( EntityManager::InstancePtr() )->New<DummAccessLM>();
}

TEST( LevelManagerTestFixture, LevelManagerTest_YAML )
{
	CBL_ENT.Types.Create<LMPartTest>()
		.Base<cbl::ObjectPart>()
		.CBL_FIELD( Value, LMPartTest );

	LevelManagerGameTest game( "LMTest" );

	game.Run();

	ASSERT_TRUE( game.SaveBeginDone );
	ASSERT_TRUE( game.SaveEndDone );
	ASSERT_TRUE( game.LoadBeginDone );
	ASSERT_TRUE( game.LoadEndDone );

	ForceReconstructEntityManager_LM();
}

TEST( LevelManagerTestFixture, LevelManagerTest_Binary )
{
	CBL_ENT.Types.Create<LMPartTest>()
		.Base<cbl::ObjectPart>()
		.CBL_FIELD( Value, LMPartTest );

	LevelManagerGameTest game( "LMTest" );
	game.Binary = true;

	game.Run();

	ASSERT_TRUE( game.SaveBeginDone );
	ASSERT_TRUE( game.SaveEndDone );
	ASSERT_TRUE( game.LoadBeginDone );
	ASSERT_TRUE( game.LoadEndDone );

	ForceReconstructEntityManager_LM();
}