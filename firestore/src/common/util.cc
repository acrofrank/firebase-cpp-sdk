/*
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "firestore/src/common/util.h"

#include <cctype>
#include <chrono>
#include <ctime>
#include <string>

#include "app/src/log.h"

namespace firebase {
namespace firestore {

const std::string& EmptyString() {
  static const std::string kEmptyString;
  return kEmptyString;
}

namespace detail {

std::string FormattedTimestamp() {
  auto timestamp = std::chrono::system_clock::now();
  std::time_t ctime_timestamp = std::chrono::system_clock::to_time_t(timestamp);
  std::string formatted_timestamp(std::ctime(&ctime_timestamp));
  while (formatted_timestamp.size() > 0) {
    auto last_char = formatted_timestamp[formatted_timestamp.size() - 1];
    if (!std::isspace(last_char)) {
      break;
    }
    formatted_timestamp.pop_back();
  }
  return formatted_timestamp;
}

void UnityIssue1154TestLog0(std::ostringstream& ss) {
  LogInfo(ss.str().c_str());
}

}

}  // namespace firestore
}  // namespace firebase
