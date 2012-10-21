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
 * @file GameWindow.h
 * @brief Application window.
 */

#ifndef __DBL_GAMEWINDOW_H_
#define __DBL_GAMEWINDOW_H_

// Chewable Headers //
#include <cbl/Chewable.h>
#include <cbl/Core/Event.h>
#include <cbl/Core/GameComponent.h>
#include <cbl/Core/Services.h>
#include <cbl/Core/GameTime.h>
#include <cbl/Util/Property.h>
#include <cbl/Math/Vector2.h>

// Delectable Headers //
#include "dbl/Delectable.h"
#include "dbl/Core/GameWindowSettings.h"
#include "dbl/Input/KeyCodes.h"
#include "dbl/Input/MouseButtons.h"
#include "dbl/Reflection/YAMLSerialiser.h"
#include "dbl/Reflection/YAMLDeserialiser.h"

namespace dbl
{
	// Forward Declarations //
	class IPlatformWindow;

	namespace E
	{
		/***** Window events *****/
#ifdef CBL_TPLFUNCTION_PREFERRED_SYNTAX
		typedef cbl::Event<void()>																		Window;				//!< Parameterless window event.
		typedef cbl::Event<void(cbl::Uint32,cbl::Uint32,cbl::Uint32,cbl::Uint32,cbl::Uint16,cbl::Real)>	WindowSize;			//!< params: Resolution X, Resolution Y, Window X, Window Y, BPP, Window aspect ratio
		typedef cbl::Event<void(bool,bool)>																WindowFullScreen;	//!< params: Fullscreen, Error
		typedef cbl::Event<void(::dbl::Key::Code)>														WindowKey;			//!< params: KeyCode
		typedef cbl::Event<void(cbl::Uint32)>															WindowKeyChar;		//!< params: Unicode
		typedef cbl::Event<void(cbl::Int32,cbl::Int32)>													WindowMouseMove;	//!< params: X, Y
		typedef cbl::Event<void(cbl::Int32,cbl::Int32,::dbl::Mouse::Button)>							WindowMouseButton;	//!< params: X, Y, Button
		typedef cbl::Event<void(cbl::Int32)>															WindowMouseWheel;	//!< params: Delta
		typedef cbl::Event<void(cbl::Int32,cbl::Int32,const cbl::FileInfo::List&)>						WindowDragDrop;		//!< params: X, Y, File names, File count.
#else
		typedef cbl::Event0<void>																		Window;				//!< Parameterless window event.
		typedef cbl::Event2<void,cbl::Uint32,cbl::Uint32,cbl::Uint32,cbl::Uint32,cbl::Uint16,cbl::Real>	WindowSize;			//!< params: Resolution X, Resolution Y, Window X, Window Y, BPP, Window aspect ratio
		typedef cbl::Event2<void,bool,bool>																WindowFullScreen;	//!< params: Fullscreen, Error
		typedef cbl::Event1<void,::dbl::Key::Code>														WindowKey;			//!< params: KeyCode
		typedef cbl::Event1<void,cbl::Uint32>															WindowKeyChar;		//!< params: Unicode
		typedef cbl::Event2<void,cbl::Int32,cbl::Int32>													WindowMouseMove;	//!< params: X, Y
		typedef cbl::Event3<void,cbl::Int32,cbl::Int32,::dbl::Mouse::Button>							WindowMouseButton;	//!< params: X, Y, Button
		typedef cbl::Event1<void,cbl::Int32>															WindowMouseWheel;	//!< params: Delta
		typedef cbl::Event3<void,cbl::Int32,cbl::Int32,const cbl::FileInfo::List&>						WindowDragDrop;		//!< params: X, Y, File names, File count.
#endif
		// Better naming
		typedef Window				WindowCreated;
		typedef Window				WindowDestroyed;
		typedef Window				WindowClosed;
		typedef Window				WindowLostFocus;
		typedef Window				WindowGainedFocus;
		typedef Window				WindowMouseEnter;
		typedef Window				WindowMouseLeave;
		typedef WindowMouseButton	WindowMouseUp;
		typedef WindowMouseButton	WindowMouseDown;
		typedef WindowMouseButton	WindowMouseDblClick;
		typedef WindowKey			WindowKeyUp;
		typedef WindowKey			WindowKeyDown;
	}

	//! @brief Main application window object which wraps the OS-dependent implementation.
	//! The game window's update order is set to always be the first to
	//! be updated in a game application.
	class DBL_API GameWindow :
		public cbl::GameComponent
	{
	/***** Public Static Methods *****/
	public:
		static void GetWorkableArea( cbl::Uint32& left, cbl::Uint32& top, cbl::Uint32& right, cbl::Uint32& bottom, GameWindowStyle& style );
		static void GetWorkableAreaCenter( cbl::Uint32& x, cbl::Uint32& y, GameWindowStyle& style );
		static void GetBestClientWindowDimensions( cbl::Uint32 targetw, cbl::Uint32 targeth, GameWindowStyle& style, cbl::Uint32& bestw, cbl::Uint32& besth );
		static cbl::Uint32 GetSystemWindowStyle( GameWindowStyle& style );

	/***** Types *****/
	public:
		//! Window size list.
		typedef std::vector< GameWindowSize >		GameWindowSizeList;

	/***** Properties *****/
	public:
		//! Game window settings.
		GETTER_AUTO_CREF( GameWindowSettings, Settings );
		//! Current cursor resource.
		GETTER_AUTO_CREF( cbl::String, CursorResource );
		//! Get the list of available window sizes.
		GETTER_AUTO_CREF( GameWindowSizeList, AvailableResolutions );
		//! Get the current desktop resolution
		GETTER_AUTO_CREF( GameWindowSize, DesktopResolution );
		//! Get game window handle.
		GameWindowHandle GetGameWindowHandle();
		//! Check if supplied resolution is valid.
		bool IsValidResolution( cbl::Uint32 width, cbl::Uint32 height ) const;

		inline const GameWindowSize& GetWindowDimensions( void ) const { return mSettings.Dimensions; }
		inline const GameWindowSize& GetResolution( void ) const { return mSettings.Resolution; }
		inline const bool IsFullscreen( void ) const { return mSettings.Style.Fullscreen; }
		inline const cbl::Real GetAspectRatio( void ) const { return mSettings.AspectRatio; }

	/***** Constants *****/
	public:
		static cbl::FileInfo	sSettingsFile;		//!< Default xml file to save/load settings.
		static cbl::Uint32		sMinimumResolutionX;//!< Default resolution width is 640
		static cbl::Uint32		sMinimumResolutionY;//!< Default resolution width is 480.
		static bool				sDefaultFullscreen;	//!< Defaults resolution to fullscreen if no window settings applied.

	/***** Events *****/
	public:
		E::WindowSize			OnWindowResized;	//!< Triggered when window is resized.
		E::WindowCreated		OnWindowCreated;	//!< Triggered when window is created.
		E::WindowDestroyed		OnWindowDestroyed;	//!< Triggered when window is destroyed.
		E::WindowClosed			OnWindowClosed;		//!< Triggered when window is closed.
		E::WindowLostFocus		OnWindowLostFocus;	//!< Triggered when window is loses focus.
		E::WindowGainedFocus	OnWindowGainedFocus;//!< Triggered when window is gains focus.
		E::WindowFullScreen		OnWindowFullscreen;	//!< Triggered when window toggles fullscreen mode.
		
		E::WindowKeyDown		OnWindowKeyDown;	//!< Triggered when a key is pressed down.
		E::WindowKeyUp			OnWindowKeyUp;		//!< Triggered when a key is released.
		E::WindowKeyChar		OnWindowKeyChar;	//!< Triggered when a key character is pressed.
		
		E::WindowMouseMove		OnWindowMouseMove;	//!< Triggered when mouse is moved.
		E::WindowMouseDblClick	OnWindowMouseDblClick;	//!< Trigger when mouse is double clicked.
		E::WindowMouseDown		OnWindowMouseDown;	//!< Triggered when mouse button is pressed down.
		E::WindowMouseUp		OnWindowMouseUp;	//!< Triggered when mouse button is released.
		E::WindowMouseWheel		OnWindowMouseWheel;	//!< Triggered when mouse wheel is moved.
		
		E::WindowMouseEnter		OnWindowMouseEnter;	//!< Triggered when mouse enters the window.
		E::WindowMouseLeave		OnWindowMouseLeave;	//!< Triggered when mouse leaves the window.

		E::WindowDragDrop		OnWindowDragDrop;	//!< Triggered when a file is dragged into the window.

	/***** Public Methods *****/
	public:
		//! Constructor.
		//! @param	game		Pointer to game.
		explicit GameWindow( cbl::Game & game );
		//! Constructor.
		//! @param	game		Pointer to game.
		//! @param	settings	Game window settings.
		GameWindow( cbl::Game & game, const GameWindowSettings & settings );
		//! Destructor.
		//! Automatically destroys any existing window.
		~GameWindow();
		//! Initialise and create the game window.
		virtual void Initialise( void );
		//! Shutdown and destroy the game window.
		virtual void Shutdown( void );
		//! Update the game window.
		virtual void Update( const cbl::GameTime & time );
		//! Set the window settings.
		//! @param	settings	New window settings.
		void SetWindowSettings( const GameWindowSettings & settings );
		//! Center window position.
		void CenterPosition( void );
		//! Set window size.
		//! @param	size		Game window dimensions.
		void SetResolution( cbl::Uint32 width, cbl::Uint32 height, cbl::Uint16 bpp );
		//! Set window title.
		//! @param	title		New window title.
		void SetTitle( const cbl::Char * const title );
		//! Set window style.
		//! @param	close		Enable window close button.
		//! @param	minimize	Enable window minimize button.
		void SetStyle( bool close, bool minimize );
		//! Set fullscreen window toggle.
		//! @param	state		Fullscreen state.
		void SetFullscreen( bool state );
		//! Show game window.
		//! @param	state		Visibility state.
		void Show( bool state );
		//! Show window cursor.
		//! @param	state		Cursor visibility state.
		void ShowCursor( bool state );
		//! Load and set cursor to the indicated resource file.
		void SetCursor( const cbl::Char* filename );
		//! Set cursor position relative to window.
		void SetCursorPosition( cbl::Int32 x, cbl::Int32 y );
		//! Get cursor position relative to window.
		void GetCursorPosition( cbl::Int32 & x, cbl::Int32 & y );
		//! Center cursor position to window center.
		void CenterCursorPosition( void );
		//! Clip cursor to window bounds.
		//! @param	clip		Clip state.
		void ClipCursor( bool state );
		//! Set whether the drag and drop works.
		void SetAcceptDragDrop( bool state );
		//! Save window settings.
		template< typename SERIALISER_TYPE >
		void SaveSettings( const cbl::Char* fileName ) const;
		//! Load window settings.
		template< typename DESERIALISER_TYPE >
		void LoadSettings( const cbl::Char* fileName );
		
	/***** Private Methods *****/
	private:
		//! Populate our available resolution list.
		void PopulateResolutions( void );
		//! Set resolution to the current desktop resolution.
		void SetCurrentDesktopResolution( void );
		//! Default to desktop resolution and fullscreen if flag is set.
		void DefaultToFullscreen( void );

	/***** Private Members *****/
	private:
		IPlatformWindow			* mPlatformWindow;		//!< Platform-specific window implementation.
		GameWindowSettings		mSettings;				//!< Game window settings.
		cbl::String				mCursorResource;		//!< Current cursor resource.
		GameWindowSizeList		mAvailableResolutions;	//!< Available screen resolutions.
		GameWindowSize			mDesktopResolution;		//!< The desktop resolution.
	};

	template<>
	DBL_API void GameWindow::SaveSettings<YAMLSerialiser>( const cbl::Char* fileName ) const;

	template<>
	DBL_API void GameWindow::LoadSettings<YAMLDeserialiser>( const cbl::Char* fileName );
}

CBL_TYPE( dbl::GameWindow, GameWindow );

#endif // __DBL_GAMEWINDOW_H_
