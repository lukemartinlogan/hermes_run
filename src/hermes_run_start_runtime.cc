//
// Created by lukemartinlogan on 6/17/23.
//

#include "hermes_shm/util/singleton.h"
#include "hermes_run/api/hermes_run_runtime.h"

int main(int argc, char **argv) {
  HERMES_RUN_RUNTIME->Create();
  HERMES_RUN_RUNTIME->RunDaemon();
  HERMES_RUN_RUNTIME->Finalize();
  return 0;
}
