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
 * @file test_MouseManager.cpp
 * @brief Unit testing for the mouse manager.
 */

// Precompiled Headers //
#include <dbl/StdAfx.h>

// Delectable Headers //
#include <dbl/Input/MouseManager.h>
//#include <dbl/Input/MouseEvents.h>
#include <dbl/Core/Game.h>

// Chewable Headers //
#include <cbl/Util/Stopwatch.h>

// Google Test //
#include <gtest/gtest.h>

using namespace dbl;

class MouseTestGame : public Game
{
public:

	explicit MouseTestGame( const cbl::Char * name )
		: Game( name ),
		timeout( 2.0 )
	{
		Mouse.OnMouseMove		+= E::MouseMove::Method<CBL_E_METHOD(MouseTestGame,OnMouseMove)>(this);
		Mouse.OnMouseDrag		+= E::MouseDrag::Method<CBL_E_METHOD(MouseTestGame,OnMouseDrag)>(this);
		Mouse.OnMouseDown		+= E::MouseDown::Method<CBL_E_METHOD(MouseTestGame,OnMouseDown)>(this);
		Mouse.OnMouseUp			+= E::MouseUp::Method<CBL_E_METHOD(MouseTestGame,OnMouseUp)>(this);
		Mouse.OnMouseClick		+= E::MouseClick::Method<CBL_E_METHOD(MouseTestGame,OnMouseClick)>(this);
		Mouse.OnMouseDblClick	+= E::MouseDblClick::Method<CBL_E_METHOD(MouseTestGame,OnMouseDblClick)>(this);
		Mouse.OnMouseWheel		+= E::MouseWheel::Method<CBL_E_METHOD(MouseTestGame,OnMouseWheel)>(this);

		stopwatch.Start();
	}

	virtual ~MouseTestGame()
	{
		stopwatch.Stop();
		
		Mouse.OnMouseWheel		-= E::MouseWheel::Method<CBL_E_METHOD(MouseTestGame,OnMouseWheel)>(this);
		Mouse.OnMouseDblClick	-= E::MouseDblClick::Method<CBL_E_METHOD(MouseTestGame,OnMouseDblClick)>(this);
		Mouse.OnMouseClick		-= E::MouseClick::Method<CBL_E_METHOD(MouseTestGame,OnMouseClick)>(this);
		Mouse.OnMouseUp			-= E::MouseUp::Method<CBL_E_METHOD(MouseTestGame,OnMouseUp)>(this);
		Mouse.OnMouseDown		-= E::MouseDown::Method<CBL_E_METHOD(MouseTestGame,OnMouseDown)>(this);
		Mouse.OnMouseDrag		-= E::MouseDrag::Method<CBL_E_METHOD(MouseTestGame,OnMouseDrag)>(this);
		Mouse.OnMouseMove		-= E::MouseMove::Method<CBL_E_METHOD(MouseTestGame,OnMouseMove)>(this);
	}

	virtual void Update( cbl::GameTime const & time )
	{
		Mouse.SetCursor( "test_cursor.cur" );
		static bool t1 = false, t2 = false, t3 = false;
		Game::Update( time );
		if( stopwatch.GetElapsedTime().TotalSeconds() > timeout )
		{
			this->Exit();
		}
		else if( stopwatch.GetElapsedTime().TotalSeconds() > ( timeout * 2.0f / 3.0f ) && !t1 )
		{
			t1 = true;
			Mouse.SetCursorPosition( 1024, 768 );
		}
		else if( stopwatch.GetElapsedTime().TotalSeconds() > ( timeout / 3.0f ) && !t2 )
		{
			t2 = true;
			Mouse.CenterCursorPosition();
		}
		else if( !t3 )
		{
			t3 = true;
			Mouse.SetCursorPosition( 0, 0 );
		}

		static bool clipit = false;
		if( !clipit )
		{
			clipit = true;
			Mouse.ClipCursor( true );
			Mouse.LockMouse();
		}
	}

	void OnMouseMove( cbl::Int32 x, cbl::Int32 y, cbl::Int32 diffx, cbl::Int32 diffy )
	{
		//Stdout << "[MouseManager] Mouse Move: (" << x << ", " << y << "), Diff: (" << diffx << ", " << diffy << ")" << std::endl;
	}
	
	void OnMouseDrag( cbl::Int32 x, cbl::Int32 y, cbl::Int32 diffx, cbl::Int32 diffy, Mouse::Button button )
	{
		//Stdout << "[MouseManager] Mouse Drag: (" << x << ", " << y << ":" << button << "), Diff: (" << diffx << ", " << diffy << ")" << std::endl;
	}
	
	void OnMouseDown( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
	{
		//Stdout << "[MouseManager] Mouse Down: (" << x << ", " << y << ":" << button << ")" << std::endl;
	}
	
	void OnMouseUp( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
	{
		Stdout << "[MouseManager] Mouse Up: (" << x << ", " << y << ":" << button << ")" << std::endl;
	}
	
	void OnMouseClick( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
	{
		Stdout << "[MouseManager] Mouse Click: (" << x << ", " << y << ":" << button << ")" << std::endl;
	}

	void OnMouseDblClick( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
	{
		Stdout << "[MouseManager] Mouse Double Click: (" << x << ", " << y << ":" << button << ")" << std::endl;
	}
	
	void OnMouseWheel( cbl::Int32 delta )
	{
		Stdout << "[MouseManager] Mouse Wheel: " << delta << std::endl;
	}


public:
	cbl::Float64		timeout;
	cbl::Stopwatch		stopwatch;
};

class MouseManagerFixture : public ::testing::Test
{
public:
	MouseManagerFixture()
		: mouseGame( "MouseManagerFixture" )
	{
	}

protected:
	MouseTestGame		mouseGame;
};

TEST_F( MouseManagerFixture, MouseManager_KeyTest )
{
	mouseGame.Run();
}
