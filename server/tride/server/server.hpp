#ifndef TRIDE_SERVER_HPP_
#define TRIDE_SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace tride {
class Processor;

namespace fcgi {
class Request;
} // namespace fcgi

namespace log {
class Logger;
} // namespace log

class Server {
	boost::asio::io_service io;
	Processor& processor;
	log::Logger& logger;

	// non copy
	Server(const Server&);
	Server operator=(const Server&);
public:
	typedef boost::function<void(void)> task_t;

	Server(Processor& p, log::Logger& l):processor(p), logger(l) {}
	void run(const size_t numberOfThreads);
	void postTask(const task_t& callback);
};
} // namespace

#endif /* TRIDE_SERVER_HPP_ */
