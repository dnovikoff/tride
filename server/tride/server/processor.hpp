#ifndef PROCESSOR_HPP_
#define PROCESSOR_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace tride {

namespace fcgi {
class Request;
} // namespace fcgi

namespace log {
class Logger;
} // namespace log

/**
 * Class, describing how to process events
 */
class Processor {
	typedef Processor self_t;
	// Non copyable
	Processor(const Processor&);
	Processor& operator=(const Processor&);

	log::Logger& logger;

	typedef boost::shared_ptr<fcgi::Request> RequestPtr;

	void runForPtr(const RequestPtr& r);
	void run(fcgi::Request& r);
public:
	typedef boost::function<void(void)> task_t;

	Processor(log::Logger& l):logger(l) {}

	task_t task(const RequestPtr& r);
};

}  // namespace tride

#endif /* PROCESSOR_HPP_ */
