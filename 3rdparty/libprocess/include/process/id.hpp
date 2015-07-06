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

#ifndef __PROCESS_ID_HPP__
#define __PROCESS_ID_HPP__

/** @file */

#include <string>

namespace process {
namespace ID {

/**
 * Returns 'prefix(N)' where N represents the number of instances
 * where the same prefix (wrt. string value equality) has been used
 * to generate an ID.
 *
 * @param prefix The prefix to base the result.
 * @return An "ID" in the shape 'prefix(N)'.
 */
std::string generate(const std::string& prefix = "");

} // namespace ID {
} // namespace process {

#endif // __PROCESS_ID_HPP__
