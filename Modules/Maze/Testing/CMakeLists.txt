#############################################################################################################
#
# HUC - Hurna Core
#
# Copyright (c) Michael Jeulin-Lagarrigue
#
#  Licensed under the MIT License, you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#         https://github.com/Hurna/Hurna-Core/blob/master/LICENSE
#
# Unless required by applicable law or agreed to in writing, software distributed under the License is
# distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and limitations under the License.
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
#############################################################################################################

set(HUC ${PROJECT_NAME})

# Source files
set(MODULE_MAZE_SRCS TestBinaryTreeGen.cxx
                     TestDFSGen.cxx
                     TestKruskalsGen.cxx
                     TestPrimsGen.cxx
                     TestRecursiveDivisionGen.cxx
                     TestSidewinderGen.cxx)

# --------------------------------------------------------------------------
# Build Testing executables
# --------------------------------------------------------------------------
include_directories(${MODULES_DIR})
cxx_gtest(TestModuleMaze "${MODULE_MAZE_SRCS}" ${HUC_SRCS})
