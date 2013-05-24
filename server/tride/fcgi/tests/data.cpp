#define BOOST_TEST_MODULE main

#include <iostream>
#include <boost/test/included/unit_test.hpp>

#include <tride/fcgi/data.hpp>

using namespace tride::fcgi;
/**
 * This test checks how the data is constructed
 */
BOOST_AUTO_TEST_CASE( data_initialization ) {
	{
		Data d("Hello!");
		BOOST_CHECK( !d.isNull() );
		BOOST_CHECK_EQUAL(d.getLength(), 6u);
		BOOST_REQUIRE( d.getData() != NULL );
		BOOST_CHECK_EQUAL( d.toString(), "Hello!" );
	}
	{
		Data d("Hello!\0World");
		BOOST_CHECK( !d.isNull() );
		BOOST_CHECK_EQUAL(d.getLength(), 6u);
		BOOST_REQUIRE( d.getData() != NULL );
		BOOST_CHECK_EQUAL( d.toString(), "Hello!" );
	}
	{
		Data d("Hello!\0World");
		BOOST_CHECK( !d.isNull() );
		BOOST_CHECK_EQUAL(d.getLength(), 6u);
		BOOST_REQUIRE( d.getData() != NULL );
		BOOST_CHECK_EQUAL( d.toString(), "Hello!" );
	}
	{
		Data d("Hello!\0World", 12u);
		BOOST_CHECK( !d.isNull() );
		BOOST_CHECK_EQUAL(d.getLength(), 12u);
		BOOST_REQUIRE( d.getData() != NULL );
		BOOST_CHECK_EQUAL( d.toString(), std::string("Hello!\0World", 12u) );
	}
	{
		Data d("Hello!\0World", 9u);
		BOOST_CHECK( !d.isNull() );
		BOOST_CHECK_EQUAL(d.getLength(), 9u);
		BOOST_REQUIRE( d.getData() != NULL );
		BOOST_CHECK_EQUAL( d.toString(), std::string("Hello!\0Wo", 9u) );
	}
	{
		Data d("");
		BOOST_CHECK( !d.isNull() );
		BOOST_CHECK_EQUAL(d.getLength(), 0u);
		BOOST_REQUIRE( d.getData() != NULL );
	}
	{
		Data d(NULL);
		BOOST_CHECK( d.isNull() );
		BOOST_CHECK_EQUAL(d.getLength(), 0u);
		BOOST_CHECK( d.getData() == NULL );
	}
}

BOOST_AUTO_TEST_CASE( data_comparation ) {
	{
		Data d(NULL);
		BOOST_CHECK( d == d );
		BOOST_CHECK( d == Data::nullData() );
		BOOST_CHECK( d != Data("") );
		BOOST_CHECK( Data::nullData() != Data("") );
	}

	{ // Comparing datas
		BOOST_CHECK( Data("Hello")  == Data("Hello") );
		BOOST_CHECK( Data("Hello")  != Data("World") );
		BOOST_CHECK( Data("Hello")  != Data("Hello!") );
		BOOST_CHECK( Data("Hello!") != Data("Hello") );
		BOOST_CHECK( Data("Hello!", 5u) == Data("Hello") );
	}

	{ // Comparing Data to const char *
		BOOST_CHECK( Data("Hello")  == "Hello" );
		BOOST_CHECK( Data("Hello")  != "World" );
		BOOST_CHECK( Data("Hello")  != "Hello!" );
		BOOST_CHECK( Data("Hello!") != "Hello" );
		BOOST_CHECK( Data("Hello!", 5u) == "Hello" );
		BOOST_CHECK( Data::nullData() != "Hello" );
		BOOST_CHECK( Data::nullData() != "" );
		BOOST_CHECK( Data::nullData() == NULL );
	}

	{ // Comparing Data to std::string
		BOOST_CHECK( Data("Hello")  == std::string("Hello") );
		BOOST_CHECK( Data("Hello")  != std::string("World") );
		BOOST_CHECK( Data("Hello")  != std::string("Hello!") );
		BOOST_CHECK( Data("Hello!") != std::string("Hello") );
		BOOST_CHECK( Data("Hello!", 5u) == std::string("Hello") );
		BOOST_CHECK( Data::nullData() != std::string("Hello") );
		BOOST_CHECK( Data::nullData() != std::string() );
	}
}

BOOST_AUTO_TEST_CASE( ostream_operator ) {
	{
		std::ostringstream out;
		out << "Data=" << Data("");
		BOOST_CHECK_EQUAL(out.str(), "Data=");
	}
	{
		std::ostringstream out;
		out << "Data=" << Data("Hello");
		BOOST_CHECK_EQUAL(out.str(), "Data=Hello");
	}
	{
		std::ostringstream out;
		out << "Data=" << Data("Hello", 3u);
		BOOST_CHECK_EQUAL(out.str(), "Data=Hel");
	}
	{
		std::ostringstream out;
		out << "Data=" << Data(NULL);
		BOOST_CHECK_EQUAL(out.str(), "Data=<NULL>");
	}
}
