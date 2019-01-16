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
#ifndef MODULE_MAZE_DFSG_HXX
#define MODULE_MAZE_DFSG_HXX

#include <DataStructures/grid.hxx>

// STD
#include <random>
#include <stack>

namespace huc
{
  namespace maze
  {
    /// Depth First Search Generator - Generate a maze using a Depth First Search (DFS) strategy.
    ///
    /// Depth First Search (DFS) Maze Generator is a randomized version of the depth-first search traversal
    /// algorithm.
    /// Implemented with a stack, this approach is one of the simplest ways to generate a maze.
    ///
    /// @param width desired width for the maze.
    /// @param height desired height for the maze.
    /// @param startPoint possible point to start the algorithm, {x: 0, y: 0} by default.
    /// @param seed number used to initiate the random generator, 0 by default.
    ///
    /// @return Operator() returns Maze Grid pointer to be owned, nullptr if construction failed.
    class DFSGenerator
    {
    public:
      struct CellInfo : CellInfoBase
      { uint64_t rootDistance; };

      typedef Grid<CellInfo> Maze;
      typedef Maze::Cell Cell;
      typedef Maze::Point Point;

      std::unique_ptr<Maze> operator()(const uint32_t width, const uint32_t height,
                                       const Point& startPoint = Point(0,0), const uint32_t seed = 0)
      {
        if (width < 1 || height < 1 || startPoint.x >= width || startPoint.y >= height)
          return nullptr;

        auto maze(std::unique_ptr<Maze>(new Maze(width, height)));
        std::mt19937 mt(seed);                       // Random generator - Mersenne Twister algorithm
        std::stack<std::shared_ptr<Cell>> pathStack; // Keep track of the cell path

        // Create a stack to keep track of the path and add the starting cell within.
        (*maze)[startPoint.x][startPoint.y]->info.rootDistance = 0;
        (*maze)[startPoint.x][startPoint.y]->info.isVisited = true;
        pathStack.push((*maze)[startPoint.x][startPoint.y]);

        // While there is cell within the stack
        while (!pathStack.empty())
        {
          // Handle the Cell at the top of the stack
          auto cell = pathStack.top();
          pathStack.pop();

          // Get available neighbours and connect to them
          auto neighbours = GetNeighbours(*maze, *cell.get());

          // Add them to the stack and select one to be processed next
          if (!neighbours.empty())
          {
            // Randomly select a cell to be processed
            auto randIdx = mt() % neighbours.size();

            // For each available cell we make a connection and push it into the stack of being processed
            // Only the choosen item should be add to the top following a DFS strategy
            for (uint8_t i = 0; i < neighbours.size(); ++i)
            {
              neighbours[i]->info.isVisited = true;
              neighbours[i]->info.rootDistance = cell->info.rootDistance + 1;

              if (i != randIdx) pathStack.push(neighbours[i]);
            }
            pathStack.push(neighbours[randIdx]);

            // Connect cell with all neighbours
            maze->Connect(cell, neighbours);
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
      /// @return vector of neighbours that has not been visited yet, empty vector if none.
      static std::vector<std::shared_ptr<Cell>> GetNeighbours(const Maze& maze, const Cell& cell)
      {
        std::vector<std::shared_ptr<Cell>> neighbours;
        neighbours.reserve(4);

        const auto x = cell.x;
        const auto y = cell.y;

        // Push left if available
        if (static_cast<int>(x - 1) >= 0 && !maze[x-1][y]->info.isVisited)
          neighbours.push_back(maze[x-1][y]);
        // Push bottom if available
        if (static_cast<int>(y - 1) >= 0 && !maze[x][y-1]->info.isVisited)
          neighbours.push_back(maze[x][y-1]);
        // Push top if available
        if (x + 1 < maze.Width() && !maze[x+1][y]->info.isVisited)
          neighbours.push_back(maze[x+1][y]);
        // Push right if available
        if (y + 1 < maze.Height() && !maze[x][y+1]->info.isVisited)
          neighbours.push_back(maze[x][y+1]);

        return neighbours;
      }
    };
  }
}

#endif // MODULE_MAZE_DFSG_HXX
