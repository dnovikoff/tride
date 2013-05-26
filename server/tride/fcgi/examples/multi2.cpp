#include <iostream>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
/**
 * In this example the request is accepted by a main thread and processed by mutiple threads
 * The number of processing threads could be any
 * A server accepts 5 requests and dies
 */

#include <boost/make_shared.hpp>

#include <tride/fcgi/exceptions.hpp>
#include <tride/fcgi/request.hpp>

using namespace tride::fcgi;

namespace {

void signalHandler( int signum ) {
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
}

void initSignals() {
	Request::installSignal(SIGINT, signalHandler);
}

} // namespace

void run(const boost::shared_ptr<Request>& r) {
	*r << "Content-type: text/html\r\n\r\nHello World!";
	std::cout << "Done" << std::endl;
}

int mainBody() {
	initSignals();
	boost::thread_group tg;
	boost::asio::io_service io;

	{
		boost::asio::io_service::work work( io );

		const size_t numberOfThreads = 3;

		// We will start 3 threads, to process task, posted at io
		for(size_t i = 0; i < numberOfThreads; ++i) {
			tg.create_thread(boost::bind(&boost::asio::io_service::run, &io));
		}

		size_t i = 0;
		while(1) {
			boost::shared_ptr<Request> r = boost::make_shared<Request>();
			std::cout << "Acception request " << ++i << std::endl;
			if( !r->accept() ) {
				std::cout << "Server stopped" << std::endl;
				break;
			}
			io.post(boost::bind(&run, r));
		}
		// No new reqests are accepted at this point
		// The io_service::work objects destructs here. The io.run(s) will exit, when all run out of tasks
	}
	std::cout << "Waiting for exit" << std::endl;
	// Waiting untill will run out of task and all thread will exit
	tg.join_all();
	std::cout << "Exit" << std::endl;
	return 0;
}

int main() {
	int r = 0;
	try {
		r = mainBody();
	} catch(const Exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		r = 1;
	}
	return r;
}
