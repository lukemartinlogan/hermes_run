//
// Created by lukemartinlogan on 6/29/23.
//

#include "hermes_run_admin/hermes_run_admin.h"
#include "hermes_run/api/hermes_run_runtime.h"
#include "small_message/small_message.h"

namespace hrun::small_message {

class Server : public TaskLib {
 public:
  int count_ = 0;

 public:
  void Construct(ConstructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Destruct(DestructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Md(MdTask *task, RunContext &ctx) {
    task->ret_[0] = 1;
    task->SetModuleComplete();
  }

  void MdPush(MdPushTask *task, RunContext &ctx) {
    task->ret_[0] = 1;
    task->SetModuleComplete();
  }

  void Io(IoTask *task, RunContext &ctx) {
    task->ret_ = 1;
    for (int i = 0; i < 256; ++i) {
      if (task->data_[i] != 10) {
        task->ret_ = 0;
        break;
      }
    }
    task->SetModuleComplete();
  }

 public:
#include "small_message/small_message_lib_exec.h"
};

}  // namespace hrun

HERMES_RUN_TASK_CC(hrun::small_message::Server, "small_message");
