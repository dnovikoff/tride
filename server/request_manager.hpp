#ifndef REQUEST_MANAGER_HPP_
#define REQUEST_MANAGER_HPP_

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <fastcgi++/request.hpp>
#include "handler_factory.hpp"

namespace tride {

class RequestManager: public Fastcgipp::Request<char> {
	enum State { START, PROCESS } state;
	virtual bool response();
public:
	RequestManager();
	virtual ~RequestManager();
	static void registerHandler(const std::string& path, HandlerFactory);
	static HandlerPtr createHandler(const std::string& path);
};

} // namespace tride

#endif /* REQUEST_MANAGER_HPP_ */