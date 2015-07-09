/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __COMMON_HTTP_VALIDATION_HPP__
#define __COMMON_HTTP_VALIDATION_HPP__

#include <string>

#include <mesos/mesos.hpp>

#include "common/http_validation.hpp"

using std::string;

using process::http::BadRequest;
using process::http::NotAcceptable;
using process::http::UnsupportedMediaType;

namespace mesos {
namespace internal {
namespace validation {
namespace http {
namespace header {

Option<process::http::Response>  validate(
    const process::http::Request& request)
{
  Option<string> connection = request.headers.get("Connection");

  if (request.acceptsMediaType("").isNone()) {
    return BadRequest("Missing Accept header");
  }
  if (connection.isNone()) {
    return BadRequest("Missing Connection header");
  }
  if (!request.acceptsMediaType("application/json").get() &&
      !request.acceptsMediaType("application/x-protobuf").get()) {
    return NotAcceptable("Unsupported Accept: '" + request.headers.get("Accept").get() +
                         "'; Expecting one of (application/x-protobuf" +
                         ", application/json)");
     }
  if (connection.get() != "close") {
    return UnsupportedMediaType("Unsupported '" + connection.get() +
                                "' Connection header; Expecting close");
  }
  // Not returning a response after validation so it can be
  // determined for each call.
  return None();
}

} // namespace header {
} // namespace http {
} // namespace validation {
} // namespace internal {
} // namespace mesos {

#endif // __COMMON_HTTP_VALIDATION_HPP__
