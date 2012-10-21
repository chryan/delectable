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
 * @file test_Game.cpp
 * @brief Unit testing for the windowed game class.
 */

// Precompiled Headers //
#include <dbl/StdAfx.h>

// Delectable Headers //
#include <dbl/Core/Game.h>

// Google Test //
#include <gtest/gtest.h>

using namespace dbl;

class GameFixture : public ::testing::Test
{
protected:

	class TestGame : public ::dbl::Game
	{
	public:
		explicit TestGame( const cbl::Char * name, cbl::Float64 timeout )
			: Game( name ),
			Timeout( timeout )
		{
		}

	public:

		virtual void Update( cbl::GameTime const & time )
		{
			Game::Update( time );

			Timeout -= time.Elapsed.TotalSeconds();
			if( Timeout < 0.0 )
			{
				this->Exit();
			}
		}

	public:

		cbl::Float64		Timeout;
	};
public:
	GameFixture()
		: windowedGame( "Windowed Game", 2.0 )
	{
	}

protected:

	TestGame			windowedGame;
};

TEST_F( GameFixture, GameFixture_Test )
{
	windowedGame.Run();
}