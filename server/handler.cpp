#include <iostream>

#include "handler.hpp"
#include "fcgi_server.hpp"

bool Handler::response() {
	switch (state) {
	case START: {
		std::cout << "New request" << std::endl;
		out << "Content-Type: text/html; charset=ISO-8859-1\r\n\r\n";
		out
				<< "<html><head><meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1' />";
		out << "<title>fastcgi++: Threaded Timer</title></head><body>";
		out << "Starting Timer...<br />";
		out.flush();
		t.reset(
				new boost::asio::deadline_timer(FcgiServer::currentIo(),
						boost::posix_time::seconds(1)));
		Fastcgipp::Message msg;
		msg.type = 1;
		{
			char cString[] = "I was passed between two threads!!";
			msg.size = sizeof(cString);
			msg.data.reset(new char[sizeof(cString)]);
			std::strncpy(msg.data.get(), cString, sizeof(cString));
		}
		t->async_wait(boost::bind(callback(), msg));
		state = FINISH;
		return false;
	}
	case FINISH: {
		std::cout << "Working with request" << std::endl;
		out << "Timer Finished! Our message data was \"some\""
				<< environment().scriptName;
		out << "</body></html>";
		return true;
	}
	}
	return true;
}

Handler::Handler(): state(START) {}

Handler::~Handler() {}

