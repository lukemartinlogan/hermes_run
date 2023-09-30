//
// Created by lukemartinlogan on 6/29/23.
//

#include "hermes_run_admin/hermes_run_admin.h"
#include "hermes_run/api/hermes_run_runtime.h"
#include "worch_proc_round_robin/worch_proc_round_robin.h"

namespace hrun::worch_proc_round_robin {

class Server : public TaskLib {
 public:
  void Construct(ConstructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Destruct(DestructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Schedule(ScheduleTask *task, RunContext &ctx) {
    int rr = 0;
    for (Worker &worker : HERMES_RUN_WORK_ORCHESTRATOR->workers_) {
      worker.SetCpuAffinity(rr % HERMES_SYSTEM_INFO->ncpu_);
      ++rr;
    }
  }

#include "worch_proc_round_robin/worch_proc_round_robin_lib_exec.h"
};

}  // namespace hrun

HERMES_RUN_TASK_CC(hrun::worch_proc_round_robin::Server, "worch_proc_round_robin");
