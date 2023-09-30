//
// Created by lukemartinlogan on 8/14/23.
//

#ifndef HERMES_RUN_WORCH_PROC_ROUND_ROBIN_TASKS_H__
#define HERMES_RUN_WORCH_PROC_ROUND_ROBIN_TASKS_H__

#include "hermes_run/api/hermes_run_client.h"
#include "hermes_run/task_registry/task_lib.h"
#include "hermes_run/work_orchestrator/scheduler.h"
#include "hermes_run_admin/hermes_run_admin.h"
#include "hermes_run/queue_manager/queue_manager_client.h"
#include "proc_queue/proc_queue.h"

namespace hrun::worch_proc_round_robin {

#include "hermes_run/hermes_run_namespace.h"

/** The set of methods in the worch task */
typedef SchedulerMethod Method;

/**
 * A task to create worch_proc_round_robin
 * */
using hrun::Admin::CreateTaskStateTask;
struct ConstructTask : public CreateTaskStateTask {
  /** SHM default constructor */
  HSHM_ALWAYS_INLINE explicit
  ConstructTask(hipc::Allocator *alloc) : CreateTaskStateTask(alloc) {}

  /** Emplace constructor */
  HSHM_ALWAYS_INLINE
  ConstructTask(hipc::Allocator *alloc,
                const TaskNode &task_node,
                const DomainId &domain_id,
                const std::string &state_name,
                const TaskStateId &id,
                const std::vector<PriorityInfo> &queue_info)
      : CreateTaskStateTask(alloc, task_node, domain_id, state_name,
                            "worch_proc_round_robin", id, queue_info) {
  }
};

/** A task to destroy worch_proc_round_robin */
using hrun::Admin::DestroyTaskStateTask;
struct DestructTask : public DestroyTaskStateTask {
  /** SHM default constructor */
  HSHM_ALWAYS_INLINE explicit
  DestructTask(hipc::Allocator *alloc) : DestroyTaskStateTask(alloc) {}

  /** Emplace constructor */
  HSHM_ALWAYS_INLINE
  DestructTask(hipc::Allocator *alloc,
               const TaskNode &task_node,
               const DomainId &domain_id,
               TaskStateId &state_id)
      : DestroyTaskStateTask(alloc, task_node, domain_id, state_id) {}

  /** Create group */
  HSHM_ALWAYS_INLINE
  u32 GetGroup(hshm::charbuf &group) {
    return TASK_UNORDERED;
  }
};

}  // namespace hrun::worch_proc_round_robin

#endif  // HERMES_RUN_WORCH_PROC_ROUND_ROBIN_TASKS_H__
