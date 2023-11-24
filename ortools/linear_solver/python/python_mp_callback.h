//
// Created by mitripet on 24/11/23.
//

#include "ortools/linear_solver/linear_solver.h"

#ifndef ORTOOLS_PYTHON_MP_CALLBACK_H
#define ORTOOLS_PYTHON_MP_CALLBACK_H

namespace operations_research {
// TODO : add description
class PythonMPCallback : public MPCallback {
 public:
  PythonMPCallback(bool might_add_cuts, bool might_add_lazy_constraints)
      : MPCallback(might_add_cuts, might_add_lazy_constraints){};
  virtual ~PythonMPCallback() {}

  virtual void Run(MPCallbackContext* callback_context) = 0;

  void RunCallback(MPCallbackContext* callback_context) override {
    // acquire Python GIL before running user-defined callback in python
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    Run(callback_context);
    // release Python GIL
    PyGILState_Release(gstate);
  };
};
}
#endif  // ORTOOLS_PYTHON_MP_CALLBACK_H
