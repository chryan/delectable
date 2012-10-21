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
 * @file InputFilter.h
 * @brief Input filter system.
 */

#ifndef __DBL_INPUTFILTER_H_
#define __DBL_INPUTFILTER_H_

// Delectable Headers //
#include "dbl/Delectable.h"

// Chewable Headers //
#include "cbl/Util/Property.h"

// External Libraries //
#include <vector>

namespace dbl
{
	//! Input filter system.
	template< typename TYPE >
	class InputFilter
	{
	public:
		typedef typename cbl::StripPtr<TYPE>::Type		Type;

	/***** Public Methods *****/
	public:
		//! Set the input stack size.
		void SetStackSize( cbl::Uint32 size );
		//! Set an input stack id's active level.
		void Set( cbl::Uint32 id, bool active );
		//! Check if a filter ID is currently active.
		bool GetActive( cbl::Uint32 id ) const;
		//! Get the highest level on the input stack that is active.
		//! @return		NULL if none are active.
		Type* Top( void );
		//! Get input stack element.
		Type& operator[] ( size_t index );
		//! Get input stack element.
		const Type& operator[] ( size_t index ) const;
		// Get filter stack size.
		size_t size( void ) const { return mFilterStack.size(); }

	/***** Private Types *****/
	private:
		//! Input stack element.
		struct StackElement {
			StackElement();
			~StackElement();
			cbl::SharedPtr<Type> Filter;
			bool Active;
		};
		typedef std::vector<StackElement>	FilterStack;

	/***** Public Types *****/
	public:
		typedef typename FilterStack::iterator					iterator;
		typedef typename FilterStack::const_iterator			const_iterator;
		typedef typename FilterStack::reverse_iterator			reverse_iterator;
		typedef typename FilterStack::const_reverse_iterator	const_reverse_iterator;

	/***** Public Methods *****/
	public:
		typename iterator begin( void );
		typename const_iterator begin( void ) const;
		typename iterator end( void );
		typename const_iterator end( void ) const;
		typename reverse_iterator rbegin( void );
		typename const_reverse_iterator rbegin( void ) const;
		typename reverse_iterator rend( void );
		typename const_reverse_iterator rend( void ) const;

	/***** Private Members *****/
	private:
		FilterStack		mFilterStack;
	};
}

#include "InputFilter.inl"

#endif // __DBL_INPUTFILTER_H_
