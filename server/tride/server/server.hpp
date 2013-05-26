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

/**
 * This class encapsulates
 * 1) Start/stop logic
 * 2) Creating threads
 * 3) Way for processing events
 * 4) Accepting requests
 *
 * All requests will be accepted in a thread, from where you call run
 * All required threads will be created
 * All requests will be passed to the Processor in created threads
 */
class Server {
	boost::asio::io_service io;
	Processor& processor;
	log::Logger& logger;

	// non copy
	Server(const Server&);
	Server operator=(const Server&);
public:
	typedef boost::function<void(void)> task_t;

	/**
	 * Class for processing request.
	 */
	Server(Processor& p, log::Logger& l):processor(p), logger(l) {}

	/**
	 * Creates threads and starts listening for incomming requests
	 * This function will run until the server is alive
	 * If function exists - the server is stopped
	 * @param number of threads to create
	 */
	void run(const size_t numberOfThreads);

	/**
	 * We can reuse created threads and io service, to process some other task (like background onces)
	 * Take care of not to block all the threads
	 * Task will be proccesed by one of N created threads
	 * @param task to process
	 */
	void postTask(const task_t& callback);
};
} // namespace

#endif /* TRIDE_SERVER_HPP_ */
