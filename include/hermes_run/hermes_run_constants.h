//
// Created by lukemartinlogan on 6/22/23.
//

#ifndef HERMES_RUN_INCLUDE_HERMES_RUN_HERMES_RUN_CONSTANTS_H_
#define HERMES_RUN_INCLUDE_HERMES_RUN_HERMES_RUN_CONSTANTS_H_

namespace hrun {

#include <string>

class Constants {
 public:
  inline static const std::string kClientConfEnv = "HERMES_RUN_CLIENT_CONF";
  inline static const std::string kServerConfEnv = "HERMES_RUN_SERVER_CONF";

  static std::string GetEnvSafe(const std::string &env_name) {
    char *data = getenv(env_name.c_str());
    if (data == nullptr) {
      return "";
    } else {
      return data;
    }
  }
};

}  // namespace hrun

#endif  // HERMES_RUN_INCLUDE_HERMES_RUN_HERMES_RUN_CONSTANTS_H_
