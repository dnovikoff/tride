#ifndef TRIDE_SERVER_HPP_
#define TRIDE_SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace tride {
namespace fcgi {
class Request;
} // namespace fcgi

class Server {
	boost::asio::io_service io;
public:
	typedef boost::shared_ptr<fcgi::Request> RequestPtr;

	typedef boost::function<void(void)> task_t;
	typedef boost::function<void(RequestPtr)> callback_t;

	void run(const size_t numberOfThreads, const callback_t& callback);

	void postTask(const task_t& callback);
};
} // namespace

#endif /* TRIDE_SERVER_HPP_ */
