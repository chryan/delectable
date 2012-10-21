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
 * @file IPlatformWindow.cpp
 * @brief Abstract platform specific window implementation.
 */

// Precompiled Headers //
#include "dbl/StdAfx.h"

// Delectable Headers //
#include "IPlatformWindow.h"

#if CBL_PLATFORM == CBL_PLATFORM_WIN32
#include "Win32/Win32PlatformWindow.h"
typedef ::dbl::Win32PlatformWindow PlatformWindowType;
#endif

using namespace dbl;

IPlatformWindow * IPlatformWindow::Create( GameWindow * const host, GameWindowSettings & settings )
{
	return new PlatformWindowType( host, settings );
}

IPlatformWindow::IPlatformWindow( GameWindow * const host, GameWindowSettings & settings )
: mHost( host )
, mSettings( settings )
{
}

IPlatformWindow::~IPlatformWindow()
{
}
