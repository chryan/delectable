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
 * @file MouseButtons.h
 * @brief Input mouse button codes.
 */

#ifndef __DBL_MOUSEBUTTONS_H_
#define __DBL_MOUSEBUTTONS_H_

namespace dbl
{
	//! Mouse button list.
	namespace Mouse
	{
		enum Button
		{
			Left		= 1,
			Right,
			Middle,
			XButton1,
			XButton2,
			
			Count,
		};
	}
}

CBL_TYPE( dbl::Mouse::Button, MouseButton );

#endif // __DBL_MOUSEBUTTONS_H_
