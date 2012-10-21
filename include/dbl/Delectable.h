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
 * @file Delectable.h
 * @brief Essential header inclusions.
 */

#ifndef __DBL_DELECTABLE_H_
#define __DBL_DELECTABLE_H_

// Chewable Headers //
#include <cbl/Chewable.h>

// Delectable Headers //
#include "dbl/Config.h"
#include "dbl/Platform.h"

#define _TPL template< typename T >

// Forward Declarations //
namespace dbl
{
	// Core //
	class Game;
	class GameWindow;
	struct GameWindowSize;
	struct GameWindowSettings;
	class IPlatformWindow;
	class LevelManager;
	class LevelObject;

	// Input //
	class KeyboardManager;
	class MouseManager;

	// Reflection //
	class DblRegistrar;
	class YAMLSerialiser;
	class YAMLDeserialiser;
}

#undef _TPL

#endif // __DBL_DELECTABLE_H_
