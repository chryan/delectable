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
#include "dbl/Input/MouseManager.h"
#include "dbl/Core/GameWindow.h"

// Chewable Headers //
#include <cbl/Debug/Assert.h>
#include <cbl/Debug/Logging.h>
#include <cbl/Core/Game.h>

using namespace dbl;

MouseManager::MouseManager( cbl::Game & game )
: cbl::GameComponent( game )
, mWindow( NULL )
, mClipCursor( false )
, mLockMouse( false )
, mFocused( true )
, mCentering( false )
{
}

MouseManager::~MouseManager()
{
}

bool MouseManager::IsButtonDown( Mouse::Button button ) const
{
	return mButtons[button];
}

void MouseManager::Initialise( void )
{
	mWindow = Game.Services.Get< GameWindow >();
	CBL_ASSERT_TRUE( mWindow );
	
	LOG( "Initialising mouse manager." );
	
	mWindow->OnWindowLostFocus		+= E::WindowLostFocus::Method<CBL_E_METHOD(MouseManager,OnWindowLostFocus)>(this);
	mWindow->OnWindowGainedFocus	+= E::WindowGainedFocus::Method<CBL_E_METHOD(MouseManager,OnWindowGainedFocus)>(this);
	mWindow->OnWindowMouseMove		+= E::WindowMouseMove::Method<CBL_E_METHOD(MouseManager,OnWindowMouseMove)>(this);
	mWindow->OnWindowMouseDown		+= E::WindowMouseDown::Method<CBL_E_METHOD(MouseManager,OnWindowMouseDown)>(this);
	mWindow->OnWindowMouseUp		+= E::WindowMouseUp::Method<CBL_E_METHOD(MouseManager,OnWindowMouseUp)>(this);
	mWindow->OnWindowMouseDblClick	+= E::WindowMouseDblClick::Method<CBL_E_METHOD(MouseManager,OnWindowMouseDblClick)>(this);
	mWindow->OnWindowMouseWheel		+= E::WindowMouseWheel::Method<CBL_E_METHOD(MouseManager,OnWindowMouseWheel)>(this);
	mWindow->OnWindowMouseLeave		+= E::WindowMouseLeave::Method<CBL_E_METHOD(MouseManager,OnWindowMouseLeave)>(this);
}

void MouseManager::Shutdown( void )
{
	if( !mWindow )
		return;

	LOG( "Shutting down mouse manager." );
	
	mWindow->OnWindowMouseLeave		-= E::WindowMouseLeave::Method<CBL_E_METHOD(MouseManager,OnWindowMouseLeave)>(this);
	mWindow->OnWindowMouseWheel		-= E::WindowMouseWheel::Method<CBL_E_METHOD(MouseManager,OnWindowMouseWheel)>(this);
	mWindow->OnWindowMouseDblClick	-= E::WindowMouseDblClick::Method<CBL_E_METHOD(MouseManager,OnWindowMouseDblClick)>(this);
	mWindow->OnWindowMouseUp		-= E::WindowMouseUp::Method<CBL_E_METHOD(MouseManager,OnWindowMouseUp)>(this);
	mWindow->OnWindowMouseDown		-= E::WindowMouseDown::Method<CBL_E_METHOD(MouseManager,OnWindowMouseDown)>(this);
	mWindow->OnWindowMouseMove		-= E::WindowMouseMove::Method<CBL_E_METHOD(MouseManager,OnWindowMouseMove)>(this);
	mWindow->OnWindowGainedFocus	-= E::WindowGainedFocus::Method<CBL_E_METHOD(MouseManager,OnWindowGainedFocus)>(this);
	mWindow->OnWindowLostFocus		-= E::WindowLostFocus::Method<CBL_E_METHOD(MouseManager,OnWindowLostFocus)>(this);

	mWindow = NULL;
}

void MouseManager::Update( cbl::GameTime const & )
{
	FilterEvent* f = Filter.Top();

	for( cbl::Uint32 count = 0; count < Mouse::Count; ++count )
	{
		if( mButtons[ count ] )
			OnMouseDown( mPosition.X, mPosition.Y, Mouse::Button( count ) );
		if( f && f->Buttons[ count ] )
			f->OnMouseDown( mPosition.X, mPosition.Y, Mouse::Button( count ) );
		for( size_t i = 0; i < Filter.size(); ++i ) {
			if( &Filter[i] == f )
				break;
			if( Filter[i].Buttons[count] ) {
				// We have an underlying state that needs to be set to false.
				Filter[i].Buttons[count] = false;
				Filter[i].OnMouseUp( mPosition.X, mPosition.Y, Mouse::Button( count ) );
			}
		}
	}
}

void MouseManager::ShowCursor( void )
{
	mShowMouse = true;
	mWindow->ShowCursor( true );
}

void MouseManager::HideCursor( void )
{
	mShowMouse = false;
	mWindow->ShowCursor( false );
}

void MouseManager::SetCursor( const cbl::Char * resource )
{
	mWindow->SetCursor( resource );
}

void MouseManager::SetCursorPosition( cbl::Int32 x, cbl::Int32 y )
{
	mWindow->SetCursorPosition( x, y );
}

void MouseManager::CenterCursorPosition( void )
{
	mWindow->CenterCursorPosition();
}

void MouseManager::ClipCursor( bool state )
{
	mClipCursor = state;
	mWindow->ClipCursor( state );
}

void MouseManager::LockMouse( bool showCursor )
{
	if( mLockMouse ) return;

	mLockMouse = true;

	mStoredPosition = mPosition;
	mPosition.X = mWindow->GetSettings().Dimensions.Width / 2;
	mPosition.Y = mWindow->GetSettings().Dimensions.Height / 2;
	mWindow->CenterCursorPosition();

	if( showCursor )
		ShowCursor();
	else
		HideCursor();
}

void MouseManager::UnlockMouse( void )
{
	if( !mLockMouse ) return;

	mLockMouse = false;
	ShowCursor();

	mPosition = mStoredPosition;
	mWindow->SetCursorPosition( mStoredPosition.X, mStoredPosition.Y );
}

void MouseManager::OnWindowLostFocus( void )
{
	if( mClipCursor )
		mWindow->ClipCursor( false );

	mFocused = false;
	
	mWindow->ShowCursor( true );
}

void MouseManager::OnWindowGainedFocus( void )
{
	if( mClipCursor )
		mWindow->ClipCursor( true );

	mFocused = true;
	
	if( mLockMouse )
		mWindow->CenterCursorPosition();
	if( !mShowMouse )
		mWindow->ShowCursor( false );
}

void MouseManager::OnWindowMouseMove( cbl::Int32 x, cbl::Int32 y )
{
	if( mCentering ) {
		mCentering = false;
		mPosition.X = x;
		mPosition.Y = y;
		return;
	}
		
	if( mPosition.X == x && mPosition.Y == y )
		return;
	
	cbl::Int32 diffx = x - mPosition.X;
	cbl::Int32 diffy = y - mPosition.Y;
	OnMouseMove( x, y, diffx, diffy );

	FilterEvent* f = Filter.Top();
	if( f ) f->OnMouseMove( x, y, diffx, diffy );

	for( cbl::Uint32 count = 0; count < Mouse::Count; ++count )
	{
		if( mButtons[ count ] )
			OnMouseDrag( x, y, diffx, diffy, Mouse::Button( count ) );
		if( f && f->Buttons[ count ] )
			f->OnMouseDrag( x, y, diffx, diffy, Mouse::Button( count ) );
	}

	if( mLockMouse && mFocused ) {
		mCentering = true;
		mWindow->CenterCursorPosition();
	}
	else {
		mPosition.X = x;
		mPosition.Y = y;
	}
}

void MouseManager::OnWindowMouseDown( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
{
	if( !mButtons[ button ] ) {
		mButtons[ button ] = true;
		OnMouseClick( x, y, button );
	}

	FilterEvent* f = Filter.Top();
	if( f && !f->Buttons[ button ]  ) {
		f->Buttons[ button ] = true;
		f->OnMouseClick( x, y, button );
	}
}

void MouseManager::OnWindowMouseUp( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
{
	if( mButtons[ button ] ) {
		mButtons[ button ] = false;
		OnMouseUp( x, y, button );
	}

	FilterEvent* f = Filter.Top();
	if( f && f->Buttons[ button ] ) {
		f->Buttons[ button ] = false;
		f->OnMouseUp( x, y, button );
	}
}

void MouseManager::OnWindowMouseDblClick( cbl::Int32 x, cbl::Int32 y, Mouse::Button button )
{
	if( !mButtons[ button ] ) {
		mButtons[ button ] = true;
		OnMouseDblClick( x, y, button );
	}

	FilterEvent* f = Filter.Top();
	if( f && !f->Buttons[ button ]  ) {
		f->Buttons[ button ] = true;
		f->OnMouseDblClick( x, y, button );
	}
}

void MouseManager::OnWindowMouseWheel( cbl::Int32 delta )
{
	OnMouseWheel( delta );

	if( FilterEvent* f = Filter.Top() )
		f->OnMouseWheel( delta );
}

void MouseManager::OnWindowMouseLeave( void )
{
	for( cbl::Uint32 count = 0; count < Mouse::Count; ++count )
	{
		if( mButtons[ count ] ) {
			mButtons[ count ] = false;
			OnMouseUp( mPosition.X, mPosition.Y, Mouse::Button( count ) );
		}

		for( size_t i = 0; i < Filter.size(); ++i ) {
			if( Filter[i].Buttons[ count ] ) {
				Filter[i].Buttons[ count ] = false;
				Filter[i].OnMouseUp( mPosition.X, mPosition.Y, Mouse::Button( count ) );
			}
		}
	}
}
