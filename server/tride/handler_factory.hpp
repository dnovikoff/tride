#ifndef HANDLER_FACTOTY_HPP_
#define HANDLER_FACTOTY_HPP_

#include <boost/function.hpp>

namespace tride {
class RequestManager;
class Handler;

typedef Handler* HandlerPtr;
typedef boost::function<HandlerPtr()> HandlerFactory;

template<typename HandlerT>
HandlerPtr createHandler() {
	return new HandlerT;
}

template<typename HandlerT>
HandlerFactory getFactory() {
	return boost::bind( &createHandler<HandlerT> );
}

} // namespace tride

#endif /* HANDLER_FACTOTY_HPP_ */
