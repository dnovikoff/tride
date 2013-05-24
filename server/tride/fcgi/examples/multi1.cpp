/**
 * In this example the request is accepted and proccesed by multiple thread
 * Every thread works the requests whole cycle ( from accepting till returning result)
 * A server accepts 5 requests and dies
 */

#include <iostream>
#include <boost/thread.hpp>

#include <tride/fcgi/exceptions.hpp>
#include <tride/fcgi/request.hpp>

using namespace tride::fcgi;

void run() {
	static boost::mutex mutex;
	Request r;
	{
		boost::mutex::scoped_lock l(mutex);
		std::cout << "Acception request " << std::endl;
		r.accept();
		std::cout << "Accepted" << std::endl;
	}
	r << "Content-type: text/html\r\n\r\nHello World!";
	std::cout << "Done" << std::endl;
}

int mainBody() {
	Request::Init();
	boost::thread_group tg;
	const size_t cnt = 5;
	for(size_t i = 0; i < cnt; ++i) {
		tg.create_thread(&run);
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
