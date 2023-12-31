//
// Created by llogan on 7/1/23.
//

#include "basic_test.h"
#include <mpi.h>
#include "hermes_run/api/hermes_run_client.h"
#include "hermes_run_admin/hermes_run_admin.h"

#include "small_message/small_message.h"
#include "hermes_shm/util/timer.h"
#include "hermes_run/work_orchestrator/affinity.h"
#include "omp.h"

TEST_CASE("TestIpc") {
  int rank, nprocs;
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  hrun::small_message::Client client;
  HERMES_RUN_ADMIN->RegisterTaskLibRoot(hrun::DomainId::GetGlobal(), "small_message");
  client.CreateRoot(hrun::DomainId::GetGlobal(), "ipc_test");
  MPI_Barrier(MPI_COMM_WORLD);
  hshm::Timer t;

  int pid = getpid();
  ProcessAffiner::SetCpuAffinity(pid, 8);

  t.Resume();
  size_t ops = 256;
  for (size_t i = 0; i < ops; ++i) {
    int ret;
    HILOG(kInfo, "Sending message {}", i);
    int node_id = 1 + ((rank + 1) % nprocs);
    ret = client.MdRoot(hrun::DomainId::GetNode(node_id));
    REQUIRE(ret == 1);
  }
  t.Pause();

  HILOG(kInfo, "Latency: {} MOps", ops / t.GetUsec());
}

void TestIpcMultithread(int nprocs) {
  hrun::small_message::Client client;
  HERMES_RUN_ADMIN->RegisterTaskLibRoot(hrun::DomainId::GetGlobal(), "small_message");
  client.CreateRoot(hrun::DomainId::GetGlobal(), "ipc_test");

#pragma omp parallel shared(client, nprocs) num_threads(nprocs)
  {
    int rank = omp_get_thread_num();
    size_t ops = 256;
    for (size_t i = 0; i < ops; ++i) {
      int ret;
      HILOG(kInfo, "Sending message {}", i);
      int node_id = 1 + ((rank + 1) % nprocs);
      ret = client.MdRoot(hrun::DomainId::GetNode(node_id));
      REQUIRE(ret == 1);
    }
  }
}

TEST_CASE("TestIpcMultithread4") {
  TestIpcMultithread(4);
}

TEST_CASE("TestIpcMultithread8") {
  TestIpcMultithread(8);
}

TEST_CASE("TestIpcMultithread16") {
  TestIpcMultithread(16);
}

TEST_CASE("TestIpcMultithread32") {
  TestIpcMultithread(32);
}

TEST_CASE("TestIO") {
  int rank, nprocs;
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  hrun::small_message::Client client;
  HERMES_RUN_ADMIN->RegisterTaskLibRoot(hrun::DomainId::GetGlobal(), "small_message");
  client.CreateRoot(hrun::DomainId::GetGlobal(), "ipc_test");
  hshm::Timer t;

  int pid = getpid();
  ProcessAffiner::SetCpuAffinity(pid, 8);

  HILOG(kInfo, "Starting IO test: {}", nprocs);

  t.Resume();
  size_t ops = 16;
  for (size_t i = 0; i < ops; ++i) {
    int ret;
    HILOG(kInfo, "Sending message {}", i);
    int node_id = 1 + ((rank + 1) % nprocs);
    ret = client.IoRoot(hrun::DomainId::GetNode(node_id));
    REQUIRE(ret == 1);
  }
  t.Pause();

  HILOG(kInfo, "Latency: {} MOps", ops / t.GetUsec());
}

//TEST_CASE("TestHostfile") {
//  for (u32 node_id = 1; node_id < HERMES_RUN_THALLIUM->rpc_->hosts_.size() + 1; ++node_id) {
//    HILOG(kInfo, "Node {}: {}", node_id, HERMES_RUN_THALLIUM->GetServerName(node_id));
//  }
//}