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
 * @file KeyboardManager.cpp
 * @brief Keyboard manager component.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Input/KeyboardManager.h"
#include "dbl/Core/GameWindow.h"

// Chewable Headers //
#include <cbl/Debug/Assert.h>
#include <cbl/Debug/Logging.h>
#include <cbl/Core/Game.h>

using namespace dbl;

KeyboardManager::KeyboardManager( cbl::Game & game )
: cbl::GameComponent( game )
, mWindow( NULL )
{
}

KeyboardManager::~KeyboardManager()
{
}

bool KeyboardManager::IsKeyDown( Key::Code code ) const
{
	return mKeys[code];
}

void KeyboardManager::Initialise( void )
{
	mWindow = Game.Services.Get< GameWindow >();
	CBL_ASSERT_TRUE( mWindow );
	
	LOG( "Initialising keyboard manager." );

	mWindow->OnWindowKeyDown	+= E::WindowKey::Method<CBL_E_METHOD( KeyboardManager, OnWindowKeyDown )>(this);
	mWindow->OnWindowKeyUp		+= E::WindowKey::Method<CBL_E_METHOD( KeyboardManager, OnWindowKeyUp )>(this);
	mWindow->OnWindowKeyChar	+= E::WindowKeyChar::Method<CBL_E_METHOD( KeyboardManager, OnWindowKeyChar )>(this);
	mWindow->OnWindowLostFocus	+= E::Window::Method<CBL_E_METHOD( KeyboardManager, OnWindowLostFocus )>(this);
}

void KeyboardManager::Shutdown( void )
{
	if( !mWindow )
		return;

	LOG( "Shutting down keyboard manager." );
	
	mWindow->OnWindowLostFocus	-= E::Window::Method<CBL_E_METHOD( KeyboardManager, OnWindowLostFocus )>(this);
	mWindow->OnWindowKeyChar	-= E::WindowKeyChar::Method<CBL_E_METHOD( KeyboardManager, OnWindowKeyChar )>(this);
	mWindow->OnWindowKeyUp		-= E::WindowKey::Method<CBL_E_METHOD( KeyboardManager, OnWindowKeyUp )>(this);
	mWindow->OnWindowKeyDown	-= E::WindowKey::Method<CBL_E_METHOD( KeyboardManager, OnWindowKeyDown )>(this);

	mWindow = NULL;
}

cbl::Uint16 _getKeyMod( const std::bitset<Key::Count>& keyList ) 
{
	return ( keyList[ Key::RShift ] || keyList[ Key::LShift ] ? Key::Modifier::Shift : 0 ) |
		( keyList[ Key::RAlt ] || keyList[ Key::LAlt ] ? Key::Modifier::Alt : 0 ) |
		( keyList[ Key::RCtrl ] || keyList[ Key::LCtrl ] ? Key::Modifier::Ctrl : 0 );
}

void KeyboardManager::Update( const cbl::GameTime & )
{
	FilterEvent* f = Filter.Top();

	for( cbl::Uint32 count = 0; count < Key::Count; ++count )
	{
		if( mKeys[count] )
			OnKeyDown( Key::Code( count ), _getKeyMod( mKeys ) );
		if( f && f->Keys[ count ] )
			f->OnKeyDown( Key::Code( count ), _getKeyMod( f->Keys ) );
		for( size_t i = 0; i < Filter.size(); ++i ) {
			if( &Filter[i] == f )
				break;
			if( Filter[i].Keys[count] ) {
				// We have an underlying state that needs to be set to key up.
				Filter[i].Keys[count] = false;
				Filter[i].OnKeyUp( Key::Code( count ), _getKeyMod( Filter[i].Keys ) );
			}
		}
	}
}

void KeyboardManager::OnWindowKeyDown( Key::Code keyCode )
{
	if( !mKeys[ keyCode ] ) {
		mKeys[ keyCode ] = true;
		OnKeyClick( keyCode, _getKeyMod( mKeys ) );
	}

	FilterEvent* f = Filter.Top();
	if( f && !f->Keys[ keyCode ] ) {
		f->Keys[ keyCode ] = true;
		f->OnKeyClick( keyCode, _getKeyMod( f->Keys ) );
	}
}

void KeyboardManager::OnWindowKeyUp( Key::Code keyCode )
{
	if( mKeys[ keyCode ] ) {
		mKeys[ keyCode ] = false;
		OnKeyUp( keyCode, _getKeyMod( mKeys ) );
	}

	FilterEvent* f = Filter.Top();
	if( f && f->Keys[ keyCode ] ) {
		f->Keys[ keyCode ] = false;
		f->OnKeyUp( keyCode, _getKeyMod( f->Keys ) );
	}
}

void KeyboardManager::OnWindowKeyChar( cbl::Uint32 unicode )
{
	OnKeyChar( unicode );

	if( FilterEvent* f = Filter.Top() )
		f->OnKeyChar( unicode );
}

void KeyboardManager::OnWindowLostFocus( void )
{
	for( cbl::Uint32 count = 0; count < Key::Count; ++count )
	{
		if( mKeys[ count ] ) {
			mKeys[ count ] = false;
			OnKeyUp( Key::Code( count ), _getKeyMod( mKeys ) );
		}

		for( size_t i = 0; i < Filter.size(); ++i ) {
			if( Filter[i].Keys[ count ] ) {
				Filter[i].Keys[ count ] = false;
				Filter[i].OnKeyUp( Key::Code( count ), _getKeyMod( Filter[i].Keys ) );
			}
		}
	}
}
