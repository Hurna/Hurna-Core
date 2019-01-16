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
#ifndef MODULE_MAZE_BTG_HXX
#define MODULE_MAZE_BTG_HXX

#include <DataStructures/grid.hxx>

// STD
#include <random>

namespace huc
{
  namespace maze
  {
    /// Sidewinder Generator - Generate a maze using a sidewinder strategy.
    ///
    /// Sidewinder Maze Generator is very similar to the Binary Tree algorithm,
    /// and only slightly more complicated.
    /// Furthermore, the Sidewinder algorithm only needs to consider the current row,
    /// and therefore can be used to generate infinitely large mazes (like the Binary Tree).
    ///
    /// While binary tree mazes have two of its four sides being one long passage,
    /// a Sidewinder mazes have just one long passage.
    ///
    /// @param width desired width for the maze.
    /// @param height desired height for the maze.
    /// @param seed number used to initiate the random generator.
    ///
    /// @return Operator() returns Maze Grid pointer to be owned, nullptr if construction failed.
    class SidewinderGenerator
    {
    public:
      typedef Grid<CellInfoBase> Maze;
      typedef Maze::Cell Cell;

      std::unique_ptr<Maze> operator()(const uint32_t width, const uint32_t height, const uint32_t seed = 0)
      {
        if (width < 1 || height < 1)
          return nullptr;

        auto maze(std::unique_ptr<Maze>(new Maze(width, height)));
        std::mt19937 mt(seed);                   // Random generator based on Mersenne Twister algorithm
        std::set<std::shared_ptr<Cell>> runSet;  // Keep track of possible paths to expand

        // Initialize an empty “run” set to keep track of the current line path.
        // Scan grid line by line starting with the cell[0,0]:
        for (uint32_t y = 0; y < height; ++y)
        {
          for (uint32_t x = 0; x < width; ++x)
          {
            // Add current cell to the path (avoid useless insertion)
            if (y > 0)
              runSet.insert((*maze)[x][y]);

            // Randomly carve to east or north
            // If a passage was carved, continue line scan.
            // First row can only be a single passage, crave it to the east
            if (x + 1 < width && (mt() % 2 == 0 || y == 0))
            {
              maze->Connect((*maze)[x][y], (*maze)[x+1][y]);
            }
            // Otherwise randomly choose a cell in the run set and carve north
            else if (y > 0)
            {
              auto cellIt = runSet.begin();
              std::advance(cellIt, mt() % runSet.size());

              maze->Connect(*cellIt, (*maze)[(*cellIt)->x][(*cellIt)->y - 1]);
              runSet.clear(); // Empty the run set and continue line scan
            }
          }

          runSet.clear();
        }

        return maze;
      }
    };
  }
}

#endif // MODULE_MAZE_BTG_HXX
