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
 * @file Game.cpp
 * @brief Delectable engine game class. Includes keyboard, mouse and windowing.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Core/Game.h"
#include "dbl/Core/GameWindow.h"
#include "dbl/Input/KeyboardManager.h"
#include "dbl/Input/MouseManager.h"

// Using 'this' is fine because the components only needs it to store the game reference.
#pragma warning( disable : 4355 )

using namespace dbl;

Game::Game( const cbl::Char * name )
: cbl::Game( name )
, Window( *this )
, Keyboard( *this )
, Mouse( *this )
, Levels( *this )
{
	Components.Add( &Window );
	Components.Add( &Keyboard );
	Components.Add( &Mouse );
	Components.Add( &Levels );

	Services.Add< GameWindow >( &Window );
	Services.Add< KeyboardManager >( &Keyboard );
	Services.Add< MouseManager >( &Mouse );
	Services.Add< LevelManager >( &Levels );
}

Game::~Game()
{
	Services.Remove< LevelManager >();
	Services.Remove< MouseManager >();
	Services.Remove< KeyboardManager >();
	Services.Remove< GameWindow >();

	Components.Remove( &Levels );
	Components.Remove( &Mouse );
	Components.Remove( &Keyboard );
	Components.Remove( &Window );
}