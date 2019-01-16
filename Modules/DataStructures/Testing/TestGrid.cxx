/*===========================================================================================================
 *
 * HUC - Hurna Core
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/Hurna/Hurna-Core/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#include <gtest/gtest.h>
#include <grid.hxx>

#include <functional>
#include <list>

using namespace huc;

#ifndef DOXYGEN_SKIP
namespace {

}
#endif /* DOXYGEN_SKIP */

// Test Grid Construction
TEST(TestGrid, build)
{
  // Empty Grid
  {
    Grid<CellInfoBase> grid(0, 0);
    EXPECT_EQ(grid.Height(), 0);
    EXPECT_EQ(grid.Width(), 0);
  }

  // Empty Connected Grid
  {
    Grid<CellInfoBase> grid(0, 0, true);
    EXPECT_EQ(grid.Height(), 0);
    EXPECT_EQ(grid.Width(), 0);
  }

  // 10 x 10 Grid
  {
    Grid<CellInfoBase> grid(10, 10);
    EXPECT_EQ(grid.Height(), 10);
    EXPECT_EQ(grid.Width(), 10);
  }

  // 10 x 10 Connected Grid
  {
    Grid<CellInfoBase> grid(10, 10, true);
    EXPECT_EQ(grid.Height(), 10);
    EXPECT_EQ(grid.Width(), 10);
  }
}

// TODO Complete UTs for all API!
