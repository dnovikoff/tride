#include <iostream>
#include <tride/fcgi/exceptions.hpp>
#include <tride/fcgi/request.hpp>

using namespace tride::fcgi;

int mainBody() {
	Request::Init();
	Request r;
	const size_t cnt = 5;
	for(size_t i = 0; i < cnt; ++i) {
		std::cout << "Acception request " << i << std::endl;
		r.accept();
		std::cout << "Accepted" << std::endl;
		r << "Content-type: text/html\r\n\r\nHello World!";
		std::cout << "Done" << std::endl;
	}
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
