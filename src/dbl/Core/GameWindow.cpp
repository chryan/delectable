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
 * @file GameWindow.cpp
 * @brief Application window.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Core/GameWindow.h"
#include "IPlatformWindow.h"

// Chewable Headers //
#include <cbl/Util/FileSystem.h>

// External Dependencies //
#include <windows.h>

using namespace dbl;

cbl::FileInfo GameWindow::sSettingsFile			= cbl::FileInfo( "window.cfg" );
cbl::Uint32 GameWindow::sMinimumResolutionX		= 640;
cbl::Uint32 GameWindow::sMinimumResolutionY		= 480;
bool GameWindow::sDefaultFullscreen				= false;

GameWindow::GameWindow( cbl::Game & game )
: cbl::GameComponent( game )
, mPlatformWindow( NULL )
, mDesktopResolution(0,0,0)
{
	this->UpdateOrder = INT_MIN; // Ensure that all window events come as early as possible.
}

GameWindow::GameWindow( cbl::Game & game, const GameWindowSettings & settings )
: cbl::GameComponent( game )
, mSettings( settings )
, mPlatformWindow( NULL )
, mDesktopResolution(0,0,0)
{
}

GameWindow::~GameWindow()
{
	CBL_DELETE( mPlatformWindow );
}

GameWindowHandle GameWindow::GetGameWindowHandle()
{
	CBL_ASSERT_TRUE( mPlatformWindow );
	return mPlatformWindow->GetHandle();
}

bool GameWindow::IsValidResolution( cbl::Uint32 width, cbl::Uint32 height ) const
{
	CBL_FOREACH_CONST( GameWindowSizeList, it, mAvailableResolutions ) {
		if( it->Width == width && it->Height == height ) return true;
	}

	return false;
}

void GameWindow::Initialise( void )
{
	PopulateResolutions();
	SetCurrentDesktopResolution();

	CBL_ASSERT( mPlatformWindow == NULL, "Platform window has already been created!" );

	if( !IsValidResolution( mSettings.Resolution.Width, mSettings.Resolution.Height ) )
		DefaultToFullscreen();

	mPlatformWindow = IPlatformWindow::Create( this, mSettings );

	OnWindowCreated();
}

void GameWindow::Shutdown( void )
{
	CBL_ASSERT_TRUE( mPlatformWindow );
	//XMLTools<GameWindowSettings>::Export( "GameWindowSettings", mSettings, sSettingsFile.GetFullFileName().c_str() );
	LOG( sSettingsFile.GetFile() << " settings file saved." );

	OnWindowDestroyed();

	CBL_DELETE( mPlatformWindow );
}

void GameWindow::Update( const cbl::GameTime & )
{
	CBL_ASSERT_TRUE( mPlatformWindow );
	mPlatformWindow->ProcessWindowEvents();
}

void GameWindow::SetWindowSettings( const GameWindowSettings & settings )
{
	CBL_ASSERT_TRUE( mPlatformWindow );

	mSettings = settings;
	mPlatformWindow->DoSetSizePosition();
	mPlatformWindow->DoSetTitle();
	mPlatformWindow->DoSetFullscreen();
	mPlatformWindow->DoSetStyle();
}

void GameWindow::CenterPosition( void )
{
	CBL_ASSERT_TRUE( mPlatformWindow );
	mPlatformWindow->DoSetSizePosition();
}

void GameWindow::SetResolution( cbl::Uint32 width, cbl::Uint32 height, cbl::Uint16 bpp )
{
	if( mPlatformWindow ){
		if( IsValidResolution( width, height ) ) {
			mSettings.Resolution.Width			= width;
			mSettings.Resolution.Height			= height;
			mSettings.Resolution.BitsPerPixel	= bpp;
			mPlatformWindow->DoSetSizePosition();
		}
	}
	else {
		mSettings.Resolution.Width	= width;
		mSettings.Resolution.Height	= height;
	}
}

void GameWindow::SetTitle( const cbl::Char * const title )
{
	mSettings.Title = title;
	if( mPlatformWindow )
		mPlatformWindow->DoSetTitle();
}

void GameWindow::SetStyle( bool close, bool minimize )
{
	mSettings.Style.Close = close;
	mSettings.Style.Minimize = minimize;
	if( mPlatformWindow )
		mPlatformWindow->DoSetStyle();
}

void GameWindow::SetFullscreen( bool state )
{
	if( mSettings.Style.Fullscreen == state )
		return;

	mSettings.Style.Fullscreen = state;
	
	if( mPlatformWindow )
		mPlatformWindow->DoSetFullscreen();
}

void GameWindow::Show( bool state )
{
	CBL_ASSERT_TRUE( mPlatformWindow );
	mPlatformWindow->Show( state );
}

void GameWindow::ShowCursor( bool state )
{
	CBL_ASSERT_TRUE( mPlatformWindow );
	mPlatformWindow->ShowCursor( state );
}

void GameWindow::SetCursor( const cbl::Char * file )
{
	if( file == NULL ) {
		mPlatformWindow->SetCursor( NULL );
		mCursorResource = "";
	}
	else {
		if( !cbl::FileSystem::FileCheckExists( file ) ) return;
	
		mPlatformWindow->SetCursor( file );
		mCursorResource = file;
	}
}

void GameWindow::SetCursorPosition( cbl::Int32 x, cbl::Int32 y )
{
	mPlatformWindow->SetCursorPosition( x, y );
}

void GameWindow::CenterCursorPosition( void )
{
	mPlatformWindow->SetCursorPosition( mSettings.Dimensions.Width / 2, mSettings.Dimensions.Height / 2 );
}

void GameWindow::ClipCursor( bool state )
{
	mPlatformWindow->ClipCursor( state );
}

void GameWindow::SetAcceptDragDrop( bool state )
{
	mPlatformWindow->SetAcceptDragDrop( state );
}

template<>
void GameWindow::SaveSettings<YAMLSerialiser>( const cbl::Char* fileName ) const
{
	YAML::Emitter e;
	dbl::YAMLSerialiser s;

	s.SetStream( e )
		.Serialise( mSettings );
	s.Output( fileName );
}

template<>
void GameWindow::LoadSettings<YAMLDeserialiser>( const cbl::Char* fileName )
{
	bool def = false;
	try {
		std::ifstream windowFile( fileName );
		YAML::Parser parser( windowFile );

		dbl::YAMLDeserialiser yaml;
		yaml.SetStream( parser );
		def = !yaml.Deserialise( mSettings );
	} catch( const YAML::Exception& ) {
		def = true;
	}

	if( def ) {
		LOG( cbl::LogLevel::Warning << "Unable to load window.cfg file." << ( sDefaultFullscreen ? "Defaulting to desktop resolution." : "" )  );
		DefaultToFullscreen();
	}
}

void GameWindow::DefaultToFullscreen( void )
{
	mSettings.Resolution = GetDesktopResolution();
	if( sDefaultFullscreen ) {
		mSettings.Style.Fullscreen = true;
	}
}
