#include <iostream>

#include <csignal>

#include <boost/thread.hpp>
#include <boost/make_shared.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <tride/fcgi/request.hpp>
#include <tride/log/message.hpp>
#include <tride/log/logger.hpp>

#include "server.hpp"

namespace tride {

namespace {

class CoutLogger: public log::Logger {
	const std::string& levelStr(const log::LogLevel level) {
		static std::string noteTxt("NTE");
		static std::string errorTxt("ERR");
		return level == log::LogLevel::ERROR? errorTxt : noteTxt;
	}
public:
	void write(const log::LogLevel level, const std::string& msg) override {
		std::cout << '[' << boost::posix_time::second_clock::local_time() << " " << levelStr(level) << "] " << msg << std::endl;
	}
};

void error_log(const char* msg) {
	using namespace std;
	using namespace boost;
//	static ofstream error;
//	if (!error.is_open()) {
//		error.open("/tmp/errlog", ios_base::out | ios_base::app);
//		error.imbue(locale(error.getloc(), new posix_time::time_facet()));
//	}
	std::cout << '[' << posix_time::second_clock::local_time() << "] " << msg << std::endl;
}

void signalHandler( int signum ) {
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
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
