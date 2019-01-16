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
#include <merge.hxx>

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
  typedef MergeWithBuffer<std::string::iterator> Aggregator_Str;

  const Container ArraySort = {-3, -2, 0, 2, 8, 15, 36, 212, 366};        // Sorted with neg values
  const Container ArraySortWithRot = {-3, 2, 7, 20, 0, 2, 8, 15, 36};     // Sorted with a rotation
  const Container ArraySortU = {0, 2, 8, 15, 36, 212, 366, 15478};        // Sorted positive values
  const Container ArrayRand = {4, 3, 5, 2, -18, 3, 2, 3, 4, 5, -5};       // Random with neg values
  const Container ArrayRandU = {4520, 30, 500, 20, 3, 2, 3, 4, 5, 15};    // Random positive values

  const std::string StrRand = "xacvgeze";
  // String with pivot at end =  begin() + 4 : Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]
  const std::string StrRandPivot = "eknxasuw";
}
#endif /* DOXYGEN_SKIP */

// Basic MergeInPlace tests
TEST(TestMerge, MergeInPlaces)
{
  // Normal Run - All elements should be sorted in order
  {
    Container vector(ArraySortWithRot);
    MergeInPlace<IT>()(vector.begin(), vector.begin() + 4, vector.end());

    // All elements of the final array are sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already SortArray - Array should not be affected
  {
    Container vector(ArraySortU);
    MergeInPlace<IT>()(vector.begin(), vector.begin() + 5, vector.end());

    // All elements are still sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container vector(ArrayRandU);
    MergeInPlace<IT>()(vector.end(), vector.begin() + 3, vector.begin());

    int i = 0;
    for (auto it = vector.begin(); it < vector.end(); ++it, ++i)
      EXPECT_EQ(ArrayRandU[i], *it);
  }

  // No error empty array
  {
    Container emptyArray;
    MergeInPlace<IT>()(emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    MergeInPlace<IT>()(uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container doubleValues(1, 511);
    doubleValues.push_back(66);

    MergeInPlace<IT>()(doubleValues.begin(), doubleValues.begin() + 1, doubleValues.end());

    EXPECT_EQ(66, doubleValues[0]);
    EXPECT_EQ(511, doubleValues[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string str = StrRandPivot;
    MergeInPlace<std::string::iterator>()(str.begin(), str.begin() + 4, str.end());

    // All elements of the final array are sorted
    for (auto it = str.begin(); it < str.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}


// Basic MergeWithBuffer tests
TEST(TestMerge, MergeWithBuffers)
{
  // Normal Run - All elements should be sorted in order
  {
    Container vector(ArraySortWithRot);
    MergeWithBuffer<IT>()(vector.begin(), vector.begin() + 4, vector.end());

    // All elements of the final array are sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already SortArray - Array should not be affected
  {
    Container vector(ArraySortU);
    MergeWithBuffer<IT>()(vector.begin(), vector.begin() + 5, vector.end());

    // All elements are still sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container vector(ArrayRandU);
    MergeWithBuffer<IT>()(vector.end(), vector.begin() + 3, vector.begin());

    int i = 0;
    for (auto it = vector.begin(); it < vector.end(); ++it, ++i)
      EXPECT_EQ(ArrayRandU[i], *it);
  }

  // No error empty array
  {
    Container emptyArray;
    MergeWithBuffer<IT>()(emptyArray.begin(), emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    MergeWithBuffer<IT>()(uniqueValueArray.begin(), uniqueValueArray.end(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // Double values array - Order should be made
  {
    Container doubleValuesArray(1, 511);
    doubleValuesArray.push_back(66);

    MergeWithBuffer<IT>()(doubleValuesArray.begin(), doubleValuesArray.begin() + 1, doubleValuesArray.end());

    EXPECT_EQ(66, doubleValuesArray[0]);
    EXPECT_EQ(511, doubleValuesArray[1]);
  }

  // String Collection - All elements should be sorted in order
  {
    std::string stringToMerge = "eknxasuw"; // Left sorted part [e,k,n,x] - Right sorted part [a,s,u,w]
    MergeWithBuffer<std::string::iterator>()
      (stringToMerge.begin(), stringToMerge.begin() + 4, stringToMerge.end());

    // All elements of the final array are sorted
    for (std::string::iterator it = stringToMerge.begin(); it < stringToMerge.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}

// Basic Merge-Sort tests - Uses the merge with buffer (computation optimized)
TEST(TestMerge, MergeSorts)
{
  // Normal Run - all elements should be sorter in order
  {
    Container vector(ArrayRand);
    MergeSort<IT>(vector.begin(), vector.end());

    // All elements are sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Already SortArray - Array should not be affected
  {
    Container vector(ArraySort);
    MergeSort<IT>(vector.begin(), vector.end());

    // All elements are still sorted
    for (auto it = vector.begin(); it < vector.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }

  // Inverse iterator order - Array should not be affected
  {
    Container vector(ArrayRand);
    MergeSort<IT>(vector.end(), vector.begin());

    int i = 0;
    for (auto it = vector.begin(); it < vector.end(); ++it, ++i)
      EXPECT_EQ(ArrayRand[i], *it);
  }

  // No error empty array
  {
    Container emptyArray;
    MergeSort<IT>(emptyArray.begin(), emptyArray.end());
  }

  // Unique value array - Array should not be affected
  {
    Container uniqueValueArray(1, 511);
    MergeSort<IT>(uniqueValueArray.begin(), uniqueValueArray.end());
    EXPECT_EQ(511, uniqueValueArray[0]);
  }

  // String collection - all elements should be sorter in order
  {
    std::string str = StrRand;
    MergeSort<std::string::iterator, Aggregator_Str>(str.begin(), str.end());

    // All elements are sorted
    for (auto it = str.begin(); it < str.end() - 1; ++it)
      EXPECT_LE(*it, *(it + 1));
  }
}
