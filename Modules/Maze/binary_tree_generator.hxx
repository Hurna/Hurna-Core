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
    /// Binary Tree Generator - Generate a maze using a binary tree strategy.
    ///
    /// Binary Tree Maze Generator is one of the very rareful algorithms with the ability to
    /// generate a perfect maze without keeping any state at all:
    /// it is a true memoryless Maze generation algorithm with no limit to the size of Maze you can create.
    /// It can build the entire maze by looking at each cell independantly.
    /// This is basically the simplest and fastest algorithm possible.
    ///
    /// Mazes generated are real Binary Tree Data Structure, while having a very biased texture.
    ///
    /// @param width desired width for the maze.
    /// @param height desired height for the maze.
    /// @param seed number used to initiate the random generator.
    ///
    /// @return Operator() returns Maze Grid pointer to be owned, nullptr if construction failed.
    class BinaryTreeGenerator
    {
    public:
      typedef Grid<CellInfoBase> Maze;
      typedef Maze::Cell Cell;

      std::unique_ptr<Maze> operator()(const uint32_t width, const uint32_t height, const uint32_t seed = 0)
      {
        if (width < 1 || height < 1)
          return nullptr;

        auto maze(std::unique_ptr<Maze>(new Maze(width, height)));
        std::mt19937 mt(seed); // Initialize random generator based on Mersenne Twister algorithm

        // For each existing cell in the grid, randomly carve a passage either east or south
        for (uint32_t y = 0; y < height; ++y)
        {
          for (uint32_t x = 0; x < width; ++x)
          {
            // Get available neighbours
            auto cell = (*maze)[x][y];
            auto curNeighbours = GetNeighbours(*maze, *cell.get());

            if (curNeighbours.empty())
              continue;

            // Randomly select a node to be processed
            auto randIdx = mt() % curNeighbours.size();
            maze->Connect(cell, curNeighbours[randIdx]);
          }
        }

        return maze;
      }

    private:
      /// GetNeighbours - Retrieve available neighbours
      ///
      /// @param maze Grid within the search occurs
      /// @param cell Cell of which neighbours will be found.
      ///
      /// @return vector of the available top and left neighbours, empty vector if none.
      static std::vector<std::shared_ptr<Cell>> GetNeighbours(const Maze& maze, const Cell& cell)
      {
        std::vector<std::shared_ptr<Cell>> neighbours;

        const auto x = cell.x;
        const auto y = cell.y;

        if (x > 0) neighbours.push_back(maze[x-1][y]);  // West Direction - Push left if available
        if (y > 0) neighbours.push_back(maze[x][y-1]);  // North Direction - Push top if available

        return neighbours;
       }
    };
  }
}

#endif // MODULE_MAZE_BTG_HXX
