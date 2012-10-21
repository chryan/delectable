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
 * @file LevelManager.h
 * @brief Level manager component.
 */

#ifndef __DBL_LEVELMANAGER_H_
#define __DBL_LEVELMANAGER_H_

// Delectable Headers //
#include "dbl/Delectable.h"
#include "dbl/Reflection/YAMLDeserialiser.h"
#include "dbl/Reflection/YAMLSerialiser.h"

// Chewable Headers //
#include "cbl/Reflection/BinaryDeserialiser.h"
#include "cbl/Reflection/BinarySerialiser.h"

namespace dbl
{
	namespace E
	{
		/***** Level manager events *****/
#ifdef CBL_TPLFUNCTION_PREFERRED_SYNTAX
		typedef cbl::Event<void(const cbl::FileInfo&)>	LevelEvent;	//!< params: level file info.
#else
		typedef cbl::Event1<void,const cbl::FileInfo&>	LevelEvent;
#endif
		typedef LevelEvent	LevelUnload;
		typedef LevelEvent	LevelLoadBegin;
		typedef LevelEvent	LevelLoadEnd;
		typedef LevelEvent	LevelSaveBegin;
		typedef LevelEvent	LevelSaveEnd;
	}

	//! Level manager component.
	//! Handles all level specific objects and functionality such as saving/loading.
	class DBL_API LevelManager :
		public cbl::DrawableGameComponent
	{
	private:
		typedef std::vector<cbl::ObjectID>	LevelObjectList;

	public:
		bool IsLoading( void ) const { return Enabled || Visible; }

	public:
		cbl::TimeReal		MaxLoadBatchTime;	//!< Maximum time that the level manager can use to load objects per draw frame.

	public:
		E::LevelUnload		OnLevelUnload;
		E::LevelLoadBegin	OnLevelLoadBegin;
		E::LevelLoadEnd		OnLevelLoadEnd;
		E::LevelSaveBegin	OnLevelSaveBegin;
		E::LevelSaveEnd		OnLevelSaveEnd;

		typedef LevelObjectList::iterator				iterator;
		typedef LevelObjectList::const_iterator			const_iterator;
		typedef LevelObjectList::reverse_iterator		reverse_iterator;
		typedef LevelObjectList::const_reverse_iterator	const_reverse_iterator;

	public:
		//! Explicit constructor.
		//! @param	game	Game pointer.
		explicit LevelManager( cbl::Game& game );
		//! Destructor.
		virtual ~LevelManager();
		//! Abstract function to initialise component (from GameComponent).
		virtual void Initialise( void );
		//! Abstract function to shut down component (from GameComponent).
		virtual void Shutdown( void );
		//! Pure virtual update function (from IUpdatable).
		virtual void Update( const cbl::GameTime & time );
		//! Pure virtual draw function (from IDrawable).
		virtual void Draw( const cbl::GameTime & time );
		//! Load new level.
		//! @tparam	DESERIALISER_TYPE	Deserialiser type. e.g. YAMLDeserialiser, BinaryDeserialiser.
		template< typename DESERIALISER_TYPE >
		void Load( const cbl::Char* file, bool unload = true );
		//! Save current level.
		//! @tparam	SERIALISER_TYPE		Serialiser type. e.g. YAMLSerialiser, BinarySerialiser.
		template< typename SERIALISER_TYPE >
		void Save( const cbl::Char* file ) const;
		//! Unload current level.
		void Unload( void );
		//! Begin iterator for level object IDs.
		iterator begin( void ) { return mLevelObjects.begin(); }
		//! Begin iterator for level object IDs.
		const_iterator begin( void ) const { return mLevelObjects.begin(); }
		//! Begin iterator for level object IDs.
		reverse_iterator rbegin( void ) { return mLevelObjects.rbegin(); }
		//! Begin iterator for level object IDs.
		const_reverse_iterator rbegin( void ) const { return mLevelObjects.rbegin(); }
		//! End iterator for level object IDs.
		iterator end( void ) { return mLevelObjects.end(); }
		//! End iterator for level object IDs.
		const_iterator end( void ) const { return mLevelObjects.end(); }
		//! End iterator for level object IDs.
		reverse_iterator rend( void ) { return mLevelObjects.rend(); }
		//! End iterator for level object IDs.
		const_reverse_iterator rend( void ) const { return mLevelObjects.rend(); }

	private:
		//! Setup the necessary variables for loading a file.
		void SetupLoad( const cbl::Char* file, bool unload );
		//! Used by LevelObject to add itself to the manager.
		void Add( LevelObject* obj );
		//! Used by LevelObject to remove itself from the manager.
		void Remove( LevelObject* obj );
		//! Called when the level is completely loaded.
		//! Defined in the LevelManager_Serialisers.cpp file to do the necessary clean up operations.
		void _LoadDone( void );

	private:
		mutable cbl::Serialiser*	mSerialiser;
		mutable cbl::Deserialiser*	mDeserialiser;
		cbl::Uint32					mUnloadWait;
		mutable cbl::FileInfo		mLoadedLevel;
		LevelObjectList				mLevelObjects;
		friend class				LevelObject;
	};

	// Compile error by default.
	template< typename DESERIALISER_TYPE >
	void LevelManager::Load( const cbl::Char*, bool ) {
		CBL_STATIC_ASSERT( false );
	}

	// Compile error by default.
	template< typename SERIALISER_TYPE >
	void LevelManager::Save( const cbl::Char* ) const {
		CBL_STATIC_ASSERT( false );
	}

	//! YAML level deserialiser.
	template<> 
	DBL_API void LevelManager::Load<YAMLDeserialiser>( const cbl::Char* file, bool unload );
	//! YAML level serialiser.
	template<> 
	DBL_API void LevelManager::Save<YAMLSerialiser>( const cbl::Char* file ) const;
	//! Binary level deserialiser.
	template<> 
	DBL_API void LevelManager::Load<cbl::BinaryDeserialiser>( const cbl::Char* file, bool unload );
	//! Binary level serialiser.
	template<> 
	DBL_API void LevelManager::Save<cbl::BinarySerialiser>( const cbl::Char* file ) const;
}

CBL_TYPE( dbl::LevelManager, LevelManager );

#endif // __DBL_LEVELMANAGER_H_
