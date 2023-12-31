//
// Created by lukemartinlogan on 6/27/23.
//

#include "hermes_run/api/hermes_run_runtime.h"
#include "hermes_run/work_orchestrator/worker.h"
#include "hermes_run/work_orchestrator/work_orchestrator.h"

namespace hrun {

void Worker::Loop() {
  pid_ = GetLinuxTid();
  WorkOrchestrator *orchestrator = HERMES_RUN_WORK_ORCHESTRATOR;
  while (orchestrator->IsAlive()) {
    try {
      Run();
    } catch (hshm::Error &e) {
      HELOG(kFatal, "(node {}) Worker {} caught an error: {}", HERMES_RUN_CLIENT->node_id_, id_, e.what());
    }
    // Yield();
  }
  Run();
}

void Worker::Run() {
  if (poll_queues_.size() > 0) {
    _PollQueues();
  }
  if (relinquish_queues_.size() > 0) {
    _RelinquishQueues();
  }
  for (WorkEntry &work_entry : work_queue_) {
    if (!work_entry.lane_->flags_.Any(QUEUE_LOW_LATENCY)) {
      work_entry.count_ += 1;
      if (work_entry.count_ % 4096 != 0) {
        continue;
      }
    }
    PollGrouped(work_entry);
  }
}

void Worker::PollGrouped(WorkEntry &work_entry) {
  int off = 0;
  Lane *&lane = work_entry.lane_;
  Task *task;
  LaneData *entry;
  for (int i = 0; i < 1024; ++i) {
    // Get the task message
    if (lane->peek(entry, off).IsNull()) {
      return;
    }
    if (entry->complete_) {
      PopTask(lane, off);
      continue;
    }
    task = HERMES_RUN_CLIENT->GetPrivatePointer<Task>(entry->p_);
    RunContext &ctx = task->ctx_;
    ctx.lane_id_ = work_entry.lane_id_;
    // Get the task state
    TaskState *&exec = ctx.exec_;
    exec = HERMES_RUN_TASK_REGISTRY->GetTaskState(task->task_state_);
    if (!exec) {
      HELOG(kFatal, "(node {}) Could not find the task state: {}",
            HERMES_RUN_CLIENT->node_id_, task->task_state_);
      entry->complete_ = true;
      EndTask(lane, exec, task, off);
      continue;
    }
    // Attempt to run the task if it's ready and runnable
    bool is_remote = task->domain_id_.IsRemote(HERMES_RUN_RPC->GetNumHosts(), HERMES_RUN_CLIENT->node_id_);
    if (!task->IsRunDisabled() && CheckTaskGroup(task, exec, work_entry.lane_id_, task->task_node_, is_remote)) {
      // TODO(llogan): Make a remote debug macro
#ifdef REMOTE_DEBUG
      if (task->task_state_ != HERMES_RUN_QM_CLIENT->admin_task_state_ &&
          !task->task_flags_.Any(TASK_REMOTE_DEBUG_MARK) &&
          task->method_ != TaskMethod::kConstruct &&
          HERMES_RUN_RUNTIME->remote_created_) {
        is_remote = true;
      }
      task->task_flags_.SetBits(TASK_REMOTE_DEBUG_MARK);
#endif
      // Execute or schedule task
      if (is_remote) {
        auto ids = HERMES_RUN_RUNTIME->ResolveDomainId(task->domain_id_);
        HERMES_RUN_REMOTE_QUEUE->Disperse(task, exec, ids);
        task->DisableRun();
        task->SetUnordered();
        task->UnsetCoroutine();
      } else if (task->IsCoroutine()) {
        if (!task->IsStarted()) {
          ctx.stack_ptr_ = malloc(ctx.stack_size_);
          if (ctx.stack_ptr_ == nullptr) {
            HILOG(kFatal, "The stack pointer of size {} is NULL",
                  ctx.stack_size_, ctx.stack_ptr_);
          }
          ctx.jmp_.fctx = bctx::make_fcontext(
              (char*)ctx.stack_ptr_ + ctx.stack_size_,
              ctx.stack_size_, &RunBlocking);
          task->SetStarted();
        }
        ctx.jmp_ = bctx::jump_fcontext(ctx.jmp_.fctx, task);
        HILOG(kDebug, "Jumping into function")
      } else if (task->IsPreemptive()) {
        task->DisableRun();
        entry->thread_ = HERMES_RUN_WORK_ORCHESTRATOR->SpawnAsyncThread(&Worker::RunPreemptive, task);
      } else {
        task->SetStarted();
        exec->Run(task->method_, task, ctx);
      }
    }
    // Cleanup on task completion
    if (task->IsModuleComplete()) {
//      HILOG(kDebug, "(node {}) Ending task: task_node={} task_state={} lane={} queue={} worker={}",
//            HERMES_RUN_CLIENT->node_id_, task->task_node_, task->task_state_, lane_id, queue->id_, id_);
      entry->complete_ = true;
      if (task->IsCoroutine()) {
        // TODO(llogan): verify leak
        free(ctx.stack_ptr_);
      } else if (task->IsPreemptive()) {
        ABT_thread_join(entry->thread_);
      }
      RemoveTaskGroup(task, exec, work_entry.lane_id_, is_remote);
      EndTask(lane, exec, task, off);
    } else {
      off += 1;
    }
  }
}

void Worker::RunBlocking(bctx::transfer_t t) {
  Task *task = reinterpret_cast<Task*>(t.data);
  RunContext &ctx = task->ctx_;
  TaskState *&exec = ctx.exec_;
  ctx.jmp_ = t;
  exec->Run(task->method_, task, ctx);
  task->Yield<TASK_YIELD_CO>();
}

void Worker::RunPreemptive(void *data) {
  Task *task = reinterpret_cast<Task *>(data);
  TaskState *exec = HERMES_RUN_TASK_REGISTRY->GetTaskState(task->task_state_);
  RunContext ctx(0);
  exec->Run(task->method_, task, ctx);
}

}  // namespace hrun