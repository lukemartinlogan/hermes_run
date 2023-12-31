//
// Created by lukemartinlogan on 6/29/23.
//

#include "hermes_run_admin/hermes_run_admin.h"
#include "hermes_run/api/hermes_run_runtime.h"
#include "proc_queue/proc_queue.h"

namespace hrun::proc_queue {

class Server : public TaskLib {
 public:
  Server() = default;

  void Construct(ConstructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Destruct(DestructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Push(PushTask *task, RunContext &ctx) {
    switch (task->phase_) {
      case PushTaskPhase::kSchedule: {
        task->sub_run_.shm_ = task->sub_cli_.shm_;
        task->sub_run_.ptr_ = HERMES_RUN_CLIENT->GetPrivatePointer<Task>(task->sub_cli_.shm_);
        Task *&ptr = task->sub_run_.ptr_;
        HILOG(kDebug, "Scheduling task {} on state {} tid {}",
              ptr->task_node_, ptr->task_state_, GetLinuxTid());
        if (ptr->IsFireAndForget()) {
          ptr->UnsetFireAndForget();
        }
        MultiQueue *real_queue = HERMES_RUN_CLIENT->GetQueue(QueueId(ptr->task_state_));
        real_queue->Emplace(ptr->prio_, ptr->lane_hash_, task->sub_run_.shm_);
        task->phase_ = PushTaskPhase::kWaitSchedule;
      }
      case PushTaskPhase::kWaitSchedule: {
        Task *&ptr = task->sub_run_.ptr_;
        if (!ptr->IsComplete()) {
          return;
        }
        // TODO(llogan): handle fire & forget tasks gracefully
        task->SetModuleComplete();
      }
    }
  }

 public:
#include "proc_queue/proc_queue_lib_exec.h"
};

}  // namespace hrun::proc_queue

HERMES_RUN_TASK_CC(hrun::proc_queue::Server, "proc_queue");
