#ifndef HERMES_RUN_WORCH_QUEUE_ROUND_ROBIN_METHODS_H_
#define HERMES_RUN_WORCH_QUEUE_ROUND_ROBIN_METHODS_H_

/** The set of methods in the admin task */
struct Method : public TaskMethod {
  TASK_METHOD_T kSchedule = kLast + 0;
};

#endif  // HERMES_RUN_WORCH_QUEUE_ROUND_ROBIN_METHODS_H_