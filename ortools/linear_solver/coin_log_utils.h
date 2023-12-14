
#ifndef OR_TOOLS_LINEAR_SOLVER_LINEAR_COIN_LOG_UTILS_H_
#define OR_TOOLS_LINEAR_SOLVER_LINEAR_COIN_LOG_UTILS_H_

// #if defined(USE_CLP) || defined(USE_CBC)

// #undef PACKAGE
// #undef VERSION
#include "CoinMessageHandler.hpp"
#include "linear_solver.h"

class CoinMessageHandlerCallBack : public CoinMessageHandler {
 public:
  explicit CoinMessageHandlerCallBack(LogHandlerInterface* log_interface);
  int print() override;

 private:
  LogHandlerInterface* log_interface_ = nullptr;
};

// #endif  // #if defined(USE_CBC) || defined(USE_CLP)

#endif  // OR_TOOLS_LINEAR_SOLVER_LINEAR_COIN_LOG_UTILS_H_
