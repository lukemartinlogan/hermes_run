//
// Created by lukemartinlogan on 6/29/23.
//

#include "hermes_run_admin/hermes_run_admin.h"
#include "hermes_run/api/hermes_run_runtime.h"
#include "worch_queue_round_robin/worch_queue_round_robin.h"

namespace hrun::worch_queue_round_robin {

class Server : public TaskLib {
 public:
  u32 count_;

 public:
  void Construct(ConstructTask *task, RunContext &ctx) {
    count_ = 0;
    task->SetModuleComplete();
  }

  void Destruct(DestructTask *task, RunContext &ctx) {
    task->SetModuleComplete();
  }

  void Schedule(ScheduleTask *task, RunContext &ctx) {
    // Check if any new queues need to be scheduled
    for (MultiQueue &queue : *HERMES_RUN_QM_RUNTIME->queue_map_) {
      if (queue.id_.IsNull()) {
        continue;
      }
      for (LaneGroup &lane_group : *queue.groups_) {
        // NOTE(llogan): Assumes a minimum of two workers, admin on worker 0.
        if (lane_group.IsLowPriority()) {
          for (u32 lane_id = lane_group.num_scheduled_; lane_id < lane_group.num_lanes_; ++lane_id) {
            HILOG(kDebug, "Scheduling the queue {} (lane {})", queue.id_, lane_id);
            Worker &worker = HERMES_RUN_WORK_ORCHESTRATOR->workers_[0];
            worker.PollQueues({WorkEntry(lane_group.prio_, lane_id, &queue)});
          }
          lane_group.num_scheduled_ = lane_group.num_lanes_;
        } else {
          for (u32 lane_id = lane_group.num_scheduled_; lane_id < lane_group.num_lanes_; ++lane_id) {
            HILOG(kDebug, "Scheduling the queue {} (lane {})", queue.id_, lane_id);
            u32 worker_id = (count_ % (HERMES_RUN_WORK_ORCHESTRATOR->workers_.size() - 1)) + 1;
            Worker &worker = HERMES_RUN_WORK_ORCHESTRATOR->workers_[worker_id];
            worker.PollQueues({WorkEntry(lane_group.prio_, lane_id, &queue)});
            count_ += 1;
          }
          lane_group.num_scheduled_ = lane_group.num_lanes_;
        }
      }
    }
  }

#include "worch_queue_round_robin/worch_queue_round_robin_lib_exec.h"
};

}  // namespace hrun

HERMES_RUN_TASK_CC(hrun::worch_queue_round_robin::Server, "worch_queue_round_robin");
