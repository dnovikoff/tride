#include "fcgi_server.hpp"

namespace tride {

FcgiServer* FcgiServer::current = NULL;

FcgiServer::FcgiServer() {
	work.reset(new boost::asio::io_service::work(service));
	assert( current == NULL );
	current = this;
}

FcgiServer::~FcgiServer() {}

void FcgiServer::run() {
	fcgi.handler();
	work.reset();
}

void FcgiServer::stop() {
	fcgi.stop();
}

boost::asio::io_service& FcgiServer::io() {
	return service;
}

boost::asio::io_service& FcgiServer::currentIo() {
	return current->io();
}

void FcgiServer::currentStop() {
	current->stop();
}

} // namespace tride
