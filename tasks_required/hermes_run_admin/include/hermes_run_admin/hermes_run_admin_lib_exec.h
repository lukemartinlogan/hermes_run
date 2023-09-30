#ifndef HERMES_RUN_HERMES_RUN_ADMIN_LIB_EXEC_H_
#define HERMES_RUN_HERMES_RUN_ADMIN_LIB_EXEC_H_

/** Execute a task */
void Run(u32 method, Task *task, RunContext &ctx) override {
  switch (method) {
    case Method::kCreateTaskState: {
      CreateTaskState(reinterpret_cast<CreateTaskStateTask *>(task), ctx);
      break;
    }
    case Method::kDestroyTaskState: {
      DestroyTaskState(reinterpret_cast<DestroyTaskStateTask *>(task), ctx);
      break;
    }
    case Method::kRegisterTaskLib: {
      RegisterTaskLib(reinterpret_cast<RegisterTaskLibTask *>(task), ctx);
      break;
    }
    case Method::kDestroyTaskLib: {
      DestroyTaskLib(reinterpret_cast<DestroyTaskLibTask *>(task), ctx);
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      GetOrCreateTaskStateId(reinterpret_cast<GetOrCreateTaskStateIdTask *>(task), ctx);
      break;
    }
    case Method::kGetTaskStateId: {
      GetTaskStateId(reinterpret_cast<GetTaskStateIdTask *>(task), ctx);
      break;
    }
    case Method::kStopRuntime: {
      StopRuntime(reinterpret_cast<StopRuntimeTask *>(task), ctx);
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      SetWorkOrchQueuePolicy(reinterpret_cast<SetWorkOrchQueuePolicyTask *>(task), ctx);
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      SetWorkOrchProcPolicy(reinterpret_cast<SetWorkOrchProcPolicyTask *>(task), ctx);
      break;
    }
  }
}
/** Delete a task */
void Del(u32 method, Task *task) override {
  switch (method) {
    case Method::kCreateTaskState: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<CreateTaskStateTask *>(task));
      break;
    }
    case Method::kDestroyTaskState: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<DestroyTaskStateTask *>(task));
      break;
    }
    case Method::kRegisterTaskLib: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<RegisterTaskLibTask *>(task));
      break;
    }
    case Method::kDestroyTaskLib: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<DestroyTaskLibTask *>(task));
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<GetOrCreateTaskStateIdTask *>(task));
      break;
    }
    case Method::kGetTaskStateId: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<GetTaskStateIdTask *>(task));
      break;
    }
    case Method::kStopRuntime: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<StopRuntimeTask *>(task));
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<SetWorkOrchQueuePolicyTask *>(task));
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      HERMES_RUN_CLIENT->DelTask(reinterpret_cast<SetWorkOrchProcPolicyTask *>(task));
      break;
    }
  }
}
/** Ensure there is space to store replicated outputs */
void ReplicateStart(u32 method, u32 count, Task *task) override {
  switch (method) {
    case Method::kCreateTaskState: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<CreateTaskStateTask*>(task));
      break;
    }
    case Method::kDestroyTaskState: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<DestroyTaskStateTask*>(task));
      break;
    }
    case Method::kRegisterTaskLib: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<RegisterTaskLibTask*>(task));
      break;
    }
    case Method::kDestroyTaskLib: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<DestroyTaskLibTask*>(task));
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<GetOrCreateTaskStateIdTask*>(task));
      break;
    }
    case Method::kGetTaskStateId: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<GetTaskStateIdTask*>(task));
      break;
    }
    case Method::kStopRuntime: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<StopRuntimeTask*>(task));
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<SetWorkOrchQueuePolicyTask*>(task));
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      hrun::CALL_REPLICA_START(count, reinterpret_cast<SetWorkOrchProcPolicyTask*>(task));
      break;
    }
  }
}
/** Determine success and handle failures */
void ReplicateEnd(u32 method, Task *task) override {
  switch (method) {
    case Method::kCreateTaskState: {
      hrun::CALL_REPLICA_END(reinterpret_cast<CreateTaskStateTask*>(task));
      break;
    }
    case Method::kDestroyTaskState: {
      hrun::CALL_REPLICA_END(reinterpret_cast<DestroyTaskStateTask*>(task));
      break;
    }
    case Method::kRegisterTaskLib: {
      hrun::CALL_REPLICA_END(reinterpret_cast<RegisterTaskLibTask*>(task));
      break;
    }
    case Method::kDestroyTaskLib: {
      hrun::CALL_REPLICA_END(reinterpret_cast<DestroyTaskLibTask*>(task));
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      hrun::CALL_REPLICA_END(reinterpret_cast<GetOrCreateTaskStateIdTask*>(task));
      break;
    }
    case Method::kGetTaskStateId: {
      hrun::CALL_REPLICA_END(reinterpret_cast<GetTaskStateIdTask*>(task));
      break;
    }
    case Method::kStopRuntime: {
      hrun::CALL_REPLICA_END(reinterpret_cast<StopRuntimeTask*>(task));
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      hrun::CALL_REPLICA_END(reinterpret_cast<SetWorkOrchQueuePolicyTask*>(task));
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      hrun::CALL_REPLICA_END(reinterpret_cast<SetWorkOrchProcPolicyTask*>(task));
      break;
    }
  }
}
/** Serialize a task when initially pushing into remote */
std::vector<DataTransfer> SaveStart(u32 method, BinaryOutputArchive<true> &ar, Task *task) override {
  switch (method) {
    case Method::kCreateTaskState: {
      ar << *reinterpret_cast<CreateTaskStateTask*>(task);
      break;
    }
    case Method::kDestroyTaskState: {
      ar << *reinterpret_cast<DestroyTaskStateTask*>(task);
      break;
    }
    case Method::kRegisterTaskLib: {
      ar << *reinterpret_cast<RegisterTaskLibTask*>(task);
      break;
    }
    case Method::kDestroyTaskLib: {
      ar << *reinterpret_cast<DestroyTaskLibTask*>(task);
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      ar << *reinterpret_cast<GetOrCreateTaskStateIdTask*>(task);
      break;
    }
    case Method::kGetTaskStateId: {
      ar << *reinterpret_cast<GetTaskStateIdTask*>(task);
      break;
    }
    case Method::kStopRuntime: {
      ar << *reinterpret_cast<StopRuntimeTask*>(task);
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      ar << *reinterpret_cast<SetWorkOrchQueuePolicyTask*>(task);
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      ar << *reinterpret_cast<SetWorkOrchProcPolicyTask*>(task);
      break;
    }
  }
  return ar.Get();
}
/** Deserialize a task when popping from remote queue */
TaskPointer LoadStart(u32 method, BinaryInputArchive<true> &ar) override {
  TaskPointer task_ptr;
  switch (method) {
    case Method::kCreateTaskState: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<CreateTaskStateTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<CreateTaskStateTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kDestroyTaskState: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<DestroyTaskStateTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<DestroyTaskStateTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kRegisterTaskLib: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<RegisterTaskLibTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<RegisterTaskLibTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kDestroyTaskLib: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<DestroyTaskLibTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<DestroyTaskLibTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<GetOrCreateTaskStateIdTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<GetOrCreateTaskStateIdTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kGetTaskStateId: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<GetTaskStateIdTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<GetTaskStateIdTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kStopRuntime: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<StopRuntimeTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<StopRuntimeTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<SetWorkOrchQueuePolicyTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<SetWorkOrchQueuePolicyTask*>(task_ptr.ptr_);
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      task_ptr.ptr_ = HERMES_RUN_CLIENT->NewEmptyTask<SetWorkOrchProcPolicyTask>(task_ptr.shm_);
      ar >> *reinterpret_cast<SetWorkOrchProcPolicyTask*>(task_ptr.ptr_);
      break;
    }
  }
  return task_ptr;
}
/** Serialize a task when returning from remote queue */
std::vector<DataTransfer> SaveEnd(u32 method, BinaryOutputArchive<false> &ar, Task *task) override {
  switch (method) {
    case Method::kCreateTaskState: {
      ar << *reinterpret_cast<CreateTaskStateTask*>(task);
      break;
    }
    case Method::kDestroyTaskState: {
      ar << *reinterpret_cast<DestroyTaskStateTask*>(task);
      break;
    }
    case Method::kRegisterTaskLib: {
      ar << *reinterpret_cast<RegisterTaskLibTask*>(task);
      break;
    }
    case Method::kDestroyTaskLib: {
      ar << *reinterpret_cast<DestroyTaskLibTask*>(task);
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      ar << *reinterpret_cast<GetOrCreateTaskStateIdTask*>(task);
      break;
    }
    case Method::kGetTaskStateId: {
      ar << *reinterpret_cast<GetTaskStateIdTask*>(task);
      break;
    }
    case Method::kStopRuntime: {
      ar << *reinterpret_cast<StopRuntimeTask*>(task);
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      ar << *reinterpret_cast<SetWorkOrchQueuePolicyTask*>(task);
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      ar << *reinterpret_cast<SetWorkOrchProcPolicyTask*>(task);
      break;
    }
  }
  return ar.Get();
}
/** Deserialize a task when returning from remote queue */
void LoadEnd(u32 replica, u32 method, BinaryInputArchive<false> &ar, Task *task) override {
  switch (method) {
    case Method::kCreateTaskState: {
      ar.Deserialize(replica, *reinterpret_cast<CreateTaskStateTask*>(task));
      break;
    }
    case Method::kDestroyTaskState: {
      ar.Deserialize(replica, *reinterpret_cast<DestroyTaskStateTask*>(task));
      break;
    }
    case Method::kRegisterTaskLib: {
      ar.Deserialize(replica, *reinterpret_cast<RegisterTaskLibTask*>(task));
      break;
    }
    case Method::kDestroyTaskLib: {
      ar.Deserialize(replica, *reinterpret_cast<DestroyTaskLibTask*>(task));
      break;
    }
    case Method::kGetOrCreateTaskStateId: {
      ar.Deserialize(replica, *reinterpret_cast<GetOrCreateTaskStateIdTask*>(task));
      break;
    }
    case Method::kGetTaskStateId: {
      ar.Deserialize(replica, *reinterpret_cast<GetTaskStateIdTask*>(task));
      break;
    }
    case Method::kStopRuntime: {
      ar.Deserialize(replica, *reinterpret_cast<StopRuntimeTask*>(task));
      break;
    }
    case Method::kSetWorkOrchQueuePolicy: {
      ar.Deserialize(replica, *reinterpret_cast<SetWorkOrchQueuePolicyTask*>(task));
      break;
    }
    case Method::kSetWorkOrchProcPolicy: {
      ar.Deserialize(replica, *reinterpret_cast<SetWorkOrchProcPolicyTask*>(task));
      break;
    }
  }
}
/** Get the grouping of the task */
u32 GetGroup(u32 method, Task *task, hshm::charbuf &group) override {
  switch (method) {
    case Method::kCreateTaskState: {
      return reinterpret_cast<CreateTaskStateTask*>(task)->GetGroup(group);
    }
    case Method::kDestroyTaskState: {
      return reinterpret_cast<DestroyTaskStateTask*>(task)->GetGroup(group);
    }
    case Method::kRegisterTaskLib: {
      return reinterpret_cast<RegisterTaskLibTask*>(task)->GetGroup(group);
    }
    case Method::kDestroyTaskLib: {
      return reinterpret_cast<DestroyTaskLibTask*>(task)->GetGroup(group);
    }
    case Method::kGetOrCreateTaskStateId: {
      return reinterpret_cast<GetOrCreateTaskStateIdTask*>(task)->GetGroup(group);
    }
    case Method::kGetTaskStateId: {
      return reinterpret_cast<GetTaskStateIdTask*>(task)->GetGroup(group);
    }
    case Method::kStopRuntime: {
      return reinterpret_cast<StopRuntimeTask*>(task)->GetGroup(group);
    }
    case Method::kSetWorkOrchQueuePolicy: {
      return reinterpret_cast<SetWorkOrchQueuePolicyTask*>(task)->GetGroup(group);
    }
    case Method::kSetWorkOrchProcPolicy: {
      return reinterpret_cast<SetWorkOrchProcPolicyTask*>(task)->GetGroup(group);
    }
  }
  return -1;
}

#endif  // HERMES_RUN_HERMES_RUN_ADMIN_METHODS_H_