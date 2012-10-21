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
 * @file test_KeyboardManager.cpp
 * @brief Unit testing for the keyboard manager.
 */

// Precompiled Headers //
#include <dbl/StdAfx.h>

// Delectable Headers //
#include <dbl/Input/KeyboardManager.h>
//#include <dbl/Input/KeyboardEvents.h>
#include <dbl/Core/Game.h>

// Chewable Headers //
#include <cbl/Util/Stopwatch.h>

// Google Test //
#include <gtest/gtest.h>

using namespace dbl;

class KeyboardTestGame : public Game
{
public:

	explicit KeyboardTestGame( const cbl::Char * name )
		: Game( name ),
		timeout( 2.0 )
	{
		Keyboard.OnKeyDown	+= E::KeyDown::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyDown)>(this);
		Keyboard.OnKeyUp	+= E::KeyUp::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyUp)>(this);
		Keyboard.OnKeyClick	+= E::KeyClick::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyClick)>(this);
		Keyboard.OnKeyChar	+= E::KeyChar::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyChar)>(this);

		stopwatch.Start();
	}

	virtual ~KeyboardTestGame()
	{
		stopwatch.Stop();
		
		Keyboard.OnKeyChar	-= E::KeyChar::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyChar)>(this);
		Keyboard.OnKeyClick	-= E::KeyClick::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyClick)>(this);
		Keyboard.OnKeyUp	-= E::KeyUp::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyUp)>(this);
		Keyboard.OnKeyDown	-= E::KeyDown::Method<CBL_E_METHOD(KeyboardTestGame,OnKeyDown)>(this);
	}

	virtual void Update( cbl::GameTime const & time )
	{
		Game::Update( time );
		if( stopwatch.GetElapsedTime().TotalSeconds() > timeout )
		{
			this->Exit();
		}
	}

	void OnKeyDown( dbl::Key::Code keyCode, cbl::Uint16 modifiers )
	{
		Stdout << "[KeyboardManager] Key Down: " << keyCode << std::endl;
	}
	
	void OnKeyUp( dbl::Key::Code keyCode, cbl::Uint16 modifiers )
	{
		Stdout << "[KeyboardManager] Key Up: " << keyCode << std::endl;
	}
	
	void OnKeyClick( dbl::Key::Code keyCode, cbl::Uint16 modifiers )
	{
		Stdout << "[KeyboardManager] Key Click: " << keyCode << std::endl;
	}
	
	void OnKeyChar( cbl::Uint32 unicode )
	{
		Stdout << "[KeyboardManager] Key Char: " << unicode << std::endl;
	}

public:
	cbl::Float64		timeout;
	cbl::Stopwatch		stopwatch;
};

class KeyboardManagerFixture : public ::testing::Test
{
public:
	KeyboardManagerFixture()
		: keyboardGame( "KeyboardManagerFixture" )
	{
	}

protected:
	KeyboardTestGame		keyboardGame;
};

TEST_F( KeyboardManagerFixture, KeyboardManager_KeyTest )
{
	keyboardGame.Run();
}