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
 * @file KeyboardManager.h
 * @brief Keyboard manager component.
 */

#ifndef __DBL_KEYBOARDMANAGER_H_
#define __DBL_KEYBOARDMANAGER_H_

// Chewable Headers //
#include <cbl/Chewable.h>
#include <cbl/Core/Event.h>
#include <cbl/Core/GameComponent.h>
#include <cbl/Core/Services.h>
#include <cbl/Core/GameTime.h>
#include <cbl/Util/Property.h>

// Delectable Headers //
#include "dbl/Delectable.h"
#include "dbl/Input/KeyCodes.h"
#include "dbl/Input/InputFilter.h"

// External Dependencies //
#include <bitset>

namespace dbl
{
	namespace E
	{
		/***** Keyboard events *****/
#ifdef CBL_TPLFUNCTION_PREFERRED_SYNTAX
		typedef cbl::Event<void(::dbl::Key::Code,cbl::Uint16)>	Key;		//!< params: KeyCode, Modifiers
		typedef cbl::Event<void(cbl::Uint32)>					KeyChar;	//!< params: Unicode
#else
		typedef cbl::Event2<void,::dbl::Key::Code,cbl::Uint16>	Key;		//!< params: KeyCode, Modifiers
		typedef cbl::Event1<void,cbl::Uint32>					KeyChar;	//!< params: Unicode
#endif
		typedef Key KeyDown;
		typedef Key KeyUp;
		typedef Key KeyClick;
	}

	//! @brief Keyboard management component.
	//! Register listeners with the keyboard manager events to respond to keyboard input.
	//! This class depends on a GameWindow service existing in the game application.
	class DBL_API KeyboardManager :
		public cbl::GameComponent
	{
	/***** Private Types *****/
	private:
		//! Key list type.
		typedef std::bitset< Key::Count >	KeyList;

	/***** Types *****/
	public:
		//! Filtered keyboard event.
		struct FilterEvent
		{
			KeyList		Keys;		//!< Filter key states.
			E::KeyDown	OnKeyDown;	//!< Key down event (triggers as long as key is down).
			E::KeyUp	OnKeyUp;	//!< Key up event.
			E::KeyClick	OnKeyClick;	//!< Key clicked event (triggers once when key is pressed).
			E::KeyChar	OnKeyChar;	//!< Key character event.
			inline bool IsKeyDown( Key::Code code ) const { return Keys[code]; }
		};
		//! Filtered keyboard event map.
		typedef InputFilter<FilterEvent>	FilterEvents;

	/***** Properties *****/
	public:
		//! Is key down?
		//! @param	code	Key code to check.
		bool IsKeyDown( Key::Code code ) const;

	/***** Events *****/
	public:
		FilterEvents	Filter;		//!< Filtered keyboard event map.
		E::KeyDown		OnKeyDown;	//!< Key down event (triggers as long as key is down).
		E::KeyUp		OnKeyUp;	//!< Key up event.
		E::KeyClick		OnKeyClick;	//!< Key clicked event (triggers once when key is pressed).
		E::KeyChar		OnKeyChar;	//!< Key character event.

	/***** Public Methods *****/
	public:
		//! Constructor.
		//! @param	game		Pointer to game.
		explicit KeyboardManager( cbl::Game & game );
		//! Destructor.
		virtual ~KeyboardManager();
		//! Pure virtual function to initialise component.
		virtual void Initialise( void );
		//! Pure virtual function to shut down component.
		virtual void Shutdown( void );
		//! Pure virtual update function (from IUpdatable).
		virtual void Update( const cbl::GameTime & time );

	/***** Event Handlers *****/
	public:
		void OnWindowKeyDown( Key::Code keyCode );
		void OnWindowKeyUp( Key::Code keyCode );
		void OnWindowKeyChar( cbl::Uint32 unicode );
		void OnWindowLostFocus( void );

	/***** Private Members *****/
	private:
		cbl::HashValue		mCurrentState;	//!< Current keyboard state.
		//StateMap			mStates;		//!< Keyboard state filtering.
		KeyList				mKeys;			//!< Key state list.
		GameWindow			* mWindow;		//!< Game window to listen to input from.
	};
}

//! Declare keyboard manager type.
CBL_TYPE( dbl::KeyboardManager, KeyboardManager );

#endif // __DBL_KEYBOARDMANAGER_H_
