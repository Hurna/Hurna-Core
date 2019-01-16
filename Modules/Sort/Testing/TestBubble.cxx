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
#include <bubble.hxx>

// STD includes
#include <functional>
#include <vector>
#include <string>

// Testing namespace
using namespace huc::sort;

#ifndef DOXYGEN_SKIP
namespace {
  typedef std::vector<int> Container;
  typedef Container::iterator IT;
  typedef std::greater<IT::value_type> GE_Comparator;

  const Container ArraySort = {-3, -2, 0, 2, 8, 15, 36, 212, 366};  // sorted with negative values
  const Container ArrayRand = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5}; // random with negative values
  const std::string RandStr = "xacvgeze";
}
#endif /* DOXYGEN_SKIP */

// Basic Bubble-Sort tests
TEST(TestBubble, BubbleSorts)
{
  // Normal Run
  {
    Container vector(ArrayRand);
    Bubble<IT>(vector.begin(), vector.end());

    // All elements are sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already ArraySort - Array should not be affected
  {
    Container vector(ArraySort);
    Bubble<IT>(vector.begin(), vector.end());

    // All elements are still sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container vector(ArrayRand);
    Bubble<IT>(vector.end(), vector.begin());

    int i = 0;
    for (auto it = vector.begin(); it < vector.end(); ++it, ++i)
      EXPECT_EQ(ArrayRand[i], *it);
  }

  // No error unitialized array
  {
    Container emptyArray;
    Bubble<IT>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    Bubble<IT>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String - String should be sorted as an array
  {
    std::string stringToSort = RandStr;
    Bubble<std::string::iterator, std::less<char>>(stringToSort.begin(), stringToSort.end());
    for (auto it = stringToSort.begin(); it < stringToSort.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Basic Bubble-Sort tests - Inverse Order
TEST(TestBubble, BubbleGreaterComparator)
{
  // Normal Run - Elements should be sorted in inverse order
  {
    Container vector(ArrayRand);
    Bubble<IT, GE_Comparator>(vector.begin(), vector.end());

    // All elements are sorted in inverse order
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // Already sorted Array in inverse order - Array should not be affected
  {
    Container vector(ArraySort);
    Bubble<IT, GE_Comparator>(vector.begin(), vector.end());

    // All elements are still sorted in inverse order
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }

  // String - String should be sorted in inverse order
  {
    std::string stringToSort = RandStr;
    Bubble<std::string::iterator, std::greater<char>>(stringToSort.begin(), stringToSort.end());

    // All elements are sorted in inverse order
    for (auto it = stringToSort.begin(); it < stringToSort.end() - 1; ++it)
      EXPECT_GE(*it, *(it + 1));
  }
}
