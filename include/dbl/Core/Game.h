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
 * @file Game.h
 * @brief Delectable game class with keyboard, mouse and windowing features.
 */

#ifndef __DBL_GAME_H_
#define __DBL_GAME_H_

// Chewable Headers //
#include <cbl/Chewable.h>
#include <cbl/Core/Game.h>

// Delectable Headers //
#include "dbl/Delectable.h"
#include "dbl/Core/GameWindow.h"
#include "dbl/Core/LevelManager.h"
#include "dbl/Input/KeyboardManager.h"
#include "dbl/Input/MouseManager.h"

namespace dbl
{
	//! @brief Basic windowed game class.
	//! Derive from this class to obtain a windowed game.
	class DBL_API Game :
		public cbl::Game
	{
	/***** Public Members *****/
	public:
		GameWindow			Window;		//!< The game window.
		KeyboardManager		Keyboard;	//!< Keyboard manager.
		MouseManager		Mouse;		//!< Mouse manager.
		LevelManager		Levels;		//!< Level manager.

	/***** Public Methods *****/
	public:
		//! Constructor.
		//! @param	name		Game application name.
		explicit Game( const cbl::Char * name );
		//! Destructor.
		virtual ~Game();
	};
}

#endif // __DBL_GAME_H_
