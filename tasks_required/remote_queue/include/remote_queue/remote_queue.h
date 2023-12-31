//
// Created by lukemartinlogan on 6/29/23.
//

#ifndef HERMES_RUN_remote_queue_H_
#define HERMES_RUN_remote_queue_H_

#include "remote_queue_tasks.h"

namespace hrun::remote_queue {

/**
 * Create remote_queue requests
 *
 * This is ONLY used in the Hermes runtime, and
 * should never be called in client programs!!!
 * */
class Client : public TaskLibClient {

 public:
  /** Default constructor */
  Client() = default;

  /** Destructor */
  ~Client() = default;

  /** Async create a task state */
  HSHM_ALWAYS_INLINE
  LPointer<ConstructTask> AsyncCreate(const TaskNode &task_node,
                                      const DomainId &domain_id,
                                      const std::string &state_name,
                                      const TaskStateId &state_id) {
    id_ = state_id;
    QueueManagerInfo &qm = HERMES_RUN_CLIENT->server_config_.queue_manager_;
    std::vector<PriorityInfo> queue_info = {
        {1, 1, qm.queue_depth_, 0},
        {1, 1, qm.queue_depth_, QUEUE_LONG_RUNNING},
        // {qm.max_lanes_, qm.max_lanes_, qm.queue_depth_, QUEUE_LOW_LATENCY}
        {1, 1, qm.queue_depth_, QUEUE_LOW_LATENCY}
    };
    return HERMES_RUN_ADMIN->AsyncCreateTaskState<ConstructTask>(
        task_node, domain_id, state_name, id_, queue_info);
  }
  HERMES_RUN_TASK_NODE_ROOT(AsyncCreate);
  template<typename ...Args>
  HSHM_ALWAYS_INLINE
  void CreateRoot(Args&& ...args) {
    LPointer<ConstructTask> task =
        AsyncCreateRoot(std::forward<Args>(args)...);
    task->Wait();
    id_ = task->id_;
    queue_id_ = QueueId(id_);
    HERMES_RUN_CLIENT->DelTask(task);
  }

  /** Destroy task state + queue */
  HSHM_ALWAYS_INLINE
  void DestroyRoot(const DomainId &domain_id) {
    HERMES_RUN_ADMIN->DestroyTaskStateRoot(domain_id, id_);
  }

  /** Call a custom method */
  HSHM_ALWAYS_INLINE
  void Disperse(Task *orig_task,
                TaskState *exec,
                std::vector<DomainId> &domain_ids) {
    // Serialize task + create the wait task
    HILOG(kDebug, "Beginning dispersion for (task_node={}, task_state={}, method={})",
          orig_task->task_node_ + 1, orig_task->task_state_, orig_task->method_)
    BinaryOutputArchive<true> ar(DomainId::GetNode(HERMES_RUN_CLIENT->node_id_));
    auto xfer = exec->SaveStart(orig_task->method_, ar, orig_task);

    // Create subtasks
    exec->ReplicateStart(orig_task->method_, domain_ids.size(), orig_task);
    LPointer<PushTask> push_task = HERMES_RUN_CLIENT->NewTask<PushTask>(
        orig_task->task_node_ + 1, DomainId::GetLocal(), id_,
        domain_ids, orig_task, exec, orig_task->method_, xfer);
    MultiQueue *queue = HERMES_RUN_CLIENT->GetQueue(queue_id_);
    queue->Emplace(orig_task->prio_, orig_task->lane_hash_, push_task.shm_);
  }

  /** Spawn task to accept new connections */
//  HSHM_ALWAYS_INLINE
//  AcceptTask* AsyncAcceptThread() {
//    hipc::Pointer p;
//    MultiQueue *queue = HERMES_RUN_CLIENT->GetQueue(queue_id_);
//    auto *task = HERMES_RUN_CLIENT->NewTask<AcceptTask>(
//        p, TaskNode::GetNull(), DomainId::GetLocal(), id_);
//    queue->Emplace(0, 0, p);
//    return task;
//  }
};

}  // namespace hrun

#endif  // HERMES_RUN_remote_queue_H_
