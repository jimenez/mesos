/** Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License
*/

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include <process/clock.hpp>
#include <process/time.hpp>

#include <stout/duration.hpp>
#include <stout/gtest.hpp>
#include <stout/os.hpp>

using namespace process;


TEST(TimeTest, Arithmetic)
{
  Time t = Time::epoch() + Weeks(1000);
  t -= Weeks(1);
  EXPECT_EQ(Time::epoch() + Weeks(999), t);

  t += Weeks(2);
  EXPECT_EQ(Time::epoch() + Weeks(1001), t);

  EXPECT_EQ(t, Time::epoch() + Weeks(1000) + Weeks(1));
  EXPECT_EQ(t, Time::epoch() + Weeks(1002) - Weeks(1));

  EXPECT_EQ(Weeks(1),
            (Time::epoch() + Weeks(1000)) - (Time::epoch() + Weeks(999)));
}


TEST(TimeTest, Now)
{
  Time t1 = Clock::now();
  os::sleep(Microseconds(10));
  ASSERT_LT(Microseconds(10), Clock::now() - t1);
}


TEST(TimeTest, Output)
{
  EXPECT_EQ("1989-03-02 00:00:00+00:00",
            stringify(Time::epoch() + Weeks(1000)));
  EXPECT_EQ("1989-03-02 00:00:00.000000001+00:00",
            stringify(Time::epoch() + Weeks(1000) + Nanoseconds(1)));
  EXPECT_EQ("1989-03-02 00:00:00.000001000+00:00",
            stringify(Time::epoch() + Weeks(1000) + Microseconds(1)));
}
