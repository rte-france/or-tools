#include "coin_log_utils.h"

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