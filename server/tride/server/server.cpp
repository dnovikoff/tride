#include <csignal>

#include <boost/thread.hpp>
#include <boost/make_shared.hpp>

#include <tride/fcgi/request.hpp>
#include <tride/server/cout_logger.hpp>

#include "server.hpp"

namespace tride {

namespace {

CoutLogger* currentLogger = NULL;

void signalHandler( int signum ) {
	(*currentLogger) << "Interrupt signal (" << signum << ") received.";
}

void initSignals() {
	static bool signalsInited = false;
	if(!signalsInited) {
		fcgi::Request::installSignal(SIGINT, signalHandler);
	}
	signalsInited = true;
}

} // namespace

void Server::postTask(const task_t& callback) {
	io.post( callback );
}

void Server::run(const size_t numberOfThreads, const callback_t& callback) {
	CoutLogger logger;
	currentLogger = &logger;

	logger << "Server started";
	initSignals();
	logger << "Signals initialized";
	boost::thread_group tg;

	{
		boost::asio::io_service::work work( io );

		logger << "Creating " << numberOfThreads << " threads";
		for(size_t i = 0; i < numberOfThreads; ++i) {
			tg.create_thread(boost::bind(&boost::asio::io_service::run, &io));
		}

		while(1) {
			auto r = boost::make_shared<fcgi::Request>();
			logger << "Accepting new request";
			if( !r->accept() ) {
				logger << "Server stopped";
				break;
			}
			postTask( boost::bind(callback, r) );
		}
		// No new reqests are accepted at this point
		// The io_service::work objects destructs here. The io.run(s) will exit, when all run out of tasks
	}
	logger << "Waiting for all threads to exit";
	// Waiting untill will run out of task and all thread will exit
	tg.join_all();
	logger << "Server stopped";
}

}  // namespace tride
