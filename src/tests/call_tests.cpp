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
using process::http::BadRequest;
using process::http::MethodNotAllowed;
using process::http::NotAcceptable;
using process::http::NotImplemented;
using process::http::Response;
using process::http::UnsupportedMediaType;

namespace mesos {
namespace internal {
namespace tests {


class CallTest : public MesosTest {};


// Testing /master/call so this endopoint returns
// 202 Accepted.
TEST_F(CallTest, CallEndpointPost)
{
  master::Flags flags = CreateMasterFlags();
  flags.credentials = None();
  Try<PID<Master> > master = StartMaster(flags);
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"] = "application/x-protobuf";
  headers["Accept"] = "application/x-protobuf";
  headers["Connection"] = "close";

  Call call;
  call.set_type(Call::SUBSCRIBE);
  call.mutable_subscribe()->
    mutable_framework_info()->CopyFrom(DEFAULT_FRAMEWORK_INFO);

  string body;
  call.SerializeToString(&body);

  Future<Response> response = process::http::post(
                                                  master.get(),
                                                  "call",
                                                  headers,
                                                  body);

  AWAIT_READY(response);
  // TODO(ijimenez): Once the stream for subscribe is implemented
  // change this test to reflect new behaviour.
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(NotImplemented().status, response);
}


TEST_F(CallTest, CallEndpointGet)
{
  Try<PID<Master> > master = StartMaster();
  ASSERT_SOME(master);

  Future<Response> response = process::http::get(master.get(), "call");

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(MethodNotAllowed().status, response);
}


TEST_F(CallTest, CallEndpointWrongHeaderContentType)
{
  master::Flags flags = CreateMasterFlags();
  flags.credentials = None();
  Try<PID<Master> > master = StartMaster(flags);
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"] = "foo";

  Call call;
  // Setting arbitrary type since all types of Call must specify
  // a Content-Type HTTP header.
  call.set_type(Call::ACCEPT);

  string body;
  call.SerializeToString(&body);

  Future<Response> response = process::http::post(
                                                  master.get(),
                                                  "call",
                                                  headers,
                                                  body);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(UnsupportedMediaType().status, response);
}


TEST_F(CallTest, CallEndpointMissingHeaderContentType)
{
  master::Flags flags = CreateMasterFlags();
  flags.credentials = None();
  Try<PID<Master> > master = StartMaster(flags);
  ASSERT_SOME(master);

  Call call;
  // Setting arbitrary type since all types of Call must specify
  // a Content-Type HTTP header. Content-Type is the only header
  // calls besides SUBSCRIBE musht specify.
  call.set_type(Call::ACCEPT);

  string body;
  call.SerializeToString(&body);

  Future<Response> response = process::http::post(
                                                  master.get(),
                                                  "call",
                                                  None(),
                                                  body);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(BadRequest().status, response);
}


TEST_F(CallTest, CallEndpointWrongHeaderAccept)
{
  master::Flags flags = CreateMasterFlags();
  flags.credentials = None();
  Try<PID<Master> > master = StartMaster(flags);
  ASSERT_SOME(master);

  hashmap<string, string> headers;
  headers["Content-Type"] = "application/x-protobuf";
  headers["Accept"] = "foo";
  headers["Connection"] = "close";

  Call call;
  // Setting SUBSCRIBE type since it's the only Call that needs
  // Accept HTTP header validation.
  call.set_type(Call::SUBSCRIBE);

  string body;
  call.SerializeToString(&body);
  Future<Response> response = process::http::post(
                                                  master.get(),
                                                  "call",
                                                  headers,
                                                  body);

  AWAIT_READY(response);
  AWAIT_EXPECT_RESPONSE_STATUS_EQ(NotAcceptable().status, response);
}


} // namespace tests {
} // namespace internal {
} // namespace mesos {
