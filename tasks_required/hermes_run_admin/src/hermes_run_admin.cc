//
// Created by lukemartinlogan on 6/29/23.
//

#include "hermes_run_admin/hermes_run_admin.h"
#include "hermes_run/api/hermes_run_runtime.h"
#include "hermes_run/work_orchestrator/scheduler.h"

namespace hrun::Admin {

class Server : public TaskLib {
 public:
  Task *queue_sched_;
  Task *proc_sched_;

 public:
  Server() : queue_sched_(nullptr), proc_sched_(nullptr) {}

  void RegisterTaskLib(RegisterTaskLibTask *task, RunContext &ctx) {
    std::string lib_name = task->lib_name_->str();
    HERMES_RUN_TASK_REGISTRY->RegisterTaskLib(lib_name);
    task->SetModuleComplete();
  }

  void DestroyTaskLib(DestroyTaskLibTask *task, RunContext &ctx) {
    std::string lib_name = task->lib_name_->str();
    HERMES_RUN_TASK_REGISTRY->DestroyTaskLib(lib_name);
    task->SetModuleComplete();
  }

  void GetOrCreateTaskStateId(GetOrCreateTaskStateIdTask *task, RunContext &ctx) {
    std::string state_name = task->state_name_->str();
    task->id_ = HERMES_RUN_TASK_REGISTRY->GetOrCreateTaskStateId(state_name);
    task->SetModuleComplete();
  }

  void CreateTaskState(CreateTaskStateTask *task, RunContext &ctx) {
    std::string lib_name = task->lib_name_->str();
    std::string state_name = task->state_name_->str();
    // Check local registry for task state
    TaskState *task_state = HERMES_RUN_TASK_REGISTRY->GetTaskState(state_name, task->id_);
    if (task_state) {
      task->id_ = task_state->id_;
      task->SetModuleComplete();
      return;
    }
    // Check global registry for task state
    if (task->id_.IsNull()) {
      DomainId domain = DomainId::GetNode(1);
      LPointer<GetOrCreateTaskStateIdTask> get_id =
          HERMES_RUN_ADMIN->AsyncGetOrCreateTaskStateId(task->task_node_ + 1, domain, state_name);
      get_id->Wait<TASK_YIELD_CO>(task);
      task->id_ = get_id->id_;
      HERMES_RUN_CLIENT->DelTask(get_id);
    }
    // Create the task state
    HILOG(kInfo, "(node {}) Creating task state {} with id {} (task_node={})",
          HERMES_RUN_CLIENT->node_id_, state_name, task->id_, task->task_node_);
    if (task->id_.IsNull()) {
      HELOG(kError, "(node {}) The task state {} with id {} is NULL.",
            HERMES_RUN_CLIENT->node_id_, state_name, task->id_);
      task->SetModuleComplete();
      return;
    }
    // Verify the state doesn't exist
    if (HERMES_RUN_TASK_REGISTRY->TaskStateExists(task->id_)) {
      HILOG(kInfo, "(node {}) The task state {} with id {} exists",
            HERMES_RUN_CLIENT->node_id_, state_name, task->id_);
      task->SetModuleComplete();
      return;
    }
    // Create the task queue for the state
    QueueId qid(task->id_);
    HERMES_RUN_QM_RUNTIME->CreateQueue(
        qid, task->queue_info_->vec());
    // Run the task state's submethod
    task->method_ = Method::kConstruct;
    bool ret = HERMES_RUN_TASK_REGISTRY->CreateTaskState(
        lib_name.c_str(),
        state_name.c_str(),
        task->id_,
        task);
    if (!ret) {
      task->SetModuleComplete();
      return;
    }
    HILOG(kInfo, "(node {}) Allocated task state {} with id {}",
          HERMES_RUN_CLIENT->node_id_, state_name, task->task_state_);
  }

  void GetTaskStateId(GetTaskStateIdTask *task, RunContext &ctx) {
    std::string state_name = task->state_name_->str();
    task->id_ = HERMES_RUN_TASK_REGISTRY->GetTaskStateId(state_name);
    task->SetModuleComplete();
  }

  void DestroyTaskState(DestroyTaskStateTask *task, RunContext &ctx) {
    HERMES_RUN_TASK_REGISTRY->DestroyTaskState(task->id_);
    task->SetModuleComplete();
  }

  void StopRuntime(StopRuntimeTask *task, RunContext &ctx) {
    HILOG(kInfo, "Stopping (server mode)");
    HERMES_RUN_WORK_ORCHESTRATOR->FinalizeRuntime();
    HERMES_RUN_THALLIUM->StopThisDaemon();
    task->SetModuleComplete();
  }

  void SetWorkOrchQueuePolicy(SetWorkOrchQueuePolicyTask *task, RunContext &ctx) {
    if (queue_sched_) {
      queue_sched_->SetModuleComplete();
    }
    if (queue_sched_ && !queue_sched_->IsComplete()) {
      return;
    }
    auto queue_sched = HERMES_RUN_CLIENT->NewTask<ScheduleTask>(
        task->task_node_, DomainId::GetLocal(), task->policy_id_);
    queue_sched_ = queue_sched.ptr_;
    MultiQueue *queue = HERMES_RUN_CLIENT->GetQueue(queue_id_);
    queue->Emplace(0, 0, queue_sched.shm_);
    task->SetModuleComplete();
  }

  void SetWorkOrchProcPolicy(SetWorkOrchProcPolicyTask *task, RunContext &ctx) {
    if (proc_sched_) {
      proc_sched_->SetModuleComplete();
    }
    if (proc_sched_ && !proc_sched_->IsComplete()) {
      return;
    }
    auto proc_sched = HERMES_RUN_CLIENT->NewTask<ScheduleTask>(
        task->task_node_, DomainId::GetLocal(), task->policy_id_);
    proc_sched_ = proc_sched.ptr_;
    MultiQueue *queue = HERMES_RUN_CLIENT->GetQueue(queue_id_);
    queue->Emplace(0, 0, proc_sched.shm_);
    task->SetModuleComplete();
  }

 public:
#include "hermes_run_admin/hermes_run_admin_lib_exec.h"
};

}  // namespace hrun

HERMES_RUN_TASK_CC(hrun::Admin::Server, "hermes_run_admin");
