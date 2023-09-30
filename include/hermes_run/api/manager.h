//
// Created by lukemartinlogan on 6/27/23.
//

#ifndef HERMES_RUN_INCLUDE_HERMES_RUN_MANAGER_MANAGER_H_
#define HERMES_RUN_INCLUDE_HERMES_RUN_MANAGER_MANAGER_H_

#include "hermes_run/hermes_run_types.h"
#include "hermes_run/hermes_run_constants.h"
#include "hermes_run/config/config_client.h"
#include "hermes_run/config/config_server.h"
#include "hermes_run/queue_manager/queue_manager.h"

namespace hrun {

/** Shared-memory header for LabStor */
struct HermesRunShm {
  u32 node_id_;
  QueueManagerShm queue_manager_;
  std::atomic<u64> unique_;
  u64 num_nodes_;
};

/** The configuration used inherited by runtime + client */
class ConfigurationManager {
 public:
  HermesRunMode mode_;
  HermesRunShm *header_;
  ClientConfig client_config_;
  ServerConfig server_config_;
  static inline const hipc::allocator_id_t main_alloc_id_ =
      hipc::allocator_id_t(0, 1);
  hipc::Allocator *main_alloc_;
  bool is_being_initialized_;
  bool is_initialized_;
  bool is_terminated_;
  bool is_transparent_;
  hshm::Mutex lock_;
  hshm::ThreadType thread_type_;

  /** Default constructor */
  ConfigurationManager() : is_being_initialized_(false),
                           is_initialized_(false),
                           is_terminated_(false),
                           is_transparent_(false) {}

  /** Destructor */
  ~ConfigurationManager() {}

  /** Whether or not HermesRun is currently being initialized */
  bool IsBeingInitialized() { return is_being_initialized_; }

  /** Whether or not HermesRun is initialized */
  bool IsInitialized() { return is_initialized_; }

  /** Whether or not HermesRun is finalized */
  bool IsTerminated() { return is_terminated_; }

  /** Load the server-side configuration */
  void LoadServerConfig(std::string config_path) {
    if (config_path.empty()) {
      config_path = Constants::GetEnvSafe(Constants::kServerConfEnv);
    }
    HILOG(kInfo, "Loading server configuration: {}", config_path);
    server_config_.LoadFromFile(config_path);
  }

  /** Load the client-side configuration */
  void LoadClientConfig(std::string config_path) {
    if (config_path.empty()) {
      config_path = Constants::GetEnvSafe(Constants::kClientConfEnv);
    }
    HILOG(kDebug, "Loading client configuration: {}", config_path);
    client_config_.LoadFromFile(config_path);
  }

  /** Get number of nodes */
  HSHM_ALWAYS_INLINE int GetNumNodes() {
    return server_config_.rpc_.host_names_.size();
  }
};

}  // namespace hrun

#endif  // HERMES_RUN_INCLUDE_HERMES_RUN_MANAGER_MANAGER_H_
