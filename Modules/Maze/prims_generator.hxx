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
#ifndef MODULE_MAZE_PRIMSG_HXX
#define MODULE_MAZE_PRIMSG_HXX

#include <DataStructures/grid.hxx>

// STD
#include <random>
#include <stack>

namespace huc
{
  namespace maze
  {
    /// Prim's Generator - Generate a maze using a Depth First Search (Prims) strategy.
    ///
    /// Prim's Maze Generator is a randomized version of Prim's algorithm:
    /// a method for producing a minimal spanning tree for a undirected weighted graph.
    ///
    /// Prim's algorithm creates a tree by getting the adjacent cells and
    /// finding the best one to travel to next.
    /// To Generate mazes using Prim's, we will instead take a random cell to travel to the next one.
    ///
    /// Although the classical Prim's algorithm keeps a list of edges,
    /// here is studied the modified version for our maze generation by maintaining a list of adjacent cells.
    /// Running faster, it still requires storage proportional to the size of the Maze.
    ///
    /// @param width desired width for the maze.
    /// @param height desired height for the maze.
    /// @param startPoint possible point to start the algorithm, {x: 0, y: 0} by default.
    /// @param seed number used to initiate the random generator, 0 by default.
    ///
    /// @return Operator() returns Maze Grid pointer to be owned, nullptr if construction failed.
    class PrimsGenerator
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
        std::mt19937 mt(seed);                   // Random generator - Mersenne Twister algorithm
        std::set<std::shared_ptr<Cell>> pathSet; // Keep track of possible paths to expand

        // While there is cell within the set:
        (*maze)[startPoint.x][startPoint.y]->info.rootDistance = 0;
        pathSet.insert((*maze)[startPoint.x][startPoint.y]);
        while (!pathSet.empty())
        {
          auto cellIt = pathSet.begin();
          std::advance(cellIt, mt() % pathSet.size());
          (*cellIt)->info.isVisited = true;

          // Randomly connect it to a cell that is already part of the mze
          auto neighbours = GetNeighbours(*maze, *(*cellIt).get(), true);
          if (!neighbours.empty())
          {
            auto randIdx = mt() % neighbours.size();
            (*cellIt)->info.rootDistance = neighbours[randIdx]->info.rootDistance + 1;
            maze->Connect(*cellIt, neighbours[randIdx]);
          }

          // Add available neighbours and remove current cell
          neighbours = GetNeighbours(*maze, *(*cellIt).get(), false);
          pathSet.insert(neighbours.begin(), neighbours.end());
          pathSet.erase(cellIt);
        }

        return maze;
      }

      /// GetNeighbours - Retrieve available neighbours
      ///
      /// @param maze Grid within the search occurs
      /// @param cell Cell of which neighbours will be found.
      /// @param visited select whether or (and only or) not cells that are visited
      ///
      /// @return vector of neighbours, empty vector if none.
      static std::vector<std::shared_ptr<Cell>>
        GetNeighbours(const Maze& maze, const Cell& cell, const bool visited)
      {
        std::vector<std::shared_ptr<Cell>> neighbour;

        const auto x = cell.x;
        const auto y = cell.y;

        // Push left if available
        if (static_cast<int>(x - 1) >= 0 &&
            ((visited) ? maze[x-1][y]->info.isVisited : !maze[x-1][y]->info.isVisited))
          neighbour.push_back(maze[x-1][y]);
        // Push bottom if available
        if (static_cast<int>(y - 1) >= 0 &&
            ((visited) ? maze[x][y-1]->info.isVisited : !maze[x][y-1]->info.isVisited))
          neighbour.push_back(maze[x][y-1]);
        // Push top if available
        if (x + 1 < maze.Width() &&
            ((visited) ? maze[x+1][y]->info.isVisited : !maze[x+1][y]->info.isVisited))
          neighbour.push_back(maze[x+1][y]);
        // Push right if available
        if (y + 1 < maze.Height() &&
            ((visited) ? maze[x][y+1]->info.isVisited : !maze[x][y+1]->info.isVisited))
          neighbour.push_back(maze[x][y+1]);

        return neighbour;
      }
    };
  }
}

#endif // MODULE_MAZE_PRIMSG_HXX
