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

#include <gmock/gmock.h>

#include <string>

#include <process/future.hpp>
#include <process/gmock.hpp>
#include <process/http.hpp>
#include <process/pid.hpp>

#include <stout/option.hpp>

#include "master/master.hpp"

#include "tests/mesos.hpp"
#include "tests/utils.hpp"

using std::string;

using mesos::internal::master::Master;

using mesos::scheduler::Call;

using process::Future;
using process::PID;

using process::http::Accepted;
using process::http::Response;

using testing::_;
using testing::Eq;
using testing::Return;

namespace mesos {
namespace internal {
namespace tests {


class CallTest : public MesosTest {};


// Testing /master/call so this endopoint returns
// 202 Accepted.

// Testing route
TEST_F(CallTest, CallEndpointPost)
{
  Try<PID<Master> > master = StartMaster();
  ASSERT_SOME(master);

  Future<Response> response = process::http::post(
      master.get(),
      "call",
      None(),
      None());

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(Accepted().status, response);
}


TEST_F(CallTest, CallEndpointGet)
{
  Try<PID<Master> > master = StartMaster();
  ASSERT_SOME(master);

  Future<Response> response = process::http::get();

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(
    BadRequest("Expecting POST").status, response);
}


TEST_F(HTTPCallTest, CallEndpointWrongHeaderContentType)
{
  Try<PID<Master>> master = StartMaster();
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"] = "foo";

  {
    Call call;
    call.mutable_framework_info()->CopyFrom(DEFAULT_FRAMEWORK_INFO);
    // Setting arbitrary type since all types of Call must specify
    // a Content-Type HTTP header.
    call.set_type(Call::ACCEPT);
  }

  Future<Response> response = process::http::post(
      master.get(),
      "call",
      headers,
      call);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(
    BadRequest("Unsupported content-type header").status, response);
}


TEST_F(HTTPCallTest, CallEndpointMissingHeaderContentType)
{
  Try<PID<Master>> master = StartMaster();
  ASSERT_SOME(master);

  {
    Call call;
    call.mutable_framework_info()->CopyFrom(DEFAULT_FRAMEWORK_INFO);
    // Setting arbitrary type
    call.set_type(Call::ACCEPT);
  }

  Future<Response> response = process::http::post(
      master.get(),
      "call",
      None(),
      call);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(
    BadRequest("Missing content-type header").status, response);
}


TEST_F(HTTPCallTest, CallEndpointWrongHeaderAccept)
{
  Try<PID<Master>> master = StartMaster();
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"] = "application/x-protobuf";
  headers["Accept"] = "foo";
  headers["Connection"] = "keep-alive";

  {
    Call call;
    call.mutable_framework_info()->CopyFrom(DEFAULT_FRAMEWORK_INFO);
    // Setting SUBSCRIBE type since it's the only Call that needs
    // Accept HTTP header validation.
    call.set_type(Call::SUBSCRIBE);
  }

  Future<Response> response = process::http::post(
      master.get(),
      "call",
      headers);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(
    NotAcceptable("Unsupported accept header").status, response);
}


TEST_F(HTTPCallTest, CallEndpointMissingHeaderAccept)
{
  Try<PID<Master>> master = StartMaster();
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"] = "application/x-protobuf";
  headers["Connection"] = "keep-alive";

  {
    Call call;
    call.mutable_framework_info()->CopyFrom(DEFAULT_FRAMEWORK_INFO);
    // Setting SUBSCRIBE type since it's the only Call that needs
    // Accept HTTP header validation.
    call.set_type(Call::SUBSCRIBE);
  }

  Future<Response> response = process::http::post(
      master.get(),
      "call",
      headers);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(
    BadRequest("Missing accept header").status, response);
}


TEST_F(HTTPCallTest, CallEndpointWrongHeaderConnection)
{
  Try<PID<Master>> master = StartMaster();
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"];
  headers["Connection"] = "foo";
  headers["Accept"];

  {
    Call call;
    call.mutable_framework_info()->CopyFrom(DEFAULT_FRAMEWORK_INFO);
    // Setting SUBSCRIBE type since it's the only Call that needs
    // Accept HTTP header validation.
    call.set_type(Call::SUBSCRIBE);
  }

  Future<Response> response = process::http::post(
      master.get(),
      "call",
      headers);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(
    BadRequest("Unsupported connection header").status, response);
}


TEST_F(HTTPCallTest, CallEndpointMissingHeaderConnection)
{
  Try<PID<Master>> master = StartMaster();
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"];
  headers["Accept"];

  {
    Call call;
    call.mutable_framework_info()->CopyFrom(DEFAULT_FRAMEWORK_INFO);
    // Setting SUBSCRIBE type since it's the only Call that needs
    // Accept HTTP header validation.
    call.set_type(Call::SUBSCRIBE);
  }

  Future<Response> response = process::http::post(
      master.get(),
      "call",
      headers);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(
    BadRequest("Missing connection header").status, response);
}


} // namespace tests {
} // namespace internal {
} // namespace mesos {
