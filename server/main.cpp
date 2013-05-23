#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <csignal>

#include "request_manager.hpp"
#include "fcgi_server.hpp"
#include "signals.hpp"
#include "echo_handler.hpp"
#include "handler_factory.hpp"

void error_log(const char* msg) {
	using namespace std;
	using namespace boost;
//	static ofstream error;
//	if (!error.is_open()) {
//		error.open("/tmp/errlog", ios_base::out | ios_base::app);
//		error.imbue(locale(error.getloc(), new posix_time::time_facet()));
//	}
	std::cout << '[' << posix_time::second_clock::local_time() << "] " << msg << std::endl;
}

int main() {
	using namespace tride;
	Signals::init();
	RequestManager::registerHandler("echo", getFactory<EchoHandler>());
	std::cout << "I'am started now" << std::endl;
	try {
		FcgiServer server;
		boost::thread t(boost::bind(&boost::asio::io_service::run, &server.io()));
		server.run();
		std::cout << "Waiting for threads" << std::endl;
		t.join();
		std::cout << "All threads exited" << std::endl;
	} catch (std::exception& e) {
		error_log(e.what());
	}
	std::cout << "I'am exiting" << std::endl;
	return 0;
}

