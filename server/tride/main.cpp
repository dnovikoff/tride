#include <boost/shared_ptr.hpp>

#include <tride/server/server.hpp>
#include <tride/server/processor.hpp>
#include <tride/server/cout_logger.hpp>
#include <tride/fcgi/request.hpp>

int main() {
	using namespace tride;
	CoutLogger logger;
	Processor processor( logger );
	Server server( processor, logger );
	server.run( 10u );
	return 0;
}
