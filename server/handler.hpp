#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <boost/scoped_ptr.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <fastcgi++/request.hpp>

namespace tride {

class Handler: public Fastcgipp::Request<char> {
	enum State {
		START, FINISH
	} state;
	boost::scoped_ptr<boost::asio::deadline_timer> t;
	bool response();
public:
	Handler();
	virtual ~Handler();
};

} // namespace tride

#endif /* HANDLER_HPP_ */
