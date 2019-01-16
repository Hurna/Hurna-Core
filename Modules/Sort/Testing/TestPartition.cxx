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
#include <partition.hxx>

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
  typedef std::greater_equal<IT::value_type> GE_Compare;

  const Container ArraySort = {-3, -2, 0, 2, 8, 15, 36, 212, 366};    // Sorted with neg values
  const Container ArrayInvSort = {366, 212, 36, 15, 8, 2, 0, -2, -3}; // Inverse Sorted with neg values
  const Container ArrayRand = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};   // Random sequence
  const std::string StrRand = "xacvgeze";



  template<typename IT>
  void CheckPartition (const IT& begin, const IT& end, const IT& newPivot,
                       typename std::iterator_traits<IT>::value_type pivotVal, bool inOrder = true)
  {
    // Value of the pivot not changed
    EXPECT_EQ(pivotVal, *newPivot);

    // All elements before the pivot are smaller or equal
    if (inOrder)
      for (auto it = begin; it < newPivot; ++it)
        EXPECT_GE(pivotVal, *it);
    else
      for (auto it = begin; it < newPivot; ++it)
        EXPECT_LE(pivotVal, *it);

    // All elements before the pivot are bigger or equal
    if (inOrder)
      for (auto it = newPivot; it < end; ++it)
        EXPECT_LE(pivotVal, *it);
    else
      for (auto it = newPivot; it < end; ++it)
        EXPECT_GE(pivotVal, *it);
  }
}
#endif /* DOXYGEN_SKIP */

// Basic Partition tests
TEST(TestPartition, Partitions)
{
  // Normal Run - Random Array
  {
    Container vector(ArrayRand);
    auto pivot = vector.begin() + 5;
    auto pivotVal = *pivot;

    // Run partition - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
    auto newPivot = Partition<IT>(vector.begin(), pivot, vector.end());
    CheckPartition<IT>(vector.begin(), vector.end(), newPivot, pivotVal);
  }

  // Already sortedArray - Array should not be affected
  {
    Container vector(ArraySort);
    auto pivot = vector.begin() + 5;

    Partition<IT>(vector.begin(), pivot, vector.end());

    int i = 0;
    for (auto it = vector.begin(); it < vector.end(); ++it, ++i)
      EXPECT_EQ(ArraySort[i], *it);
  }

  // Begin and End inversed - Array should not be affected
  {
    Container vector(ArrayRand);
    auto pivot = vector.begin() + 5;

    Partition<IT>(vector.end(), pivot, vector.begin());

    int i = 0;
    for (auto it = vector.begin(); it < vector.end(); ++it, ++i)
      EXPECT_EQ(ArrayRand[i], *it);
  }
}

// String collection - Should result in: max[begin, pivot[ <= pivot <= min]pivot, end]
TEST(TestPartition, PartitionString)
{
  std::string str = StrRand;
  auto pivot = str.begin() + 5;
  auto pivotVal = *pivot;

  auto newPivot = Partition<std::string::iterator>(str.begin(), pivot, str.end());
  CheckPartition<std::string::iterator>(str.begin(), str.end(), newPivot, pivotVal);
}

// Extreme Pivot Partition tests
TEST(TestPartition, PartitionBoudaryPivots)
{
  // Pivot choose as begin
  {
    Container vector(ArrayRand);
    auto pivot = vector.begin();
    const int pivotVal = *pivot;

    // Run partition
    auto newPivot = Partition<IT>(vector.begin(), pivot, vector.end());
    CheckPartition<IT>(vector.begin(), vector.end(), newPivot, pivotVal);

  }

  // Pivot choose as last element
  {
    Container vector(ArrayRand);
    auto pivot = vector.end() - 1;
    const int pivotVal = *pivot;

    // Run partition
    auto newPivot = Partition<IT>(vector.begin(), pivot, vector.end());
    CheckPartition<IT>(vector.begin(), vector.end(), newPivot, pivotVal);
  }

  // Pivot choose as end - cannot process
  {
    Container vector(ArrayRand);
    auto pivot = vector.end();

    // Run partition
    Partition<IT>(vector.begin(), pivot, vector.end());

    int i = 0;
    for (auto it = vector.begin(); it < vector.end(); ++it, ++i)
      EXPECT_EQ(ArrayRand[i], *it);
  }
}

// Basic Partition tests - Greater element in the left partition
TEST(TestPartition, PartitionGreaterComparator)
{
  // Normal Run - Should result in: min[begin, pivot[ >= pivot >= max]pivot, end]
  {
    Container vector(ArrayRand);
    auto pivot = vector.begin() + 5;
    const auto pivotVal = *pivot;

    // Run partition
    auto newPivot = Partition<IT, GE_Compare>(vector.begin(), pivot, vector.end());
    CheckPartition<IT>(vector.begin(), vector.end(), newPivot, pivotVal, false);
  }

  // Already InverseSortedArray - Array should not be affected
  {
    Container invSortedArray(ArrayInvSort);
    auto pivot = invSortedArray.begin() + 5;

    Partition<IT, GE_Compare>(invSortedArray.begin(), pivot, invSortedArray.end());

    int i = 0;
    for (auto it = invSortedArray.begin(); it < invSortedArray.end(); ++it, ++i)
      EXPECT_EQ(invSortedArray[i], *it);
  }

  // String collection - Should result in: min[begin, pivot[ >= pivot >= max]pivot, end]
  {
    std::string str = StrRand;
    auto pivot = str.begin() + 5;
    const auto pivotVal = *pivot;

    // Run partition
    auto newPivot =
      Partition<std::string::iterator, std::greater_equal<char>>(str.begin(), pivot, str.end());
    CheckPartition<std::string::iterator>(str.begin(), str.end(), newPivot, pivotVal, false);
  }
}
