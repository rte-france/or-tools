

#if defined(USE_CLP) || defined(USE_CBC)

#undef PACKAGE
#undef VERSION
#include "coin_log_utils.h"
namespace operations_research {

CoinMessageHandlerCallBack::CoinMessageHandlerCallBack(
    LogHandlerInterface* log_interface)
    : log_interface_(log_interface) {}

int CoinMessageHandlerCallBack::print() {
  if (log_interface_) {
    log_interface_->message(messageBuffer());
    return 0;
  } else {
    return CoinMessageHandler::print();
  }
}
}  // namespace operations_research
#endif  // #if defined(USE_CBC) || defined(USE_CLP)
