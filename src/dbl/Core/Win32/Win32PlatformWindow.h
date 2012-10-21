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
 * @file WindowImplWin32.h
 * @brief Windows specific window implementation.
 */

#ifndef __DBL_WIN32PLATFORMWINDOW_H_
#define __DBL_WIN32PLATFORMWINDOW_H_

// Delectable Headers //
#include "../IPlatformWindow.h"

// External Dependencies //
#include <windows.h>

namespace dbl
{
	//! @brief Windows 32-bit platform window.
	//! This class is created in the GameWindow through a static factory method and should not be instantiated manually.
	class Win32PlatformWindow :
		public IPlatformWindow
	{
	/***** Properties *****/
	public:
		//! Get the game window handle.
		//! @return			Returns a pointer to the window handle.
		virtual GameWindowHandle	GetHandle( void );

	/***** Public Methods *****/
	public:
		//! Constructor.
		//! @param	s		Reference to window settings.
		Win32PlatformWindow( GameWindow * const host, GameWindowSettings & s );
		//! Destructor.
		//! Automatically destroys window when deleted.
		virtual ~Win32PlatformWindow();
		//! Set window position.
		//! @param	x		X position.
		//! @param	y		Y position.
		virtual void DoSetSizePosition( void );
		//! Reset window title using referenced window settings.
		virtual void DoSetTitle( void );
		//! Reset window style using referenced window settings.
		virtual void DoSetStyle( void );
		//! Reset window fullscreen mode using referenced window settings.
		virtual void DoSetFullscreen( void );
		//! Show/hide window.
		//! @param	state	Window visibility state.
		virtual void Show( bool state );
		//! Show/hide window cursor.
		//! @param	state	Window cursor visibility state.
		virtual void ShowCursor( bool state );
		//! Load and set cursor to a file.
		virtual void SetCursor( const cbl::Char * file );
		//! Set the cursor position within the window.
		//! @param	x		Window cursor x position.
		//! @param	y		Window cursor y position.
		virtual void SetCursorPosition( cbl::Int32 x, cbl::Int32 y );
		//! Lock the cursor into the platform window.
		//! @param	clip	Clip state.
		virtual void ClipCursor( bool clip );
		//! Process window events.
		virtual void ProcessWindowEvents( void );
		//! Set whether the drag and drop works.
		virtual void SetAcceptDragDrop( bool state );
		//! Process individual window event.
		void ProcessEvent( UINT msg, WPARAM wParam, LPARAM lParam );

	/***** Private Methods *****/
	private:
		//! Register window class (Win32 thing).
		void RegisterWindowClass( void );
		void SetIdealDimensions( RECT& rc );

	/***** Public Static Methods *****/
	public:
		//! Window procedure callback.
		static LRESULT CALLBACK WindowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam );

	/***** Private Static Members *****/
	private:
		static cbl::Uint32					sWindowCount;      //!< Number of windows that we own

	/***** Internal Types *****/
	private:
		typedef std::unordered_map<cbl::Hash, HCURSOR>		CursorTable;

	/***** Private Members *****/
	private:
		HWND				mHwnd;				//!< Win32 handle of the window
		HCURSOR				mCurrentCursor;		//!< The system cursor to display into the window
		HICON				mIcon;				//!< Custom icon assigned to the window
		bool				mShowCursor;		//!< Show cursor flag.
		bool				mIsCursorIn;		//!< Is the mouse cursor in the window's area?
		bool				mClippedCursor;
		bool				mResizingMove;
		CursorTable			mCursorTable;
	};
}

#endif // __DBL_WIN32PLATFORMWINDOW_H_
