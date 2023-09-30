//
// Created by lukemartinlogan on 6/29/23.
//

#ifndef HERMES_RUN_TASKS_HERMES_RUN_ADMIN_HERMES_RUN_ADMIN_H_
#define HERMES_RUN_TASKS_HERMES_RUN_ADMIN_HERMES_RUN_ADMIN_H_

#include "hermes_run_admin_tasks.h"

namespace hrun::Admin {

/** Create admin requests */
class Client : public TaskLibClient {
 public:
  /** Default constructor */
  Client() {
    id_ = TaskStateId(HERMES_RUN_QM_CLIENT->admin_queue_);
    queue_id_ = HERMES_RUN_QM_CLIENT->admin_queue_;
  }

  /** Destructor */
  ~Client() = default;

  /** Register a task library */
  HSHM_ALWAYS_INLINE
  void AsyncRegisterTaskLibConstruct(RegisterTaskLibTask *task,
                                     const TaskNode &task_node,
                                     const DomainId &domain_id,
                                     const std::string &lib_name) {
    HERMES_RUN_CLIENT->ConstructTask<RegisterTaskLibTask>(
        task, task_node, domain_id, lib_name);
  }
  HSHM_ALWAYS_INLINE
  void RegisterTaskLibRoot(const DomainId &domain_id,
                           const std::string &lib_name) {
    LPointer<RegisterTaskLibTask> task = AsyncRegisterTaskLibRoot(domain_id, lib_name);
    task->Wait();
    HERMES_RUN_CLIENT->DelTask(task);
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(RegisterTaskLib)

  /** Unregister a task */
  HSHM_ALWAYS_INLINE
  void AsyncDestroyTaskLibConstruct(DestroyTaskLibTask *task,
                                    const TaskNode &task_node,
                                    const DomainId &domain_id,
                                    const std::string &lib_name) {
    HERMES_RUN_CLIENT->ConstructTask<DestroyTaskLibTask>(
        task, task_node, domain_id, lib_name);
  }
  HSHM_ALWAYS_INLINE
  void DestroyTaskLibRoot(const TaskNode &task_node,
                              const DomainId &domain_id,
                              const std::string &lib_name) {
    LPointer<DestroyTaskLibTask> task =
        AsyncDestroyTaskLibRoot(domain_id, lib_name);
    task->Wait();
    HERMES_RUN_CLIENT->DelTask(task);
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(DestroyTaskLib)

  /** Spawn a task state */
  template<typename CreateTaskT, typename ...Args>
  HSHM_ALWAYS_INLINE
  void AsyncCreateTaskStateConstruct(CreateTaskT *task,
                                     const TaskNode &task_node,
                                     const DomainId &domain_id,
                                     Args&& ...args) {
    HERMES_RUN_CLIENT->ConstructTask<CreateTaskT>(
        task, task_node, domain_id, std::forward<Args>(args)...);
  }
  template<typename CreateTaskT, typename ...Args>
  HSHM_ALWAYS_INLINE
  TaskStateId CreateTaskStateRoot(const DomainId &domain_id,
                                  Args&& ...args) {
    LPointer<CreateTaskT> task = AsyncCreateTaskStateRoot<CreateTaskT>(
        domain_id, std::forward<Args>(args)...);
    task->Wait();
    TaskStateId new_id = task->id_;
    HERMES_RUN_CLIENT->DelTask(task);
    if (new_id.IsNull()) {
      HELOG(kWarning, "Failed to create task state");
    }
    return new_id;
  }
  template<typename CreateTaskT, typename ...Args>
  hipc::LPointer<CreateTaskT> AsyncCreateTaskStateAlloc(const TaskNode &task_node,
                                                        Args&& ...args) {
    hipc::LPointer<CreateTaskT> task = HERMES_RUN_CLIENT->AllocateTask<CreateTaskT>();
    AsyncCreateTaskStateConstruct<CreateTaskT>(task.ptr_, task_node, std::forward<Args>(args)...);
    return task;
  }
  template<typename CreateTaskT, typename ...Args>
  hipc::LPointer<CreateTaskT> AsyncCreateTaskState(const TaskNode &task_node,
                                                   Args&& ...args) {
    hipc::LPointer<CreateTaskT> task =
        AsyncCreateTaskStateAlloc<CreateTaskT>(task_node, std::forward<Args>(args)...);
    MultiQueue *queue = HERMES_RUN_CLIENT->GetQueue(queue_id_);
    queue->Emplace(task.ptr_->prio_, task.ptr_->lane_hash_, task.shm_);
    return task;
  }
  template<typename CreateTaskT, typename ...Args>
  hipc::LPointer<CreateTaskT> AsyncCreateTaskStateEmplace(MultiQueue *queue,
                                                      const TaskNode &task_node,
                                                      Args&& ...args) {
    hipc::LPointer<CreateTaskT> task =
        AsyncCreateTaskStateAllocCreateTaskT(task_node, std::forward<Args>(args)...);
    queue->Emplace(task.ptr_->prio_, task.ptr_->lane_hash_, task.shm_);
    return task;
  }
  template<typename CreateTaskT, typename ...Args>
  hipc::LPointer<CreateTaskT> AsyncCreateTaskStateRoot(Args&& ...args) {
    TaskNode task_node = HERMES_RUN_CLIENT->MakeTaskNodeId();
    hipc::LPointer<CreateTaskT> task =
        AsyncCreateTaskState<CreateTaskT>(task_node, std::forward<Args>(args)...);
    return task;
  }

  /** Get the ID of a task state */
  void AsyncGetOrCreateTaskStateIdConstruct(GetOrCreateTaskStateIdTask *task,
                                            const TaskNode &task_node,
                                            const DomainId &domain_id,
                                            const std::string &state_name) {
    HERMES_RUN_CLIENT->ConstructTask<GetOrCreateTaskStateIdTask>(
        task, task_node, domain_id, state_name);
  }
  TaskStateId GetOrCreateTaskStateIdRoot(const DomainId &domain_id,
                                         const std::string &state_name) {
    LPointer<GetOrCreateTaskStateIdTask> task =
        AsyncGetOrCreateTaskStateIdRoot(domain_id, state_name);
    task->Wait();
    TaskStateId new_id = task->id_;
    HERMES_RUN_CLIENT->DelTask(task);
    return new_id;
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(GetOrCreateTaskStateId)

  /** Get the ID of a task state */
  void AsyncGetTaskStateIdConstruct(GetTaskStateIdTask *task,
                                    const TaskNode &task_node,
                                    const DomainId &domain_id,
                                    const std::string &state_name) {
    HERMES_RUN_CLIENT->ConstructTask<GetTaskStateIdTask>(
        task, task_node, domain_id, state_name);
  }
  TaskStateId GetTaskStateIdRoot(const DomainId &domain_id,
                                 const std::string &state_name) {
    LPointer<GetTaskStateIdTask> task =
        AsyncGetTaskStateIdRoot(domain_id, state_name);
    task->Wait();
    TaskStateId new_id = task->id_;
    HERMES_RUN_CLIENT->DelTask(task);
    return new_id;
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(GetTaskStateId)

  /** Terminate a task state */
  HSHM_ALWAYS_INLINE
  void AsyncDestroyTaskStateConstruct(DestroyTaskStateTask *task,
                                      const TaskNode &task_node,
                                      const DomainId &domain_id,
                                      const TaskStateId &id) {
    HERMES_RUN_CLIENT->ConstructTask<DestroyTaskStateTask>(
        task, task_node, domain_id, id);
  }
  HSHM_ALWAYS_INLINE
  void DestroyTaskStateRoot(const DomainId &domain_id,
                            const TaskStateId &id) {
    LPointer<DestroyTaskStateTask> task =
        AsyncDestroyTaskStateRoot(domain_id, id);
    task->Wait();
    HERMES_RUN_CLIENT->DelTask(task);
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(DestroyTaskState)

  /** Terminate the runtime */
  void AsyncStopRuntimeConstruct(StopRuntimeTask *task,
                                 const TaskNode &task_node,
                                 const DomainId &domain_id) {
    HERMES_RUN_CLIENT->ConstructTask<StopRuntimeTask>(
        task, task_node, domain_id);
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(StopRuntime);

  /** Set work orchestrator queue policy */
  void AsyncSetWorkOrchQueuePolicyConstruct(SetWorkOrchQueuePolicyTask *task,
                                            const TaskNode &task_node,
                                            const DomainId &domain_id,
                                            const TaskStateId &policy) {
    HERMES_RUN_CLIENT->ConstructTask<SetWorkOrchQueuePolicyTask>(
        task, task_node, domain_id, policy);
  }
  void SetWorkOrchQueuePolicyRoot(const DomainId &domain_id,
                                  const TaskStateId &policy) {
    LPointer<SetWorkOrchQueuePolicyTask> task =
        AsyncSetWorkOrchQueuePolicyRoot(domain_id, policy);
    task->Wait();
    HERMES_RUN_CLIENT->DelTask(task);
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(SetWorkOrchQueuePolicy);

  /** Set work orchestrator process policy */
  void AsyncSetWorkOrchProcPolicyConstruct(SetWorkOrchProcPolicyTask *task,
                                           const TaskNode &task_node,
                                           const DomainId &domain_id,
                                           const TaskStateId &policy) {
    HERMES_RUN_CLIENT->ConstructTask<SetWorkOrchProcPolicyTask>(
        task, task_node, domain_id, policy);
  }
  void SetWorkOrchProcPolicyRoot(const DomainId &domain_id,
                                 const TaskStateId &policy) {
    LPointer<SetWorkOrchProcPolicyTask> task =
        AsyncSetWorkOrchProcPolicyRoot(domain_id, policy);
    task->Wait();
    HERMES_RUN_CLIENT->DelTask(task);
  }
  HERMES_RUN_TASK_NODE_ADMIN_ROOT(SetWorkOrchProcPolicy);
};

}  // namespace hrun::Admin

#define HERMES_RUN_ADMIN \
  hshm::EasySingleton<hrun::Admin::Client>::GetInstance()

#endif  // HERMES_RUN_TASKS_HERMES_RUN_ADMIN_HERMES_RUN_ADMIN_H_
