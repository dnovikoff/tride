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

void run(const boost::shared_ptr<Request>& r) {
	*r << "Content-type: text/html\r\n\r\nHello World!";
	std::cout << "Done" << std::endl;
}

int mainBody() {
	Request::Init();
	boost::thread_group tg;
	boost::asio::io_service io;

	{
		boost::asio::io_service::work work( io );

		for(size_t i = 0; i < 3; ++i) {
			tg.create_thread(boost::bind(&boost::asio::io_service::run, &io));
		}

		for(size_t i = 0; i < 5; ++i) {
			boost::shared_ptr<Request> r = boost::make_shared<Request>();
			std::cout << "Acception request " << i << std::endl;
			r->accept();
			io.post(boost::bind(&run, r));
		}
	}
	std::cout << "Waiting for exit" << std::endl;
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
