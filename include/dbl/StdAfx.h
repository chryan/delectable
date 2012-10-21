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
 * @file StdAfx.h
 * @brief Precompiled Headers.
 */

// Precompiled Headers //
#include "cbl/StdAfx.h"

// Delectable Headers //
#include "dbl/Delectable.h"
#include "dbl/Config.h"
#include "dbl/Platform.h"
// Core //
#include "dbl/Core/Game.h"
#include "dbl/Core/GameWindow.h"
#include "dbl/Core/GameWindowSettings.h"
#include "dbl/Core/LevelManager.h"
#include "dbl/Core/LevelObject.h"
// Input //
#include "dbl/Input/InputFilter.h"
#include "dbl/Input/KeyboardManager.h"
#include "dbl/Input/KeyCodes.h"
#include "dbl/Input/MouseButtons.h"
#include "dbl/Input/MouseManager.h"
// Reflection //
#include "dbl/Reflection/DblRegistrar.h"
#include "dbl/Reflection/YAMLDeserialiser.h"
#include "dbl/Reflection/YAMLSerialiser.h"
