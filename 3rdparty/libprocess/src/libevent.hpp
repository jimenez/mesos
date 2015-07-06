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

#ifndef __LIBEVENT_HPP__
#define __LIBEVENT_HPP__

#include <event2/event.h>

#include <stout/lambda.hpp>
#include <stout/thread.hpp>

namespace process {

// Event loop.
extern event_base* base;


// Per thread bool pointer. The extra level of indirection from
// _in_event_loop_ to __in_event_loop__ is used in order to take
// advantage of the ThreadLocal operators without needing the extra
// dereference as well as lazily construct the actual bool.
extern ThreadLocal<bool>* _in_event_loop_;


#define __in_event_loop__ *(*_in_event_loop_ == NULL ?               \
  *_in_event_loop_ = new bool(false) : *_in_event_loop_)


enum EventLoopLogicFlow {
  ALLOW_SHORT_CIRCUIT,
  DISALLOW_SHORT_CIRCUIT
};


void run_in_event_loop(
    const lambda::function<void(void)>& f,
    EventLoopLogicFlow event_loop_logic_flow = ALLOW_SHORT_CIRCUIT);

} // namespace process {

#endif // __LIBEVENT_HPP__
