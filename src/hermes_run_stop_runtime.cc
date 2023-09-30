//
// Created by lukemartinlogan on 6/22/23.
//

#include "hermes_run_admin/hermes_run_admin.h"

int main() {
  TRANSPARENT_HERMES_RUN();
  HERMES_RUN_ADMIN->AsyncStopRuntimeRoot(hrun::DomainId::GetLocal());
}