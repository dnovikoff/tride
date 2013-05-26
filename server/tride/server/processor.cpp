#include <boost/bind.hpp>

#include <tride/fcgi/request.hpp>
#include <tride/log/logger.hpp>

#include "processor.hpp"

namespace tride {

void Processor::run(fcgi::Request& request) {
	logger.trace() << "Processing new request";
	request << "Content-type: text/html\r\n\r\nHello v2";
}

void Processor::runForPtr(const RequestPtr& request) {
	run(*request);
}

Processor::task_t Processor::task(const RequestPtr& r) {
	return boost::bind(&self_t::runForPtr, this, r);
}

}  // namespace tride
