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
 * @file KeyCodes.h
 * @brief Input key codes.
 */

#ifndef __DBL_KEYCODES_H_
#define __DBL_KEYCODES_H_

// Delectable Headers //
#include "dbl/Delectable.h"

namespace dbl
{
	//! Key code list.
	namespace Key
	{
		//! Key modifier list.
		namespace Modifier
		{
			enum Code
			{
				Shift	= 1 << 0,
				Alt		= 1 << 1,
				Ctrl	= 1 << 2,
			};
		}
		enum Code
		{
			A = 'a',
			B = 'b',
			C = 'c',
			D = 'd',
			E = 'e',
			F = 'f',
			G = 'g',
			H = 'h',
			I = 'i',
			J = 'j',
			K = 'k',
			L = 'l',
			M = 'm',
			N = 'n',
			O = 'o',
			P = 'p',
			Q = 'q',
			R = 'r',
			S = 's',
			T = 't',
			U = 'u',
			V = 'v',
			W = 'w',
			X = 'x',
			Y = 'y',
			Z = 'z',
			Num0 = '0',
			Num1 = '1',
			Num2 = '2',
			Num3 = '3',
			Num4 = '4',
			Num5 = '5',
			Num6 = '6',
			Num7 = '7',
			Num8 = '8',
			Num9 = '9', 
			Escape = 256,
			LCtrl,
			LShift,
			LAlt,
			LSystem,      //!< OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
			RCtrl,
			RShift,
			RAlt,
			RSystem,      //!< OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
			Menu,
			LBracket,     //!< [
			RBracket,     //!< ]
			SemiColon,    //!< ;
			Comma,        //!< ,
			Period,       //!< .
			Quote,        //!< '
			Slash,        //!< /
			BackSlash,
			Tilde,        //!< ~
			Equal,        //!< =
			Dash,         //!< -
			Space,
			Return,
			Back,
			Tab,
			PageUp,
			PageDown,
			End,
			Home,
			Insert,
			Delete,
			Add,          //!< +
			Subtract,     //!< -
			Multiply,     //!< *
			Divide,       //!< /
			Left,         //!< Left arrow
			Right,        //!< Right arrow
			Up,           //!< Up arrow
			Down,         //!< Down arrow
			Numpad0,
			Numpad1,
			Numpad2,
			Numpad3,
			Numpad4,
			Numpad5,
			Numpad6,
			Numpad7,
			Numpad8,
			Numpad9,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			Pause,

			Count,
		};
	}
}

CBL_TYPE( dbl::Key::Modifier::Code, KeyModifier );
CBL_TYPE( dbl::Key::Code, KeyCode );

#endif // __DBL_KEYCODES_H_
