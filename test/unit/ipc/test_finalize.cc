//
// Created by llogan on 7/1/23.
//

#include "basic_test.h"
#include "hermes_run/api/hermes_run_client.h"
#include "hermes_run_admin/hermes_run_admin.h"

TEST_CASE("TestFinalize") {
  HERMES_RUN_ADMIN->AsyncStopRuntimeRoot(hrun::DomainId::GetGlobal());
}