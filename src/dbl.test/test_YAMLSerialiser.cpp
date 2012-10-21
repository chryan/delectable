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
 * @file test_YAMLSerialiser.cpp
 * @brief Unit testing for the YAML serialiser.
 */

// Precompiled Headers //
#include <dbl/StdAfx.h>

// Delectable Headers //
#include <dbl/Serialisation/YAMLSerialiser.h>
#include <dbl/Serialisation/YAMLDeserialiser.h>

// Google Test //
#include <gtest/gtest.h>

#include <yaml-cpp/yaml.h>

using namespace dbl;

namespace cbl
{
	struct DummAccessYAML {};
	template<>
	DummAccessYAML* EntityManager::New<DummAccessYAML>( void ) const
	{
		this->EntityManager::~EntityManager();
		this->EntityManager::EntityManager();
		cbl::CblRegistrar::RegisterCblTypes();
		dbl::DblRegistrar::RegisterDblTypes();
		return NULL;
	}
}
void ForceReconstructEntityManager_YAML( void )
{
	using namespace cbl;
	const_cast<EntityManager*>( EntityManager::InstancePtr() )->New<DummAccessYAML>();
}

struct BaseClassTest
	: public cbl::Entity
{
	virtual ~BaseClassTest() {}
	cbl::Int32	T1;
	cbl::Int32	T2;
	BaseClassTest() : T1( 0 ), T2( 0 ) {}
	BaseClassTest( cbl::Int32 t1, cbl::Int32 t2 )
		: T1( t1 ), T2( t2 ) {}

	virtual cbl::Entity::OPTIONS OnPreChanged( void ) { return cbl::Entity::O_NORMAL; }
	virtual void OnChanged( void ) {}
	virtual cbl::Entity::OPTIONS OnPreSaved( void ) const { return cbl::Entity::O_NORMAL; }
	virtual void OnSaved( void ) const {}
};

struct ChildClassTest : public BaseClassTest
{
	virtual ~ChildClassTest() {}
	cbl::Int32	T3;
	cbl::Int32	T4;
	ChildClassTest() : T3( 0 ), T4( 0 ) {}
	ChildClassTest( cbl::Int32 t1, cbl::Int32 t2, cbl::Int32 t3, cbl::Int32 t4 )
		: BaseClassTest( t1, t2 ), T3( t3 ), T4( t4 ) {}
};

struct ContainerTest
{
	typedef std::map<cbl::Int8,BaseClassTest*> MapType;
	cbl::Int32					Number;
	cbl::Int32					HexNumber;
	std::vector<BaseClassTest*>	Vector;
	MapType						Map;

	ContainerTest() {
	}

	~ContainerTest() {
		for( size_t t = 0; t < Vector.size(); ++t ) {
			CBL_DELETE( Vector[t] );
		}
		CBL_FOREACH( MapType, it, Map ) {
			CBL_DELETE( it->second );
		}
	}
};

CBL_TYPE( BaseClassTest, BaseClassTest );
CBL_TYPE( ChildClassTest, ChildClassTest );
CBL_TYPE( ContainerTest, ContainerTest );

class YAMLSerialiserFixture : public ::testing::Test
{
public:
	YAMLSerialiserFixture()
	{
	}

	void SetUp()
	{
		CBL_ENT.Types.Create<BaseClassTest>()
			.CBL_FIELD( T1, BaseClassTest )
			.CBL_FIELD( T2, BaseClassTest );
		CBL_ENT.Types.Create<ChildClassTest>()
			.Base<BaseClassTest>()
			.CBL_FIELD( T3, ChildClassTest )
			.CBL_FIELD( T4, ChildClassTest );
		CBL_ENT.Types.Create<ContainerTest>()
			.CBL_FIELD( Number, ContainerTest )
			.CBL_FIELD_ATTR( HexNumber, ContainerTest, cbl::FieldAttr::F_HEX )
			.CBL_FIELD_ATTR( Vector, ContainerTest, cbl::FieldAttr::F_INLINE )
			.CBL_FIELD( Map, ContainerTest );

		Container.Number = 50;
		Container.HexNumber = 100;

		cbl::Int32 t = 0;

		for( size_t i = 0; i < 5; ++i ) {
			if( i % 2 ) {
				BaseClassTest* b = CBL_ENT.New<BaseClassTest>();
				b->T1 = t++;
				b->T2 = t++;
				Container.Vector.push_back( b );
			}
			else {
				ChildClassTest* c = CBL_ENT.New<ChildClassTest>();
				c->T1 = t++;
				c->T2 = t++;
				c->T3 = t++;
				c->T4 = t++;
				Container.Vector.push_back( c );
			}
		}

		for( size_t i = 0; i < 5; ++i ) {
			if( i % 2 ) {
				BaseClassTest* b = CBL_ENT.New<BaseClassTest>();
				b->T1 = t++;
				b->T2 = t++;
				Container.Map.insert( std::make_pair( t, b ) );
			}
			else {
				ChildClassTest* c = CBL_ENT.New<ChildClassTest>();
				c->T1 = t++;
				c->T2 = t++;
				c->T3 = t++;
				c->T4 = t++;
				Container.Map.insert( std::make_pair( t, c ) );
			}

			++t;
		}
	}

	void TearDown()
	{
		ForceReconstructEntityManager_YAML();
	}

public:
	ContainerTest Container;
};

TEST_F( YAMLSerialiserFixture, YAMLSerialiser_OutputTest )
{
	YAML::Emitter e;

	cbl::Vector3f vec( 1.0f, 2.0f, 3.0f );
	YAMLSerialiser s;
	s
		.SetStream( e )
		.Serialise( Container )
		.Serialise( vec );
	s.Output( "output.yaml" );

	std::ifstream i( "output.yaml" );
	YAML::Parser parser( i );

	YAML::Node doc;
	ASSERT_TRUE( parser.GetNextDocument(doc) );

	cbl::Int32 tval = 0;
	char t_char[255] = { '\0' };
	// Container testing.
	{
		const YAML::Node* base = &doc;
		ASSERT_TRUE( base->Tag() == "!ContainerTest" );
		const YAML::Node* number	= base->FindValue( "Number" );
		const YAML::Node* vector	= base->FindValue( "Vector" );
		const YAML::Node* map		= base->FindValue( "Map" );
		ASSERT_TRUE( number != NULL && vector != NULL && map != NULL );

		size_t i = 0;
		for( YAML::Iterator it = vector->begin(); it != vector->end(); ++it, ++i ) {
			if( i % 2 ) {
				const YAML::Node* baseClass = &(*it);
				ASSERT_TRUE( baseClass != NULL );
				ASSERT_TRUE( baseClass->Tag() == "!BaseClassTest" || baseClass->Tag() == "?" );
				const YAML::Node* t1 = baseClass->FindValue( "T1" );
				const YAML::Node* t2 = baseClass->FindValue( "T2" );
				ASSERT_TRUE( t1 != NULL && t2 != NULL && baseClass->FindValue( "T3" ) == NULL );

				std::string t[2];
				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t1).GetScalar( t[0] ) );
				ASSERT_STREQ( t[0].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t2).GetScalar( t[1] ) );
				ASSERT_STREQ( t[1].c_str(), t_char );
			}
			else {
				const YAML::Node* baseClass = &(*it);
				ASSERT_TRUE( baseClass != NULL );
				ASSERT_TRUE( baseClass->Tag() == "!ChildClassTest" );
				const YAML::Node* t1 = baseClass->FindValue( "T1" );
				const YAML::Node* t2 = baseClass->FindValue( "T2" );
				const YAML::Node* t3 = baseClass->FindValue( "T3" );
				const YAML::Node* t4 = baseClass->FindValue( "T4" );
				ASSERT_TRUE( t1 != NULL && t2 != NULL && t3 != NULL && t4 != NULL && baseClass->FindValue("T5") == NULL );

				std::string t[4];
				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t1).GetScalar( t[0] ) );
				ASSERT_STREQ( t[0].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t2).GetScalar( t[1] ) );
				ASSERT_STREQ( t[1].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t3).GetScalar( t[2] ) );
				ASSERT_STREQ( t[2].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t4).GetScalar( t[3] ) );
				ASSERT_STREQ( t[3].c_str(), t_char );
			}
		}

		i = 0;
		for( YAML::Iterator it = map->begin(); it != map->end(); ++it, ++i ) {
			if( i % 2 ) {
				const YAML::Node* baseClass = (*it).FindValue( "Value" );
				ASSERT_TRUE( baseClass != NULL );
				ASSERT_TRUE( baseClass->Tag() == "!BaseClassTest" || baseClass->Tag() == "?" );
				const YAML::Node* t1 = baseClass->FindValue( "T1" );
				const YAML::Node* t2 = baseClass->FindValue( "T2" );
				ASSERT_TRUE( t1 != NULL && t2 != NULL && baseClass->FindValue( "T3" ) == NULL );

				std::string t[2];
				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t1).GetScalar( t[0] ) );
				ASSERT_STREQ( t[0].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t2).GetScalar( t[1] ) );
				ASSERT_STREQ( t[1].c_str(), t_char );
			}
			else {
				const YAML::Node* baseClass = (*it).FindValue( "Value" );
				ASSERT_TRUE( baseClass != NULL );
				ASSERT_TRUE( baseClass->Tag() == "!ChildClassTest" );
				const YAML::Node* t1 = baseClass->FindValue( "T1" );
				const YAML::Node* t2 = baseClass->FindValue( "T2" );
				const YAML::Node* t3 = baseClass->FindValue( "T3" );
				const YAML::Node* t4 = baseClass->FindValue( "T4" );
				ASSERT_TRUE( t1 != NULL && t2 != NULL && t3 != NULL && t4 != NULL && baseClass->FindValue("T5") == NULL );

				std::string t[4];
				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t1).GetScalar( t[0] ) );
				ASSERT_STREQ( t[0].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t2).GetScalar( t[1] ) );
				ASSERT_STREQ( t[1].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t3).GetScalar( t[2] ) );
				ASSERT_STREQ( t[2].c_str(), t_char );

				sprintf( t_char, "%d", tval++ );
				ASSERT_TRUE( (*t4).GetScalar( t[3] ) );
				ASSERT_STREQ( t[3].c_str(), t_char );
			}

			const YAML::Node* baseClass = (*it).FindValue( "Key" );
			ASSERT_TRUE( baseClass != NULL );

			std::string num;
			sprintf( t_char, "%d", tval++ ); 
			ASSERT_TRUE( baseClass->GetScalar( num ) );
			ASSERT_STREQ( num.c_str(), t_char );
		}
	}

	ASSERT_TRUE( parser.GetNextDocument(doc) );
	// Vector3f is the last document.
	ASSERT_FALSE( parser.GetNextDocument(doc) );
}

struct MultipleTest
{
	cbl::Vector3f Position;
	cbl::Vector3f Scale;

	std::vector<cbl::Int32>	VectorInts;
};

CBL_TYPE( MultipleTest, MultipleTest );

TEST( YAMLDeserialiserFixture, YAMLDeserialiser_InputTest )
{
	CBL_ENT.Types.Create<MultipleTest>()
		.CBL_FIELD( Position, MultipleTest )
		.CBL_FIELD( Scale, MultipleTest )
		.CBL_FIELD( VectorInts, MultipleTest );

	MultipleTest mtest; 

	YAML::Emitter e;
	e
		<< YAML::LocalTag( "MultipleTest" )
		<< YAML::BeginMap
		<< YAML::Key << "Position"
		<< YAML::Value
		<< YAML::BeginMap
		<< YAML::Key << "X" << YAML::Value << "1"
		<< YAML::Key << "Y" << YAML::Value << "2"
		<< YAML::Key << "Z" << YAML::Value << "3"
		<< YAML::EndMap
		<< YAML::Key << "Scale"
		<< YAML::Value
		<< YAML::BeginMap
		<< YAML::Key << "X" << YAML::Value << "4"
		<< YAML::Key << "Y" << YAML::Value << "5"
		<< YAML::Key << "Z" << YAML::Value << "6"
		<< YAML::EndMap
		<< YAML::Key << "VectorInts"
		<< YAML::Value
		<< YAML::BeginSeq;
	for( cbl::Int32 i = 0; i < 5; ++i ) {
		e << i;
	}
	// Test type specific additions.
	for( cbl::Int32 i = 5; i < 10; ++i ) {
		e << YAML::LocalTag( "Int32" ) << i;
	}
	// Test invalid additions.
	for( cbl::Int16 i = 0; i < 10; ++i ) {
		e << YAML::LocalTag( "Int16" ) << i;
	}
	e	<< YAML::EndSeq
		<< YAML::EndMap;

	std::istringstream i( e.c_str() );
	YAML::Parser parser( i );

	YAMLDeserialiser()
		.SetStream( parser )
		.Deserialise( mtest );

	ASSERT_EQ( mtest.Position.X, 1.0f );
	ASSERT_EQ( mtest.Position.Y, 2.0f );
	ASSERT_EQ( mtest.Position.Z, 3.0f );
	ASSERT_EQ( mtest.Scale.X, 4.0f );
	ASSERT_EQ( mtest.Scale.Y, 5.0f );
	ASSERT_EQ( mtest.Scale.Z, 6.0f );
	ASSERT_EQ( mtest.VectorInts.size(), 10 );
	for( cbl::Int32 i = 0; i < 10; ++i )
		ASSERT_EQ( i, mtest.VectorInts[i] );

	ForceReconstructEntityManager_YAML();
}

TEST_F( YAMLSerialiserFixture, YAMLDeserialiser_CombinedTest )
{
	YAML::Emitter e;

	cbl::Vector3f vec( 1.0f, 2.0f, 3.0f );
	YAMLSerialiser s;
	s
		.SetStream( e )
		.Serialise( Container );
	s.Output( "output.yaml" );

	std::ifstream i( "output.yaml" );
	YAML::Parser parser( i );

	ContainerTest test;
	YAMLDeserialiser()
		.SetStream( parser )
		.Deserialise( test );

	ASSERT_EQ( Container.Number, test.Number );
	ASSERT_EQ( Container.HexNumber, test.HexNumber );

	cbl::Int32 t = 0;

	for( size_t i = 0; i < test.Vector.size(); ++i ) {
		if( i % 2 ) {
			ASSERT_EQ( test.Vector[i]->T1, t++ );
			ASSERT_EQ( test.Vector[i]->T2, t++ );
		}
		else {
			ASSERT_EQ( test.Vector[i]->T1, t++ );
			ASSERT_EQ( test.Vector[i]->T2, t++ );
			ASSERT_EQ( ((ChildClassTest*)(test.Vector[i]))->T3, t++ );
			ASSERT_EQ( ((ChildClassTest*)(test.Vector[i]))->T4, t++ );
		}
	}

	for( size_t i = 0; i < 5; ++i ) {
		if( i % 2 ) {
			ContainerTest::MapType::iterator it = test.Map.find( t + 2 );
			ASSERT_TRUE( it != test.Map.end() );
			ASSERT_EQ( it->second->T1, t++ );
			ASSERT_EQ( it->second->T2, t++ );
		}
		else {
			ContainerTest::MapType::iterator it = test.Map.find( t + 4 );
			ASSERT_TRUE( it != test.Map.end() );
			ASSERT_EQ( it->second->T1, t++ );
			ASSERT_EQ( it->second->T2, t++ );
			ASSERT_EQ( ((ChildClassTest*)(it->second))->T3, t++ );
			ASSERT_EQ( ((ChildClassTest*)(it->second))->T4, t++ );
		}

		++t;
	}
}

// Chewable Headers //
#include "cbl/Core/Object.h"
#include "cbl/Core/ObjectPart.h"
#include "cbl/Core/ObjectManager.h"

class TestObjectPart :
	public cbl::ObjectPart
{
public:
	TestObjectPart() 
	: Int(50), Float(32.0f)
	{}
	virtual ~TestObjectPart() {}

	virtual void Initialise( void )
	{
		ASSERT_TRUE( this->Object != NULL );
	}

	virtual void Shutdown( void )
	{
	}

	cbl::Int32		Int;
	cbl::Float32	Float;
};

CBL_TYPE( TestObjectPart, TestObjectPart );

TEST( YAMLSerialiserObjects, YAML_ObjectTest )
{
	static const cbl::Uint32 sObjectCount = 10;
	cbl::ObjectManager objMgr;

	YAML::Emitter e;

	CBL_ENT.Types.Create<TestObjectPart>()
		.Base<cbl::ObjectPart>()
		.CBL_FIELD( Int, TestObjectPart )
		.CBL_FIELD( Float, TestObjectPart );

	cbl::Vector3f vec( 1.0f, 2.0f, 3.0f );
	YAMLSerialiser s;
	s
		.SetStream( e )
		.Serialise( vec );

	for( cbl::Uint32 i = 0; i < sObjectCount; ++i ) {
		char objName[255] = { '\0' };
		sprintf( objName, "NewObject_%d", i );
		cbl::Object* newObj = objMgr.Create<cbl::Object>( objName );

		if( i % 2 ) {
			TestObjectPart* top = newObj->Parts.Add<TestObjectPart>();
			top->Int = 10;
			top->Float = 1.0f;
		}

		s.Serialise( *newObj );
		objMgr.Destroy( newObj );
		objMgr.Purge();

		ASSERT_TRUE( objMgr.Get( objName ) == NULL );
	}

	s.Output( "obj.yaml" );


	YAMLDeserialiser yaml;
	std::istringstream is( e.c_str() );
	YAML::Parser parser(is);
	yaml.SetStream( parser );

	while( !yaml.IsStreamEnded() )
		objMgr.DeserialiseObject<cbl::Object>( yaml );

	for( cbl::Uint32 i = 0; i < sObjectCount; ++i ) {
		char objName[255] = { '\0' };
		sprintf( objName, "NewObject_%d", i );
		cbl::ObjectPtr newObj = objMgr.Get( objName );
		ASSERT_TRUE( newObj != NULL );
		if( i % 2 ) {
			TestObjectPart* top = newObj->Parts.Get<TestObjectPart>();
			ASSERT_TRUE( top != NULL );
			ASSERT_EQ( top->Int, 10 );
			ASSERT_EQ( top->Float, 1.0f );
		}
	}

	objMgr.ForceFullPurge();

	ForceReconstructEntityManager_YAML();
}

TEST( YAMLSerialiserPointer, YAML_PointerTest )
{
	cbl::Vector3f vec( 1.0f, 2.0f, 3.0f );
	YAML::Emitter e;
	YAMLSerialiser s;
	s
		.SetStream( e )
		.Serialise( vec );

	YAMLDeserialiser yaml;
	std::istringstream is( e.c_str() );
	YAML::Parser parser(is);
	yaml.SetStream( parser );

	std::cout << e.c_str();
	cbl::Vector3f* v = NULL;
	yaml.DeserialisePtr( v );

	ASSERT_TRUE( v != NULL );
	ASSERT_EQ( v->X, 1.0f );
	ASSERT_EQ( v->Y, 2.0f );
	ASSERT_EQ( v->Z, 3.0f );
	CBL_DELETE( v );
}

struct ParentClassTest
{
	BaseClassTest*	BasePointer;
};

CBL_TYPE( ParentClassTest, ParentClassTest );

TEST( YAMLSerialiserInheritedPointer, YAML_InheriatedPointer )
{
	CBL_ENT.Types.Create<BaseClassTest>()
		.CBL_FIELD( T1, BaseClassTest )
		.CBL_FIELD( T2, BaseClassTest );
	CBL_ENT.Types.Create<ChildClassTest>()
		.Base<BaseClassTest>()
		.CBL_FIELD( T3, ChildClassTest )
		.CBL_FIELD( T4, ChildClassTest );
	CBL_ENT.Types.Create<ParentClassTest>()
		.CBL_FIELD( BasePointer, ParentClassTest );

	ParentClassTest t;
	t.BasePointer = CBL_ENT.New<ChildClassTest>();

	YAML::Emitter e;
	YAMLSerialiser s;
	s
		.SetStream( e )
		.Serialise( t );
	s.Output( "inherited.yaml" );

	CBL_DELETE( t.BasePointer );

	ASSERT_TRUE( t.BasePointer == NULL );

	YAMLDeserialiser d;
	std::ifstream is( "inherited.yaml" );
	YAML::Parser parser(is);
	d
		.SetStream( parser )
		.Deserialise( t );

	ASSERT_TRUE( t.BasePointer != NULL );
	ASSERT_EQ( t.BasePointer->GetType().Name, cbl::TypeCName<ChildClassTest>() );

	CBL_DELETE( t.BasePointer );

	ForceReconstructEntityManager_YAML();
}