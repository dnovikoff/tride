#define BOOST_TEST_MODULE main

#include <iostream>
#include <list>

#include <boost/assign.hpp>

#include <boost/test/included/unit_test.hpp>

#include <tride/log/logger.hpp>

using namespace boost::assign;
using namespace tride::log;

namespace {

class TestMessage {
	LogLevel level;
	std::string message;
public:
	TestMessage(const LogLevel l, const std::string& m):level(l), message(m) {}
	void print() {
		std::cout << "[Level=" << level << "] " << message << std::endl;
	}
	bool operator==(const TestMessage& rhs) const {
		if(level != rhs.level) return false;
		return message == rhs.message;
	}

	bool operator!=(const TestMessage& rhs) const { return !operator ==(rhs); }
};

typedef std::list<TestMessage> MessageList;

void printMessages(const MessageList& mlist) {
	size_t i = 0;
	for(auto m: mlist) {
		std::cout << "[" << i++ << "]";
		m.print();
	}
}

bool compareMessages(const MessageList& lhs, const MessageList& rhs) {
	if(lhs.size() != rhs.size()) return false;
	return std::equal(rhs.begin(), rhs.end(), lhs.begin());
}

void printDiff(const MessageList& messages, const MessageList& ethalon) {
	std::cout << "Collections are not equal" << std::endl;
	std::cout << "=== Messages from test are:" << std::endl;
	printMessages( messages );
	std::cout << "=== Ethalon messages are:" << std::endl;
	printMessages( ethalon  );
}

bool compareAndPrint(const MessageList& messages, const MessageList& ethalon) {
	bool r = compareMessages(messages, ethalon);
	if(!r) printDiff( messages, ethalon );
	return r;
}

class TestLogger: public Logger {
	MessageList messages;

	void doWrite(const LogLevel level, const std::string& message) override {
		messages.push_back(TestMessage(level, message));
	}
public:
	TestLogger(const LogLevel th = defaultThreshold()):Logger(th) {}
	const MessageList& getMessages() const { return messages; }
	void clear() { messages.clear(); }
};

int throwException() {
	throw std::runtime_error("int throwException()");
}

} // namespace

BOOST_AUTO_TEST_CASE( basic ) {
	{
		TestLogger logger;
		logger << "Hello!";
		MessageList et;
		et += TestMessage(LogLevel::INFO, "Hello!");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
		logger.clear();
		BOOST_REQUIRE( logger.getMessages().empty() );
		logger.info() << "Hello!";
	}
	{
		TestLogger logger;
		logger.error() << "Hello!";
		MessageList et;
		et += TestMessage(LogLevel::ERROR, "Hello!");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
	{
		TestLogger logger;
		logger.info() << "Hello!";
		logger.error() << "World!";
		MessageList et;
		et += TestMessage(LogLevel::INFO, "Hello!");
		et += TestMessage(LogLevel::ERROR, "World!");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
	// Check for exeption, while constructing output
	{
		TestLogger logger;
		BOOST_CHECK_THROW( logger.info() << "Hello " << throwException() << " World", std::runtime_error );
		MessageList et;
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
		BOOST_CHECK_THROW( logger.info() << throwException(), std::runtime_error );
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
	{
		TestLogger logger;
		try {
			throw std::runtime_error("test");
		} catch(const std::runtime_error& x) {
			TestLogger logger;
			logger.write(LogLevel::INFO, "Hello");
			MessageList et;
			et += TestMessage(LogLevel::INFO, "Hello");
			BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
		}
	}
	// Direct write
	{
		TestLogger logger;
		logger.write(LogLevel::INFO, "Hello");
		logger.write(LogLevel::ERROR, "World");

		MessageList et;
		et += TestMessage(LogLevel::INFO, "Hello");
		et += TestMessage(LogLevel::ERROR, "World");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
}

BOOST_AUTO_TEST_CASE( checkLevel ) {
	{
		TestLogger logger;
		BOOST_CHECK( logger.checkLevel(LogLevel::FATAL) );
		BOOST_CHECK( logger.checkLevel(LogLevel::ERROR) );
		BOOST_CHECK( logger.checkLevel(LogLevel::WARNING) );
		BOOST_CHECK( logger.checkLevel(LogLevel::INFO) );
		BOOST_CHECK( !logger.checkLevel(LogLevel::DEBUG) );
		BOOST_CHECK( !logger.checkLevel(LogLevel::TRACE) );
	}
	{
		TestLogger logger(LogLevel::FATAL);
		BOOST_CHECK( logger.checkLevel(LogLevel::FATAL) );
		BOOST_CHECK( !logger.checkLevel(LogLevel::ERROR) );
		BOOST_CHECK( !logger.checkLevel(LogLevel::WARNING) );
		BOOST_CHECK( !logger.checkLevel(LogLevel::INFO) );
		BOOST_CHECK( !logger.checkLevel(LogLevel::DEBUG) );
		BOOST_CHECK( !logger.checkLevel(LogLevel::TRACE) );
	}
}

BOOST_AUTO_TEST_CASE( threshold ) {
	{
		TestLogger logger(LogLevel::WARNING);
		logger.error()    << "error";
		logger.info()    << "info";
		logger.warning() << "warning";
		logger.trace()   << "trace";
		logger.debug()   << "debug";
		logger.fatal()   << "fatal";

		MessageList et;
		et += TestMessage(LogLevel::ERROR, "error");
		et += TestMessage(LogLevel::WARNING, "warning");
		et += TestMessage(LogLevel::FATAL, "fatal");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
	// Default level test
	{
		TestLogger logger;
		logger.error()    << "error";
		logger.info()    << "info";
		logger.warning() << "warning";
		logger.trace()   << "trace";
		logger.debug()   << "debug";
		logger.fatal()   << "fatal";

		MessageList et;
		et += TestMessage(LogLevel::ERROR, "error");
		et += TestMessage(LogLevel::INFO, "info");
		et += TestMessage(LogLevel::WARNING, "warning");
		et += TestMessage(LogLevel::FATAL, "fatal");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
	// All levels test
	{
		TestLogger logger(LogLevel::TRACE);
		logger.error()    << "error";
		logger.info()    << "info";
		logger.warning() << "warning";
		logger.trace()   << "trace";
		logger.debug()   << "debug";
		logger.fatal()   << "fatal";

		MessageList et;
		et += TestMessage(LogLevel::ERROR, "error");
		et += TestMessage(LogLevel::INFO, "info");
		et += TestMessage(LogLevel::WARNING, "warning");
		et += TestMessage(LogLevel::TRACE, "trace");
		et += TestMessage(LogLevel::DEBUG, "debug");
		et += TestMessage(LogLevel::FATAL, "fatal");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
}

BOOST_AUTO_TEST_CASE( outstream ) {
	{
		TestLogger logger;
		logger << "Hello " << 123 << " World " << std::string("std::string");
		MessageList et;
		et += TestMessage(LogLevel::INFO, "Hello 123 World std::string");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
}
