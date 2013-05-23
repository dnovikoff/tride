#ifndef FCGI_SERVER_HPP_
#define FCGI_SERVER_HPP_

#include <boost/asio.hpp>
#include <fastcgi++/manager.hpp>

#include "request_manager.hpp"

namespace tride {

class FcgiServer {
	static FcgiServer* current;

	Fastcgipp::Manager<RequestManager> fcgi;
	boost::asio::io_service service;
	boost::scoped_ptr<boost::asio::io_service::work> work;
	FcgiServer& operator=(const FcgiServer&);
	FcgiServer(const FcgiServer&);
public:
	FcgiServer();
	~FcgiServer();

	void run();
	void stop();
	boost::asio::io_service& io();
	static boost::asio::io_service& currentIo();
	static void currentStop();
};

} // namespace tride

#endif /* FCGI_SERVER_HPP_ */
