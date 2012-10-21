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
 * @file MouseManager.h
 * @brief Mouse manager component.
 */

#ifndef __DBL_MOUSEMANAGER_H_
#define __DBL_MOUSEMANAGER_H_

// Chewable Headers //
#include <cbl/Chewable.h>
#include <cbl/Core/GameComponent.h>
#include <cbl/Core/Event.h>
#include <cbl/Core/Services.h>
#include <cbl/Math/Vector2.h>
#include <cbl/Util/Property.h>

// Delectable Headers //
#include "dbl/Delectable.h"
#include "dbl/Input/MouseButtons.h"
#include "dbl/Input/InputFilter.h"

// External Dependencies //
#include <bitset>

namespace dbl
{
	namespace E
	{
		/***** Mouse events *****/
#ifdef CBL_TPLFUNCTION_PREFERRED_SYNTAX
		typedef cbl::Event<void(cbl::Int32,cbl::Int32,cbl::Int32,cbl::Int32)>						MouseMove;		//!< params: X, Y, DiffX, DiffY
		typedef cbl::Event<void(cbl::Int32,cbl::Int32,cbl::Int32,cbl::Int32,::dbl::Mouse::Button)>	MouseDrag;		//!< params: X, Y, DiffX, DiffY
		typedef cbl::Event<void(cbl::Int32,cbl::Int32,::dbl::Mouse::Button)>						MouseButton;	//!< params: X, Y, Button
		typedef cbl::Event<void(cbl::Int32)>														MouseWheel;		//!< params: Delta
#else
		typedef cbl::Event2<void,cbl::Int32,cbl::Int32,cbl::Int32,cbl::Int32>						MouseMove;		//!< params: X, Y, DiffX, DiffY
		typedef cbl::Event5<void,cbl::Int32,cbl::Int32,cbl::Int32,cbl::Int32,::dbl::Mouse::Button>	MouseDrag;		//!< params: X, Y, DiffX, DiffY
		typedef cbl::Event3<void,cbl::Int32,cbl::Int32,::dbl::Mouse::Button>						MouseButton;	//!< params: X, Y, Button
		typedef cbl::Event1<void,cbl::Int32>														MouseWheel;		//!< params: Delta
#endif
		typedef MouseButton		MouseDown;
		typedef MouseButton		MouseUp;
		typedef MouseButton		MouseClick;
		typedef MouseButton		MouseDblClick;
	}

	namespace MouseLockType
	{
		enum Options
		{
			None,	//!< No locking
			Window,	//!< Lock to window size.
			Center,	//!< Lock to center (for FPS controls)
		};
	}
	//! @brief Mouse management component.
	//! Register listeners with the mouse manager events to respond to mouse input.
	//! This class depends on a GameWindow service existing in the game application.
	class DBL_API MouseManager :
		public cbl::GameComponent
	{
	/***** Private Types *****/
	private:
		//! Button list type.
		typedef std::bitset< Mouse::Count >	ButtonList;

	/***** Types *****/
	public:
		//! Filtered mouse event.
		struct FilterEvent
		{
			ButtonList			Buttons;		//!< Mouse button states.
			E::MouseMove		OnMouseMove;	//!< Mouse move event.
			E::MouseDrag		OnMouseDrag;	//!< Mouse drag event.
			E::MouseButton		OnMouseDown;	//!< Mouse down event.
			E::MouseUp			OnMouseUp;		//!< Mouse up event.
			E::MouseClick		OnMouseClick;	//!< Mouse click event.
			E::MouseDblClick	OnMouseDblClick;//!< Mouse double click event.
			E::MouseWheel		OnMouseWheel;	//!< Mouse wheel event.
			inline bool IsButtonDown( Mouse::Button button ) const { return Buttons[button]; }
		};
		//! Filtered mouse event map.
		typedef InputFilter<FilterEvent>	FilterEvents;

	/***** Properties *****/
	public:
		//! Get mouse position.
		GETTER_AUTO_CREF( cbl::Vector2i, Position );
		//! Get mouse position vector pointer.
		const cbl::Vector2i * GetPositionPtr( void ) const { return &mPosition; }
		//! Get mouse position vector copy.
		const cbl::Vector2i GetPositionCopy( void ) const { return mPosition; }
		//! Is button down?
		//! @param	button	Button to check.
		bool IsButtonDown( Mouse::Button button ) const;
		//! Check if cursor is clipped.
		GETTER_AUTO( bool, ClipCursor );

	/***** Events *****/
	public:
		FilterEvents		Filter;			//!< Filtered mouse event map.
		E::MouseMove		OnMouseMove;	//!< Mouse move event.
		E::MouseDrag		OnMouseDrag;	//!< Mouse drag event.
		E::MouseButton		OnMouseDown;	//!< Mouse down event.
		E::MouseUp			OnMouseUp;		//!< Mouse up event.
		E::MouseClick		OnMouseClick;	//!< Mouse click event.
		E::MouseDblClick	OnMouseDblClick;//!< Mouse double click event.
		E::MouseWheel		OnMouseWheel;	//!< Mouse wheel event.

	/***** Public Methods *****/
	public:
		//! Constructor.
		//! @param	game		Pointer to game.
		explicit MouseManager( cbl::Game & game );
		//! Destructor.
		virtual ~MouseManager();
		//! Pure virtual function to initialise component.
		virtual void Initialise( void );
		//! Pure virtual function to shut down component.
		virtual void Shutdown( void );
		//! Pure virtual update function (from IUpdatable).
		virtual void Update( const cbl::GameTime & time );
		//! Show mouse cursor.
		void ShowCursor( void );
		//! Hide mouse cursor.
		void HideCursor( void );
		//! Load and set cursor.
		void SetCursor( const cbl::Char * resource );
		//! Set mouse x/y position relative to window.
		void SetCursorPosition( cbl::Int32 x, cbl::Int32 y );
		//! Set mouse x/y position relative to window.
		void CenterCursorPosition( void );
		//! Clip cursor within window.
		void ClipCursor( bool state );
		//! Lock the mouse to the center of the window.
		void LockMouse( bool showCursor = false );
		//! Unlock the mouse from the center of the window.
		void UnlockMouse( void );

	/***** Event Handlers *****/
	public:
		void OnWindowLostFocus( void );
		void OnWindowGainedFocus( void );
		void OnWindowMouseMove( cbl::Int32 x, cbl::Int32 y );
		void OnWindowMouseDown( cbl::Int32 x, cbl::Int32 y, Mouse::Button button );
		void OnWindowMouseUp( cbl::Int32 x, cbl::Int32 y, Mouse::Button button );
		void OnWindowMouseDblClick( cbl::Int32 x, cbl::Int32 y, Mouse::Button button );
		void OnWindowMouseWheel( cbl::Int32 delta );
		void OnWindowMouseLeave( void );

	/***** Private Members *****/
	private:
		ButtonList			mButtons;		//!< Button stat	e list.
		GameWindow			* mWindow;		//!< Game window to listen to input from.
		cbl::Vector2i		mPosition;		//!< Current mouse position;
		cbl::Vector2i		mStoredPosition;
		bool				mClipCursor;
		bool				mShowMouse;
		bool				mLockMouse;		//!< Lock the mouse cursor.
		bool				mFocused;
		bool				mCentering;
	};
}

//! Declare mouse manager type.
CBL_TYPE( dbl::MouseManager, MouseManager );

#endif // __DBL_MOUSEMANAGER_H_
