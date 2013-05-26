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

	const std::string& levelStr(const LogLevel level) {
		static std::string noteTxt("NTE");
		static std::string errorTxt("ERR");
		return level == LogLevel::ERROR? errorTxt : noteTxt;
	}
public:
	TestMessage(const LogLevel l, const std::string& m):level(l), message(m) {}
	void print() {
		std::cout << "[Message " << levelStr(level) << "] " << message << std::endl;
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
public:
	void write(const LogLevel level, const std::string& message) override {
		messages.push_back(TestMessage(level, message));
	}
	const MessageList& getMessages() const { return messages; }
	void clear() { messages.clear(); }
};

int throwException() {
	throw std::runtime_error("int throwException()");
}

} // namespace

BOOST_AUTO_TEST_CASE( logger ) {
	{
		TestLogger logger;
		logger << "Hello!";
		MessageList et;
		et += TestMessage(LogLevel::NOTE, "Hello!");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
		logger.clear();
		BOOST_REQUIRE( logger.getMessages().empty() );
		logger.note() << "Hello!";
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
		logger.note() << "Hello!";
		logger.error() << "World!";
		MessageList et;
		et += TestMessage(LogLevel::NOTE, "Hello!");
		et += TestMessage(LogLevel::ERROR, "World!");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
	// Check for exeption, while constructing output
	{
		TestLogger logger;
		BOOST_CHECK_THROW( logger.note() << "Hello " << throwException() << " World", std::runtime_error );
		MessageList et;
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
		BOOST_CHECK_THROW( logger.note() << throwException(), std::runtime_error );
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
	{
		TestLogger logger;
		try {
			throw std::runtime_error("test");
		} catch(const std::runtime_error& x) {
			TestLogger logger;
			logger.write(LogLevel::NOTE, "Hello");
			MessageList et;
			et += TestMessage(LogLevel::NOTE, "Hello");
			BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
		}
	}
	// Direct write
	{
		TestLogger logger;
		logger.write(LogLevel::NOTE, "Hello");
		logger.write(LogLevel::ERROR, "World");

		MessageList et;
		et += TestMessage(LogLevel::NOTE, "Hello");
		et += TestMessage(LogLevel::ERROR, "World");
		BOOST_CHECK( compareAndPrint( logger.getMessages(), et) );
	}
}
