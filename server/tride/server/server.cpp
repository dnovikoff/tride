#include <csignal>

#include <boost/thread.hpp>
#include <boost/make_shared.hpp>

#include <tride/fcgi/request.hpp>
#include <tride/log/logger.hpp>

#include "processor.hpp"
#include "server.hpp"

namespace tride {

namespace {

static boost::mutex currentLoggerMutex;
log::Logger* currentLogger = NULL;

void signalHandler( int signum ) {
	boost::mutex::scoped_lock l(currentLoggerMutex);
	if(currentLogger) {
		currentLogger->trace() << "Interrupt signal (" << signum << ") received.";
	}
}

void initSignals() {
	static bool signalsInited = false;
	if(!signalsInited) {
		fcgi::Request::installSignal(SIGINT, signalHandler);
		fcgi::Request::installSignal(SIGTERM, signalHandler);
	}
	signalsInited = true;
}

} // namespace

void Server::postTask(const task_t& callback) {
	io.post( callback );
}

void Server::run(const size_t numberOfThreads) {
	{
		boost::mutex::scoped_lock l(currentLoggerMutex);
		currentLogger = &logger;
	}

	logger.trace() << "Initializing signals";
	initSignals();
	boost::thread_group tg;

	{
		boost::asio::io_service::work work( io );

		logger << "Creating " << numberOfThreads << " threads";
		for(size_t i = 0; i < numberOfThreads; ++i) {
			tg.create_thread(boost::bind(&boost::asio::io_service::run, &io));
		}
		logger << "Server started";
		while(1) {
			auto r = boost::make_shared<fcgi::Request>();
			logger.trace() << "Accepting new request";
			if( !r->accept() ) {
				logger << "Server shutting down";
				break;
			}
			postTask( processor.task(r) );
		}
		// No new reqests are accepted at this point
		// The io_service::work objects destructs here. The io.run(s) will exit, when all run out of tasks
	}
	logger.trace() << "Waiting for all threads to exit";
	// Waiting untill will run out of task and all thread will exit
	tg.join_all();
	logger << "Server stopped";
	{
		boost::mutex::scoped_lock l(currentLoggerMutex);
		currentLogger = NULL;
	}
}

}  // namespace tride
