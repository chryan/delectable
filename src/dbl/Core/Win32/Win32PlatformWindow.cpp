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
 * @file Win32PlatformWindow.cpp
 * @brief Windows specific window implementation.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "Win32PlatformWindow.h"
#include "dbl/Input/KeyCodes.h"
#include "dbl/Core/GameWindow.h"

// Chewable Headers //
#include <cbl/Core/Game.h>
#include <cbl/Debug/Logging.h>

// External Libraries //
#include <ShellAPI.h>

using namespace dbl;

Key::Code ConvertVirtualCode( WPARAM virtualKey, LPARAM flags );

cbl::Uint32					Win32PlatformWindow::sWindowCount	= 0;
const cbl::Char * const		sWindowClassNameA		= "DELECTABLE_WINDOW_CLASS";
const cbl::Wchar * const	sWindowClassNameW		= L"DELECTABLE_WINDOW_CLASS";

static std::wstring			sTitle					= L"Delectable Window";

Win32PlatformWindow::Win32PlatformWindow( GameWindow * const host, GameWindowSettings & s )
: IPlatformWindow( host, s )
, mHwnd( NULL )
, mCurrentCursor( NULL )
, mIcon( NULL )
, mShowCursor( true )
, mIsCursorIn( false )
, mClippedCursor( false )
, mResizingMove( false )
{
	if( sWindowCount == 0 )
		RegisterWindowClass();

	RECT rc;
	SetIdealDimensions( rc );

	if( s.Title.length() == 0 )
		s.Title = host->Game.GetName();

	DWORD style = GameWindow::GetSystemWindowStyle( mSettings.Style );
#if CBL_TEXT == CBL_TEXT_UTF16
	sTitle.resize( s.Title.length(), L' ' );
	std::copy(s.Title.begin(), s.Title.end(), sTitle.begin());

	mHwnd = CreateWindowW( sWindowClassNameW, sTitle.c_str(), style, rc.left, rc.top, 
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, GetModuleHandle(NULL), this );
#else
	mHwnd = CreateWindowA( sWindowClassNameA, s.Title.c_str(), style, rc.left, rc.top, 
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, GetModuleHandle(NULL), this );
#endif

	mCurrentCursor = ::LoadCursor( NULL, IDC_ARROW );
	::ShowWindow( mHwnd, SW_SHOW );
	::ShowCursor( true );

	if( s.Style.Fullscreen )
		DoSetFullscreen();
	else
		DoSetSizePosition();

	SetForegroundWindow( mHwnd );

	++sWindowCount;
}

GameWindowHandle Win32PlatformWindow::GetHandle( void )
{
	return reinterpret_cast< GameWindowHandle >( mHwnd );
}

Win32PlatformWindow::~Win32PlatformWindow()
{
	if( mHwnd )
		::DestroyWindow( mHwnd );

	--sWindowCount;

	if( sWindowCount == 0 )
#if CBL_TEXT == CBL_TEXT_UTF16
		UnregisterClassW( sWindowClassNameW, GetModuleHandle( NULL ) );
#else
		UnregisterClassA( sWindowClassNameA, GetModuleHandle( NULL ) );
#endif
}

void Win32PlatformWindow::DoSetSizePosition( void )
{
	if( mSettings.Style.Fullscreen ) {
		mSettings.Dimensions.Width = mSettings.Resolution.Width;
		mSettings.Dimensions.Height = mSettings.Resolution.Height;
		mSettings.AspectRatio = cbl::Real( mSettings.Dimensions.Width ) / cbl::Real( mSettings.Dimensions.Height );
		::SetWindowPos( mHwnd, HWND_TOP, 0, 0, mSettings.Resolution.Width, mSettings.Resolution.Height, SWP_FRAMECHANGED );
	}
	else {
		RECT rc; SetIdealDimensions( rc );
		mSettings.AspectRatio = cbl::Real( mSettings.Dimensions.Width ) / cbl::Real( mSettings.Dimensions.Height );
		::SetWindowPos( mHwnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 0 );
	}
	mResizingMove = true;

	mHost->OnWindowResized(
		mHost->GetSettings().Resolution.Width,
		mHost->GetSettings().Resolution.Height,
		mHost->GetSettings().Dimensions.Width,
		mHost->GetSettings().Dimensions.Height,
		mHost->GetSettings().Resolution.BitsPerPixel,
		mHost->GetSettings().AspectRatio
	);
	SetForegroundWindow( mHwnd );
}

void Win32PlatformWindow::DoSetTitle( void )
{
#if CBL_TEXT == CBL_TEXT_UTF16
	sTitle.resize( mSettings.Title.length(), L' ' );
	std::copy( mSettings.Title.begin(), mSettings.Title.end(), sTitle.begin() );

	::SetWindowTextW( mHwnd, sTitle.c_str() );
#else
	::SetWindowTextA( mHwnd, mSettings.Title.c_str() );
#endif
}

void Win32PlatformWindow::DoSetStyle( void )
{
	SetWindowLongPtr( mHwnd, GWL_STYLE, GameWindow::GetSystemWindowStyle( mSettings.Style ) );
}

void Win32PlatformWindow::DoSetFullscreen( void )
{
	if( mSettings.Style.Fullscreen )
	{
		DEVMODE dm;
		dm.dmSize       = sizeof(DEVMODE);
		dm.dmPelsWidth  = DWORD( mSettings.Resolution.Width );
		dm.dmPelsHeight = DWORD( mSettings.Resolution.Height );
		dm.dmBitsPerPel = DWORD( mSettings.Resolution.BitsPerPixel );
		dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Apply fullscreen mode
		if( ::ChangeDisplaySettings( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			mHost->OnWindowFullscreen( true, true );
			LOG( cbl::LogLevel::Error << "Failed to change display mode for fullscreen." );

			return;
		}

		// Change window style (no border, no titlebar, ...)
		::SetWindowLong( mHwnd, GWL_STYLE,   WS_POPUP );
		::SetWindowLong( mHwnd, GWL_EXSTYLE, WS_EX_APPWINDOW );

		// SetPixelFormat can fail (really ?) if window style doesn't contain these flags
		long Style = ::GetWindowLong( mHwnd, GWL_STYLE );
		::SetWindowLong( mHwnd, GWL_STYLE, Style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
	}
	else
	{
		ChangeDisplaySettings( NULL, 0 );
		DoSetStyle();
	}

	DoSetSizePosition();
	ShowWindow( mHwnd, SW_SHOW );
}

void Win32PlatformWindow::Show( bool state )
{
	::ShowWindow( mHwnd, state ? SW_SHOW : SW_HIDE );
}

void Win32PlatformWindow::ShowCursor( bool state )
{
	mShowCursor = state;
	::SetCursor( state ? mCurrentCursor : NULL );
}

void Win32PlatformWindow::SetCursor( const cbl::Char * file )
{
	if( file == NULL )
		mCurrentCursor = ::LoadCursor( NULL, IDC_ARROW );
	else {
		cbl::Hash hash = cbl::Hash( file );
		CursorTable::iterator findit = mCursorTable.find( hash );
		if( findit != mCursorTable.end() )
			mCurrentCursor = findit->second;
		else if( HCURSOR cursor = ::LoadCursorFromFile( file ) ) {
			mCurrentCursor = cursor;
			mCursorTable.insert( std::make_pair( hash, cursor ) );
		}
		else
			mCurrentCursor = ::LoadCursor( NULL, IDC_ARROW );
	}

	::SetCursor( mShowCursor ? mCurrentCursor : NULL );
}

void Win32PlatformWindow::SetCursorPosition( cbl::Int32 x, cbl::Int32 y )
{
	RECT rect;
	if( GetClientRect( mHwnd, (LPRECT)&rect ) != 0 )
	{
		ClientToScreen( mHwnd, (LPPOINT)&rect.left );
		ClientToScreen( mHwnd, (LPPOINT)&rect.right );
		rect.right -= 1;
		rect.bottom -= 1;

		if( x < 0 )
			x = rect.left;
		else if( x >= rect.right - rect.left )
			x = rect.right;
		else
			x = rect.left + x;

		if( y < 0 )
			y = rect.top;
		else if( y >= rect.bottom - rect.top )
			y = rect.bottom;
		else
			y = rect.top + y;

		SetCursorPos( x, y );
	}
}

void Win32PlatformWindow::ClipCursor( bool clip )
{
	RECT rect;
	if( mClippedCursor && !clip ) {
		::ClipCursor( NULL );
	}
	else if( !mClippedCursor && clip && GetClientRect( mHwnd, (LPRECT)&rect ) != 0 )
	{
		ClientToScreen( mHwnd, (LPPOINT)&rect.left );
		ClientToScreen( mHwnd, (LPPOINT)&rect.right );
		::ClipCursor( &rect );
	}
	mClippedCursor = clip;
}

void Win32PlatformWindow::ProcessWindowEvents( void )
{
	// We update the window only if it has been created.
	if( !mHwnd )
		return;

	MSG msg;
	while( PeekMessage( &msg, mHwnd, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void Win32PlatformWindow::SetAcceptDragDrop( bool state )
{
	::DragAcceptFiles( mHwnd, state ? TRUE : FALSE );
}

void Win32PlatformWindow::ProcessEvent( UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( !mHwnd )
		return;

	switch( msg ) {
		// Window close event.
		case WM_CLOSE: {
			mHost->OnWindowClosed();
			} break;
		// Set cursor event
		case WM_SETCURSOR: {
			// The mouse has moved, if the cursor is in our window we must refresh the cursor
			if (LOWORD(lParam) == HTCLIENT)
				::SetCursor( mShowCursor ? mCurrentCursor : NULL );
			} break;
		// Window gain focus event.
		case WM_SETFOCUS: {
			mHost->OnWindowGainedFocus();
			} break;
		// Window lost focus event.
		case WM_KILLFOCUS: {
			mHost->OnWindowLostFocus();
			} break;
		case WM_MOVE:
			if( mResizingMove ) {
				mResizingMove = false;
				if( mClippedCursor ) {
					ClipCursor( false );
					ClipCursor( true );
				}
			}
		// Text event.
		case WM_CHAR: {
			mHost->OnWindowKeyChar( cbl::Uint32( wParam ) );
			} break;
		// Key down event.
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			dbl::Key::Code key = ConvertVirtualCode( wParam, lParam );
			if( key != 0 )
				mHost->OnWindowKeyDown( key );
			} break;
		// Key up event.
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			dbl::Key::Code key = ConvertVirtualCode( wParam, lParam );
			if( key != 0 )
				mHost->OnWindowKeyUp( key );
			} break;
		// Mouse wheel event.
		case WM_MOUSEWHEEL : {
			mHost->OnWindowMouseWheel( static_cast< cbl::Int16 >( HIWORD( wParam ) ) / 120 );
			} break;
		// Mouse move event.
		case WM_MOUSEMOVE : {
			if( !mIsCursorIn ) {
				// TODO: Finish tracking.
				TRACKMOUSEEVENT tme;
				tme.cbSize    = sizeof(TRACKMOUSEEVENT);
				tme.hwndTrack = mHwnd;
				tme.dwFlags   = TME_LEAVE;
				TrackMouseEvent(&tme);

				mIsCursorIn = true;
				mHost->OnWindowMouseEnter();
			}
			mHost->OnWindowMouseMove( LOWORD(lParam), HIWORD(lParam) );
			} break;
		// Left mouse button down event.
		case WM_LBUTTONDOWN : {
			mHost->OnWindowMouseDown( LOWORD(lParam), HIWORD(lParam), Mouse::Left );
			} break;
		// Left mouse button up event.
		case WM_LBUTTONUP : {
			mHost->OnWindowMouseUp( LOWORD(lParam), HIWORD(lParam), Mouse::Left );
			} break;
		// Left mouse button double click event.
		case WM_LBUTTONDBLCLK : {
			mHost->OnWindowMouseDblClick( LOWORD(lParam), HIWORD(lParam), Mouse::Left );
			} break;
		// Left mouse button down event.
		case WM_RBUTTONDOWN : {
			mHost->OnWindowMouseDown( LOWORD(lParam), HIWORD(lParam), Mouse::Right );
			} break;
		// Right mouse button up event.
		case WM_RBUTTONUP : {
			mHost->OnWindowMouseUp( LOWORD(lParam), HIWORD(lParam), Mouse::Right );
			} break;
		// Right mouse button double click event.
		case WM_RBUTTONDBLCLK : {
			mHost->OnWindowMouseDblClick( LOWORD(lParam), HIWORD(lParam), Mouse::Right );
			} break;
		// Middle mouse button down event.
		case WM_MBUTTONDOWN : {
			mHost->OnWindowMouseDown( LOWORD(lParam), HIWORD(lParam), Mouse::Middle );
			} break;
		// Middle mouse button up event.
		case WM_MBUTTONUP : {
			mHost->OnWindowMouseUp( LOWORD(lParam), HIWORD(lParam), Mouse::Middle );
			} break;
		// Right mouse button double click event.
		case WM_MBUTTONDBLCLK : {
			mHost->OnWindowMouseDblClick( LOWORD(lParam), HIWORD(lParam), Mouse::Middle );
			} break;
		// X1 mouse button down event.
		case WM_XBUTTONDOWN : {
			mHost->OnWindowMouseDown( LOWORD(lParam), HIWORD(lParam),
				HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2 );
			} break;
		// X2 mouse button up event.
		case WM_XBUTTONUP : {
			mHost->OnWindowMouseUp( LOWORD(lParam), HIWORD(lParam),
				HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2 );
			} break;
		// Right mouse button double click event.
		case WM_XBUTTONDBLCLK : {
			mHost->OnWindowMouseDblClick( LOWORD(lParam), HIWORD(lParam),
				HIWORD(wParam) == XBUTTON1 ? Mouse::XButton1 : Mouse::XButton2 );
			} break;
		case WM_MOUSELEAVE: {
			mIsCursorIn = false;
			mHost->OnWindowMouseLeave();
			} break;
		case WM_DROPFILES: {
			HDROP hDrop = (HDROP)wParam;

			POINT dropPoint;
			::DragQueryPoint( hDrop, &dropPoint );
			cbl::Uint32 fileCount = ::DragQueryFile( hDrop, 0xFFFFFFFF, NULL, 0 );

			cbl::FileInfo::List files;
			for( cbl::Uint32 i = 0; i < fileCount; ++i ) {
				cbl::Char tmpFile[256];
				if( ::DragQueryFile( hDrop, i, tmpFile, 256 ) ) {
					// This is a folder?
					if( cbl::FileSystem::FolderCheckExists( tmpFile ) ) {
						cbl::FileInfo::List _newfiles = cbl::FileSystem::GetFileList( tmpFile, true );
						for( size_t i = 0; i < _newfiles.size(); ++i )
							files.push_back( _newfiles[i] );
					}
					else 
						files.push_back( tmpFile );
				}
			}
			::DragFinish( hDrop );
			mHost->OnWindowDragDrop( dropPoint.x, dropPoint.y, files );

			} break;
	}
}

void Win32PlatformWindow::RegisterWindowClass( void )
{
#if CBL_TEXT == CBL_TEXT_UTF16
	WNDCLASSW wc;
	wc.style         = CS_DBLCLKS;
	wc.lpfnWndProc   = &Win32PlatformWindow::WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetModuleHandle(NULL);
	wc.hIcon         = NULL;
	wc.hCursor       = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = sWindowClassNameW;
	RegisterClassW( &wc );
#else
	WNDCLASSA wc;
	wc.style         = CS_DBLCLKS;
	wc.lpfnWndProc   = &Win32PlatformWindow::WindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetModuleHandle(NULL);
	wc.hIcon         = NULL;
	wc.hCursor       = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = sWindowClassNameA;
	RegisterClassA( &wc );
#endif

	++sWindowCount;
}

void Win32PlatformWindow::SetIdealDimensions( RECT& rc )
{
	DWORD w32style = GameWindow::GetSystemWindowStyle( mSettings.Style );
	GameWindow::GetBestClientWindowDimensions( mSettings.Resolution.Width, mSettings.Resolution.Height, mSettings.Style, mSettings.Dimensions.Width, mSettings.Dimensions.Height );

	cbl::Uint32 cx, cy;
	GameWindow::GetWorkableAreaCenter( cx, cy, mSettings.Style );

	cbl::Int32 hWidth = cbl::Int32( mSettings.Dimensions.Width ) / 2;
	cbl::Int32 hHeight = cbl::Int32( mSettings.Dimensions.Height ) / 2;

	rc.left		= cbl::Int32( cx ) - hWidth;
	rc.top		= cbl::Int32( cy ) - hHeight;
	rc.right	= cbl::Int32( cx ) + ( hWidth % 2 ? hWidth + 1 : hWidth );
	rc.bottom	= cbl::Int32( cy ) + ( hHeight % 2 ? hHeight + 1 : hHeight );

	::AdjustWindowRect( &rc, w32style, FALSE );
}

dbl::Key::Code ConvertVirtualCode( WPARAM virtualKey, LPARAM flags )
{
	switch( virtualKey )
	{
		// VK_SHIFT is handled by the GetShiftState function
		case VK_SHIFT :			return ( MapVirtualKey( VK_RSHIFT, MAPVK_VK_TO_VSC ) == UINT((flags & 0x01FF0000) >> 16) ? Key::RShift : Key::LShift );
		case VK_MENU :			return (flags & (1 << 24)) ? Key::RAlt		: Key::LAlt;
		case VK_CONTROL :		return (flags & (1 << 24)) ? Key::RCtrl	: Key::LCtrl;
		case VK_LWIN :			return Key::LSystem;
		case VK_RWIN :			return Key::RSystem;
		case VK_APPS :			return Key::Menu;
		case VK_OEM_1 :			return Key::SemiColon;
		case VK_OEM_2 :			return Key::Slash;
		case VK_OEM_PLUS :		return Key::Equal;
		case VK_OEM_MINUS :		return Key::Dash;
		case VK_OEM_4 :			return Key::LBracket;
		case VK_OEM_6 :			return Key::RBracket;
		case VK_OEM_COMMA :		return Key::Comma;
		case VK_OEM_PERIOD :	return Key::Period;
		case VK_OEM_7 :			return Key::Quote;
		case VK_OEM_5 :			return Key::BackSlash;
		case VK_OEM_3 :			return Key::Tilde;
		case VK_ESCAPE :		return Key::Escape;
		case VK_SPACE :			return Key::Space;
		case VK_RETURN :		return Key::Return;
		case VK_BACK :			return Key::Back;
		case VK_TAB :			return Key::Tab;
		case VK_PRIOR :			return Key::PageUp;
		case VK_NEXT :			return Key::PageDown;
		case VK_END :			return Key::End;
		case VK_HOME :			return Key::Home;
		case VK_INSERT :		return Key::Insert;
		case VK_DELETE :		return Key::Delete;
		case VK_ADD :			return Key::Add;
		case VK_SUBTRACT :		return Key::Subtract;
		case VK_MULTIPLY :		return Key::Multiply;
		case VK_DIVIDE :		return Key::Divide;
		case VK_PAUSE :			return Key::Pause;
		case VK_F1 :			return Key::F1;
		case VK_F2 :			return Key::F2;
		case VK_F3 :			return Key::F3;
		case VK_F4 :			return Key::F4;
		case VK_F5 :			return Key::F5;
		case VK_F6 :			return Key::F6;
		case VK_F7 :			return Key::F7;
		case VK_F8 :			return Key::F8;
		case VK_F9 :			return Key::F9;
		case VK_F10 :			return Key::F10;
		case VK_F11 :			return Key::F11;
		case VK_F12 :			return Key::F12;
		case VK_F13 :			return Key::F13;
		case VK_F14 :			return Key::F14;
		case VK_F15 :			return Key::F15;
		case VK_LEFT :			return Key::Left;
		case VK_RIGHT :			return Key::Right;
		case VK_UP :			return Key::Up;
		case VK_DOWN :			return Key::Down;
		case VK_NUMPAD0 :		return Key::Numpad0;
		case VK_NUMPAD1 :		return Key::Numpad1;
		case VK_NUMPAD2 :		return Key::Numpad2;
		case VK_NUMPAD3 :		return Key::Numpad3;
		case VK_NUMPAD4 :		return Key::Numpad4;
		case VK_NUMPAD5 :		return Key::Numpad5;
		case VK_NUMPAD6 :		return Key::Numpad6;
		case VK_NUMPAD7 :		return Key::Numpad7;
		case VK_NUMPAD8 :		return Key::Numpad8;
		case VK_NUMPAD9 :		return Key::Numpad9;
		case 'A' :				return Key::A;
		case 'Z' :				return Key::Z;
		case 'E' :				return Key::E;
		case 'R' :				return Key::R;
		case 'T' :				return Key::T;
		case 'Y' :				return Key::Y;
		case 'U' :				return Key::U;
		case 'I' :				return Key::I;
		case 'O' :				return Key::O;
		case 'P' :				return Key::P;
		case 'Q' :				return Key::Q;
		case 'S' :				return Key::S;
		case 'D' :				return Key::D;
		case 'F' :				return Key::F;
		case 'G' :				return Key::G;
		case 'H' :				return Key::H;
		case 'J' :				return Key::J;
		case 'K' :				return Key::K;
		case 'L' :				return Key::L;
		case 'M' :				return Key::M;
		case 'W' :				return Key::W;
		case 'X' :				return Key::X;
		case 'C' :				return Key::C;
		case 'V' :				return Key::V;
		case 'B' :				return Key::B;
		case 'N' :				return Key::N;
		case '0' :				return Key::Num0;
		case '1' :				return Key::Num1;
		case '2' :				return Key::Num2;
		case '3' :				return Key::Num3;
		case '4' :				return Key::Num4;
		case '5' :				return Key::Num5;
		case '6' :				return Key::Num6;
		case '7' :				return Key::Num7;
		case '8' :				return Key::Num8;
		case '9' :				return Key::Num9;
	}

	return Key::Code( 0 );
}

LRESULT CALLBACK Win32PlatformWindow::WindowProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// Associate handle and Window instance when the creation message is received
	if( msg == WM_CREATE )
	{
		// Get WindowImplWin32 instance (it was passed as the last argument of CreateWindow)
		long This = reinterpret_cast<long>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		// Set as the "user data" parameter of the window
		SetWindowLongPtr( handle, GWLP_USERDATA, This );
	}

	// Get the WindowImpl instance corresponding to the window handle
	Win32PlatformWindow * window = reinterpret_cast< Win32PlatformWindow *>( GetWindowLongPtr( handle, GWLP_USERDATA ) );

	// Forward the event to the appropriate function
	if( window )
		window->ProcessEvent( msg, wParam, lParam);

	// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
	switch( msg ) {
	case WM_CLOSE:
	case WM_DROPFILES:
		return 0;
	}

#if CBL_TEXT == CBL_TEXT_UTF16
	return DefWindowProcW( handle, msg, wParam, lParam );
#else
	return DefWindowProcA( handle, msg, wParam, lParam );
#endif
}
