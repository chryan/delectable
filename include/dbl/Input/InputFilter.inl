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
 * @file InputFilter<TYPE>.inl
 * @brief Input filter system.
 */

namespace dbl
{
	template< typename TYPE >
	inline void InputFilter<TYPE>::SetStackSize( cbl::Uint32 size )
	{
		mFilterStack.resize( size );
	}

	template< typename TYPE >
	inline void InputFilter<TYPE>::Set( cbl::Uint32 id, bool active )
	{
		if( id < mFilterStack.size() )
			mFilterStack[id].Active = active;
	}

	template< typename TYPE >
	inline bool InputFilter<TYPE>::GetActive( cbl::Uint32 id ) const
	{
		for( size_t i = mFilterStack.size(); i > 0; --i )
			if( mFilterStack[i-1].Active )
				return id == i-1;
		return false;
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::Type* InputFilter<TYPE>::Top( void )
	{
		if( mFilterStack.size() == 0 ) return NULL;

		for( size_t i = mFilterStack.size(); i > 0; --i )
			if( mFilterStack[i-1].Active )
				return mFilterStack[i-1].Filter.Get();

		return NULL;
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::Type& InputFilter<TYPE>::operator[] ( size_t index ) {
		return *(mFilterStack[index].Filter);
	}

	template< typename TYPE >
	inline const typename InputFilter<TYPE>::Type& InputFilter<TYPE>::operator[] ( size_t index ) const {
		return *(mFilterStack[index].Filter);
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::iterator InputFilter<TYPE>::begin( void ) {
		return mFilterStack.begin();
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::const_iterator InputFilter<TYPE>::begin( void ) const {
		return mFilterStack.begin();
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::iterator InputFilter<TYPE>::end( void ) {
		return mFilterStack.end();
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::const_iterator InputFilter<TYPE>::end( void ) const {
		return mFilterStack.end();
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::reverse_iterator InputFilter<TYPE>::rbegin( void ) {
		return mFilterStack.rbegin();
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::const_reverse_iterator InputFilter<TYPE>::rbegin( void ) const {
		return mFilterStack.rbegin();
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::reverse_iterator InputFilter<TYPE>::rend( void ) {
		return mFilterStack.rend();
	}

	template< typename TYPE >
	inline typename InputFilter<TYPE>::const_reverse_iterator InputFilter<TYPE>::rend( void ) const {
		return mFilterStack.rend();
	}

	template< typename TYPE >
	InputFilter<TYPE>::StackElement::StackElement()
	: Filter( new TYPE () )
	, Active( false )
	{
	}

	template< typename TYPE >
	InputFilter<TYPE>::StackElement::~StackElement()
	{
	}
}
