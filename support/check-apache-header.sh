str="\/\*\* Licensed to the Apache Software Foundation \(ASF\) under one\n\
\* or more contributor license agreements\.  See the NOTICE file\n\
\* distributed with this work for additional information\n\
\* regarding copyright ownership\.  The ASF licenses this file\n\
\* to you under the Apache License, Version 2\.0 \(the\n\
\* \"License\"\); you may not use this file except in compliance\n\
\* with the License.  You may obtain a copy of the License at\n\
\*\n\
\*     http\:\/\/www\.apache\.org\/licenses\/LICENSE-2\.0\n\
\*\n\
\* Unless required by applicable law or agreed to in writing, software\n\
\* distributed under the License is distributed on an \"AS IS\" BASIS,\n\
\* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied\.\n\
\* See the License for the specific language governing permissions and\n\
\* limitations under the License\n\
\*\/\n"

makeStr="# Licensed to the Apache Software Foundation \(ASF\) under one\n\
# or more contributor license agreements\.  See the NOTICE file\n\
# distributed with this work for additional information\n\
# regarding copyright ownership\.  The ASF licenses this file\n\
# to you under the Apache License, Version 2\.0 \(the\n\
# \"License\"\); you may not use this file except in compliance\n\
# with the License.  You may obtain a copy of the License at\n\
#\n\
#     http\:\/\/www\.apache\.org\/licenses\/LICENSE-2\.0\n\
#\n\
# Unless required by applicable law or agreed to in writing, software\n\
# distributed under the License is distributed on an \"AS IS\" BASIS,\n\
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied\.\n\
# See the License for the specific language governing permissions and\n\
# limitations under the License\n"

count=0
for file in `find . -type f -name '*pp'` ; do
    if ! grep -q "http\:\/\/www\.apache\.org\/licenses\/LICENSE-2\.0" $file
    then
        ((count++))
        echo $file
        sed -i "1 i $str" $file
     fi
 done

for file in `find . -type f -name 'Makefile.am'` ; do
    if ! grep -q "http\:\/\/www\.apache\.org\/licenses\/LICENSE-2\.0" $file
    then
        ((count++))
        echo $file
        sed -i "1 i $makeStr" $file
     fi
 done

echo $count " files where modified"