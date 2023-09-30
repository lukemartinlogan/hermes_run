//
// Created by lukemartinlogan on 8/14/23.
//

#ifndef HERMES_RUN_INCLUDE_HERMES_RUN_HERMES_RUN_NAMESPACE_H_
#define HERMES_RUN_INCLUDE_HERMES_RUN_HERMES_RUN_NAMESPACE_H_

#include "hermes_run/api/hermes_run_client.h"
#include "hermes_run/task_registry/task_lib.h"

using hrun::TaskMethod;
using hrun::BinaryOutputArchive;
using hrun::BinaryInputArchive;
using hrun::Task;
using hrun::TaskPointer;
using hrun::MultiQueue;
using hrun::PriorityInfo;
using hrun::Task;
using hrun::TaskFlags;
using hrun::DataTransfer;
using hrun::TaskLib;
using hrun::TaskLibClient;
using hrun::config::QueueManagerInfo;
using hrun::TaskPrio;
using hrun::RunContext;

using hshm::RwLock;
using hshm::Mutex;
using hshm::bitfield;
using hshm::bitfield32_t;
typedef hshm::bitfield<uint64_t> bitfield64_t;
using hshm::ScopedRwReadLock;
using hshm::ScopedRwWriteLock;
using hipc::LPointer;

#endif  // HERMES_RUN_INCLUDE_HERMES_RUN_HERMES_RUN_NAMESPACE_H_
