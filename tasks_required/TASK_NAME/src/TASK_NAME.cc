//
// Created by lukemartinlogan on 6/29/23.
//

#include "hermes_run_admin/hermes_run_admin.h"
#include "hermes_run/api/hermes_run_runtime.h"
#include "TASK_NAME/TASK_NAME.h"

namespace hrun::TASK_NAME {

class Server : public TaskLib {
 public:
  Server() = default;

  void Construct(ConstructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Destruct(DestructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Custom(CustomTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

 public:
#include "TASK_NAME/TASK_NAME_lib_exec.h"
};

}  // namespace hrun::TASK_NAME

HERMES_RUN_TASK_CC(hrun::TASK_NAME::Server, "TASK_NAME");
