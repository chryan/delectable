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
 * @file DblTypes.cpp
 * @brief Delectable type registration.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Reflection/DblRegistrar.h"

using namespace dbl;

DblRegistrar DblRegistrar::_sInstance = DblRegistrar();

void RegisterKeyMouseEnums( cbl::TypeDB& typedb );

void DblRegistrar::RegisterDblTypes( void )
{
	cbl::TypeDB& typedb = CBL_ENT.Types;

	RegisterKeyMouseEnums( typedb );

	typedb.Create<GameWindowStyle>()
		.CBL_FIELD( Close, GameWindowStyle )
		.CBL_FIELD( Minimize, GameWindowStyle )
		.CBL_FIELD( Fullscreen, GameWindowStyle );

	typedb.Create<GameWindowSize>()
		.CBL_FIELD( Width, GameWindowSize )
		.CBL_FIELD( Height, GameWindowSize )
		.CBL_FIELD( BitsPerPixel, GameWindowSize );

	typedb.Create<GameWindowSettings>()
		.CBL_FIELD( Title, GameWindowSettings )
		.CBL_FIELD( Style, GameWindowSettings )
		.CBL_FIELD( Resolution, GameWindowSettings )
		.CBL_FIELD_ATTR( AspectRatio, GameWindowSettings, cbl::FieldAttr::F_TRANSIENT );

	typedb.Create<LevelObject>()
		.Base<cbl::Object>();
}

DblRegistrar::DblRegistrar()
{
	RegisterDblTypes();
}

void RegisterKeyMouseEnums( cbl::TypeDB& typedb )
{
	/***** Mouse *****/
	typedb.Create<Mouse::Button>()
		.CBL_ENUM( Left, Mouse )
		.CBL_ENUM( Right, Mouse )
		.CBL_ENUM( Middle, Mouse )
		.CBL_ENUM( XButton1, Mouse )
		.CBL_ENUM( XButton2, Mouse );

	/***** Keyboard *****/
	typedb.Create<Key::Code>()
		.CBL_ENUM( A, Key )
		.CBL_ENUM( B, Key )
		.CBL_ENUM( C, Key )
		.CBL_ENUM( D, Key )
		.CBL_ENUM( E, Key )
		.CBL_ENUM( F, Key )
		.CBL_ENUM( G, Key )
		.CBL_ENUM( H, Key )
		.CBL_ENUM( I, Key )
		.CBL_ENUM( J, Key )
		.CBL_ENUM( K, Key )
		.CBL_ENUM( L, Key )
		.CBL_ENUM( M, Key )
		.CBL_ENUM( N, Key )
		.CBL_ENUM( O, Key )
		.CBL_ENUM( P, Key )
		.CBL_ENUM( Q, Key )
		.CBL_ENUM( R, Key )
		.CBL_ENUM( S, Key )
		.CBL_ENUM( T, Key )
		.CBL_ENUM( U, Key )
		.CBL_ENUM( V, Key )
		.CBL_ENUM( W, Key )
		.CBL_ENUM( X, Key )
		.CBL_ENUM( Y, Key )
		.CBL_ENUM( Z, Key )
		.CBL_ENUM( Num0, Key )
		.CBL_ENUM( Num1, Key )
		.CBL_ENUM( Num2, Key )
		.CBL_ENUM( Num3, Key )
		.CBL_ENUM( Num4, Key )
		.CBL_ENUM( Num5, Key )
		.CBL_ENUM( Num6, Key )
		.CBL_ENUM( Num7, Key )
		.CBL_ENUM( Num8, Key )
		.CBL_ENUM( Num9, Key ) 
		.CBL_ENUM( Escape, Key )
		.CBL_ENUM( LCtrl, Key )
		.CBL_ENUM( LShift, Key )
		.CBL_ENUM( LAlt, Key )
		.CBL_ENUM( LSystem, Key )
		.CBL_ENUM( RCtrl, Key )
		.CBL_ENUM( RShift, Key )
		.CBL_ENUM( RAlt, Key )
		.CBL_ENUM( RSystem, Key )
		.CBL_ENUM( Menu, Key )
		.CBL_ENUM( LBracket, Key )
		.CBL_ENUM( RBracket, Key )
		.CBL_ENUM( SemiColon, Key )
		.CBL_ENUM( Comma, Key )
		.CBL_ENUM( Period, Key )
		.CBL_ENUM( Quote, Key )
		.CBL_ENUM( Slash, Key )
		.CBL_ENUM( BackSlash, Key )
		.CBL_ENUM( Tilde, Key )
		.CBL_ENUM( Equal, Key )
		.CBL_ENUM( Dash, Key )
		.CBL_ENUM( Space, Key )
		.CBL_ENUM( Return, Key )
		.CBL_ENUM( Back, Key )
		.CBL_ENUM( Tab, Key )
		.CBL_ENUM( PageUp, Key )
		.CBL_ENUM( PageDown, Key )
		.CBL_ENUM( End, Key )
		.CBL_ENUM( Home, Key )
		.CBL_ENUM( Insert, Key )
		.CBL_ENUM( Delete, Key )
		.CBL_ENUM( Add, Key )
		.CBL_ENUM( Subtract, Key )
		.CBL_ENUM( Multiply, Key )
		.CBL_ENUM( Divide, Key )
		.CBL_ENUM( Left, Key )
		.CBL_ENUM( Right, Key )
		.CBL_ENUM( Up, Key )
		.CBL_ENUM( Down, Key )
		.CBL_ENUM( Numpad0, Key )
		.CBL_ENUM( Numpad1, Key )
		.CBL_ENUM( Numpad2, Key )
		.CBL_ENUM( Numpad3, Key )
		.CBL_ENUM( Numpad4, Key )
		.CBL_ENUM( Numpad5, Key )
		.CBL_ENUM( Numpad6, Key )
		.CBL_ENUM( Numpad7, Key )
		.CBL_ENUM( Numpad8, Key )
		.CBL_ENUM( Numpad9, Key )
		.CBL_ENUM( F1, Key )
		.CBL_ENUM( F2, Key )
		.CBL_ENUM( F3, Key )
		.CBL_ENUM( F4, Key )
		.CBL_ENUM( F5, Key )
		.CBL_ENUM( F6, Key )
		.CBL_ENUM( F7, Key )
		.CBL_ENUM( F8, Key )
		.CBL_ENUM( F9, Key )
		.CBL_ENUM( F10, Key )
		.CBL_ENUM( F11, Key )
		.CBL_ENUM( F12, Key )
		.CBL_ENUM( F13, Key )
		.CBL_ENUM( F14, Key )
		.CBL_ENUM( F15, Key )
		.CBL_ENUM( Pause, Key );
}
