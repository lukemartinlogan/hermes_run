//
// Created by llogan on 7/1/23.
//

#ifndef HERMES_RUN_INCLUDE_HERMES_RUN_QUEUE_MANAGER_QUEUE_FACTORY_H_
#define HERMES_RUN_INCLUDE_HERMES_RUN_QUEUE_MANAGER_QUEUE_FACTORY_H_

#include "queues/hshm_queue.h"

namespace hrun {

#ifdef QUEUE_TYPE
using MultiQueue = MultiQueueT<QUEUE_TYPE>;
#else
using MultiQueue = MultiQueueT<Hshm>;
#endif
}  // namespace hrun

#endif  // HERMES_RUN_INCLUDE_HERMES_RUN_QUEUE_MANAGER_QUEUE_FACTORY_H_
