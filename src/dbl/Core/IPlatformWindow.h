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
 * @file IPlatformWindow.h
 * @brief Abstract platform specific window implementation.
 */

#ifndef __DBL_PLATFORMWINDOW_H_
#define __DBL_PLATFORMWINDOW_H_

// Chewable Headers //
#include <cbl/Chewable.h>
#include <cbl/Util/Noncopyable.h>
#include <cbl/Core/Event.h>
#include <cbl/Util/Property.h>

// Delectable Headers //
#include "dbl/Delectable.h"
#include "dbl/Core/GameWindowSettings.h"

namespace dbl
{
	// Using directive.
	using cbl::Event;

	//! @brief Abstract platform window implementation.
	//! 
	//! Derive from this class to implement a platform specific window implementation.
	class IPlatformWindow :
		cbl::Noncopyable
	{
	/***** Types *****/
	public:
		//! Window size list.
		typedef std::list< GameWindowSize >		GameWindowSizeList;

	/***** Static Public Methods *****/
	public:
		//! Create a new platform window instance.
		//! @param	host	Host window.
		//! @param	s		Platform window settings reference.
		static IPlatformWindow * Create( GameWindow * const host, GameWindowSettings & s );

	/***** Properties *****/
	public:
		//! Get game window handle.
		virtual GameWindowHandle GetHandle( void ) = 0;

	/***** Public Methods *****/
	public:
		//! Destructor.
		virtual ~IPlatformWindow();
		//! Set window position.
		//! @param	x		X position.
		//! @param	y		Y position.
		virtual void DoSetSizePosition( void ) = 0;
		//! Reset window title.
		virtual void DoSetTitle( void ) = 0;
		//! Reset window style.
		virtual void DoSetStyle( void ) = 0;
		//! Reset window fullscreen toggle.
		virtual void DoSetFullscreen( void ) = 0;
		//! Show/hide window.
		//! @param	state	Window visibility state.
		virtual void Show( bool state ) = 0;
		//! Show/hide window cursor.
		//! @param	state	Window cursor visibility state.
		virtual void ShowCursor( bool state ) = 0;
		//! Load and set cursor to the a file.
		virtual void SetCursor( const cbl::Char * file ) = 0;
		//! Set the cursor position.
		//! @param	x		Window cursor x position.
		//! @param	y		Window cursor y position.
		virtual void SetCursorPosition( cbl::Int32 x, cbl::Int32 y ) = 0;
		//! Lock the cursor into the platform window.
		//! @param	state	Clip state.
		virtual void ClipCursor( bool state ) = 0;
		//! Process window events.
		virtual void ProcessWindowEvents( void ) = 0;
		//! Set whether the drag and drop works.
		virtual void SetAcceptDragDrop( bool state ) = 0;

	/***** Protected Methods *****/
	protected:
		//! Constructor.
		//! @param	s		Window settings reference.
		IPlatformWindow( GameWindow * const host, GameWindowSettings & s );

	/***** Protected Members *****/
	protected:
		GameWindow							* mHost;				//!< Pointer to host window.
		GameWindowSettings					& mSettings;			//!< Const reference to windows settings.
	};
}

#endif // __DBL_PLATFORMWINDOW_H_
