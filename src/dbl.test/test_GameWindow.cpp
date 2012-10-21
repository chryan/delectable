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
 * @file test_GameWindow.cpp
 * @brief Unit testing for the game window.
 */

// Precompiled Headers //
#include <dbl/StdAfx.h>

// Delectable Headers //
#include <dbl/Core/GameWindow.h>

// Chewable Headers //
#include <cbl/Util/Stopwatch.h>
#include <cbl/Core/Game.h>

// Google Test //
#include <gtest/gtest.h>

using namespace dbl;

class GameWindowFixture : public ::testing::Test
{
public:

	GameWindowFixture()
		: dummyGame( "GameWindow_Test" ),
		gameWindow( dummyGame ),
		timeout( 2.0 ),
		windowCreated( false ),
		windowDestroyed( false ),
		mouseMoveCount( 0 )
	{
	}

	void SetUp()
	{
		timeout = 1.0;
		fullscreenTimeout = 2;

		gameWindow.OnWindowCreated		+= E::WindowCreated::Method<CBL_E_METHOD(GameWindowFixture,OnWindowCreated)>(this);
		gameWindow.OnWindowDestroyed	+= E::WindowDestroyed::Method<CBL_E_METHOD(GameWindowFixture,OnWindowDestroyed)>(this);
		gameWindow.OnWindowClosed		+= E::WindowClosed::Method<CBL_E_METHOD(GameWindowFixture,OnWindowClosed)>(this);

		gameWindow.OnWindowKeyChar		+= E::WindowKeyChar::Method<CBL_E_METHOD(GameWindowFixture,OnWindowKeyChar)>(this);
		gameWindow.OnWindowKeyDown		+= E::WindowKeyDown::Method<CBL_E_METHOD(GameWindowFixture,OnWindowKeyDown)>(this);
		gameWindow.OnWindowKeyUp		+= E::WindowKeyUp::Method<CBL_E_METHOD(GameWindowFixture,OnWindowKeyUp)>(this);

		gameWindow.OnWindowMouseWheel	+= E::WindowMouseWheel::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseWheel)>(this);
		gameWindow.OnWindowMouseMove	+= E::WindowMouseMove::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseMove)>(this);
		gameWindow.OnWindowMouseDown	+= E::WindowMouseDown::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseDown)>(this);
		gameWindow.OnWindowMouseUp		+= E::WindowMouseUp::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseUp)>(this);
		
		gameWindow.OnWindowMouseEnter	+= E::WindowMouseEnter::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseEnter)>(this);
		gameWindow.OnWindowMouseLeave	+= E::WindowMouseLeave::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseLeave)>(this);
	}

	void TearDown()
	{
		gameWindow.OnWindowMouseLeave	+= E::WindowMouseLeave::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseLeave)>(this);
		gameWindow.OnWindowMouseEnter	+= E::WindowMouseEnter::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseEnter)>(this);
		
		gameWindow.OnWindowMouseUp		+= E::WindowMouseUp::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseUp)>(this);
		gameWindow.OnWindowMouseDown	+= E::WindowMouseDown::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseDown)>(this);
		gameWindow.OnWindowMouseMove	+= E::WindowMouseMove::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseMove)>(this);
		gameWindow.OnWindowMouseWheel	+= E::WindowMouseWheel::Method<CBL_E_METHOD(GameWindowFixture,OnWindowMouseWheel)>(this);
		
		gameWindow.OnWindowKeyUp		+= E::WindowKeyUp::Method<CBL_E_METHOD(GameWindowFixture,OnWindowKeyUp)>(this);
		gameWindow.OnWindowKeyDown		+= E::WindowKeyDown::Method<CBL_E_METHOD(GameWindowFixture,OnWindowKeyDown)>(this);
		gameWindow.OnWindowKeyChar		+= E::WindowKeyChar::Method<CBL_E_METHOD(GameWindowFixture,OnWindowKeyChar)>(this);
		
		gameWindow.OnWindowClosed		+= E::WindowClosed::Method<CBL_E_METHOD(GameWindowFixture,OnWindowClosed)>(this);
		gameWindow.OnWindowDestroyed	+= E::WindowDestroyed::Method<CBL_E_METHOD(GameWindowFixture,OnWindowDestroyed)>(this);
		gameWindow.OnWindowCreated		+= E::WindowCreated::Method<CBL_E_METHOD(GameWindowFixture,OnWindowCreated)>(this);
	}


	void OnWindowCreated( void )
	{
		Stdout << "[GameWindow] Window created.\n";
		windowCreated = true;
	}
	
	void OnWindowDestroyed( void )
	{
		Stdout << "[GameWindow] Window destroyed.\n";
		windowDestroyed = true;
	}
	
	void OnWindowClosed( void )
	{
		Stdout << "[GameWindow] Window closed.\n";
	}
	
	void OnWindowKeyChar( cbl::Uint32 unicode )
	{
		Stdout << static_cast< char >( unicode );
	}

	void OnWindowKeyDown( Key::Code keyCode )
	{
		Stdout << "[GameWindow] Key down:" << keyCode << std::endl;
	}
	
	void OnWindowKeyUp( Key::Code keyCode )
	{
		Stdout << "[GameWindow] Key up:" << keyCode << std::endl;
	}
	
	void OnWindowMouseWheel( cbl::Int32 delta )
	{
		Stdout << "[GameWindow] Mouse wheel move: " << delta << std::endl;
	}
	
	void OnWindowMouseMove( cbl::Int32 x, cbl::Int32 y )
	{
		++mouseMoveCount;
	}
	
	void OnWindowMouseDown( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
	{
		Stdout << "[GameWindow] MouseDown (";
		switch( button )
		{
		case Mouse::Left:
			Stdout << "Left";
			break;
		case Mouse::Right:
			Stdout << "Right";
			break;
		case Mouse::Middle:
			Stdout << "Middle";
			break;
		case Mouse::XButton1:
			Stdout << "Xbutton1";
			break;
		case Mouse::XButton2:
			Stdout << "Xbutton2";
			break;
		}
		Stdout << "): (" <<x << ", " << y << ")" << std::endl;
	}
	
	void OnWindowMouseUp( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
	{
		Stdout << "[GameWindow] MouseUp (";
		switch( button )
		{
		case Mouse::Left:
			Stdout << "Left";
			break;
		case Mouse::Right:
			Stdout << "Right";
			break;
		case Mouse::Middle:
			Stdout << "Middle";
			break;
		case Mouse::XButton1:
			Stdout << "Xbutton1";
			break;
		case Mouse::XButton2:
			Stdout << "Xbutton2";
			break;
		}
		Stdout << "): (" << x << ", " << y << ")" << std::endl;
	}
	
	void OnWindowMouseEnter( void )
	{
		Stdout << "[GameWindow] Mouse entered." << std::endl;
	}

	void OnWindowMouseLeave( void )
	{
		Stdout << "[GameWindow] Mouse left." << std::endl;
	}

protected:

	cbl::Game			dummyGame;
	cbl::Stopwatch		stopwatch;
	cbl::Stopwatch		lapwatch;
	GameWindow			gameWindow;
	cbl::Float64		timeout;
	cbl::Float64		fullscreenTimeout;

	bool				windowCreated;
	bool				windowDestroyed;
	cbl::Uint32			mouseMoveCount;
};

TEST_F( GameWindowFixture, GameWindow_CreateDestroy )
{
	ASSERT_FALSE( windowCreated );
	gameWindow.Initialise();
	gameWindow.SetTitle( "HEYHEYHEY!" );
	ASSERT_TRUE( windowCreated );

	stopwatch.Start();
	while( stopwatch.GetElapsedTime().TotalSeconds() < timeout )
	{
		gameWindow.Update( cbl::GameTime() );
	}
	stopwatch.Stop();
	
	Stdout << "[GameWindow] Mouse move events: " << mouseMoveCount << std::endl;
	
	ASSERT_FALSE( windowDestroyed );
	gameWindow.Shutdown();
	ASSERT_TRUE( windowDestroyed );
}

TEST_F( GameWindowFixture, GameWindow_Fullscreen )
{
	ASSERT_FALSE( windowCreated );
	gameWindow.Initialise();
	ASSERT_TRUE( windowCreated );

	stopwatch.Start();

	gameWindow.SetFullscreen( true );
	while( stopwatch.GetElapsedTime().TotalSeconds() < fullscreenTimeout )
	{
		gameWindow.Update( cbl::GameTime() );
	}

	stopwatch.Stop();
	stopwatch.Reset();
	stopwatch.Start();

	gameWindow.SetFullscreen( false );
	while( stopwatch.GetElapsedTime().TotalSeconds() < fullscreenTimeout )
	{
		gameWindow.Update( cbl::GameTime() );
	}

	stopwatch.Stop();
	
	Stdout << "[GameWindow] Mouse move events: " << mouseMoveCount << std::endl;

	gameWindow.SetWindowSettings( GameWindowSettings() ); // Reset the window to the default settings.
	ASSERT_FALSE( windowDestroyed );
	gameWindow.Shutdown();
	ASSERT_TRUE( windowDestroyed );
}

TEST_F( GameWindowFixture, GameWindow_BestWindowSize )
{
	gameWindow.SetResolution( 1920, 1080, 16 );
	//gameWindow.SetFullscreen( true );1
	cbl::Uint32 width = 1024;
	cbl::Uint32 height = 768;
	ASSERT_FALSE( windowCreated );
	gameWindow.Initialise();
	ASSERT_TRUE( windowCreated );

	stopwatch.Start();
	lapwatch.Start();
	while( stopwatch.GetElapsedTime().TotalSeconds() < timeout ) {
		gameWindow.Update( cbl::GameTime() );
	}
	lapwatch.Stop();
	stopwatch.Stop();

	Stdout << "[GameWindow] Mouse move events: " << mouseMoveCount << std::endl;

	ASSERT_FALSE( windowDestroyed );
	gameWindow.Shutdown();
	ASSERT_TRUE( windowDestroyed );
}