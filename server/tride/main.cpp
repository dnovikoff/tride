#include <boost/shared_ptr.hpp>

#include <tride/server/server.hpp>
#include <tride/fcgi/request.hpp>


void run(const boost::shared_ptr<tride::fcgi::Request>& x) {
	tride::fcgi::Request& request = *x;
	request << "Content-type: text/html\r\n\r\nHello v2";
}

int main() {
	tride::Server server;
	server.run( 10u, &run );
	return 0;
}
