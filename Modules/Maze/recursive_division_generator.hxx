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
#ifndef MODULE_MAZE_DIVISIONG_HXX
#define MODULE_MAZE_DIVISIONG_HXX

#include <DataStructures/grid.hxx>

// STD
#include <random>

namespace huc
{
  namespace maze
  {
    /// Recursive Division Generator - Generate a maze using a recursive division strategy.
    ///
    /// Recursive Division Maze Generator is the fastest algorithm without directional biases.
    /// While Recursive division really stands out with respect to parallelism,
    /// this algorithm is particularly fascinating because of its fractal nature:
    /// you could theoretically continue the process indefinitely at
    /// finer and finer levels of detail (smaller and smaller scales).
    ///
    /// This algorithm is somewhat similar to recursive backtracking, since they're both stack based,
    /// except this focuses on walls instead of passages. As a Wall Builders generator,
    /// the process begins with a large empty space (all cells are connected),
    /// and adds walls (disconnect cells) until a maze results.
    ///
    /// @param width desired width for the maze.
    /// @param height desired height for the maze.
    /// @param seed number used to initiate the random generator.
    ///
    /// @return Operator() returns Maze Grid pointer to be owned, nullptr if construction failed.
    class RecursiveDivisionGenerator
    {
    public:
      typedef Grid<CellInfoBase> Maze;
      typedef Maze::Cell Cell;
      typedef Maze::Point Point;

      std::unique_ptr<Maze> operator()(const uint32_t width, const uint32_t height, const uint32_t seed = 0)
      {
        if (width < 1 || height < 1)
          return nullptr;

        auto maze(std::unique_ptr<Maze>(new Maze(width, height, true)));
        std::mt19937 mt(seed);  // Initialize random generator (Mersenne Twister)

        Compute(mt, *maze, Point(0,0), width, height);

        return maze;
      }

    private:
      static void Compute
        (std::mt19937& mt, Maze& maze, const Point& origin, const uint32_t width, uint32_t height)
      {
        if (width < 2 || height < 2)
          return;

        // Build a wall within the room, whether vertical or horizontal,
        // Open a gate at a random position
        bool isHorizontalCut = (mt() % 2 == 0) ? true : false; //(width <= height);
        uint32_t wallIdx = (isHorizontalCut) ? mt() % (height - 1) : mt() % (width - 1);
        uint32_t pathIdx = (isHorizontalCut) ? mt() % width : mt() % height;

        // Build wall and Recurse on sub areas (Top and Bottom)
        if (isHorizontalCut)
        {
          maze.DisconnectRow(origin, wallIdx, width, pathIdx);
          Compute(mt, maze, origin, width, wallIdx + 1);
          Compute(mt, maze, Point(origin.x, origin.y + wallIdx + 1),  width, height - wallIdx - 1);
        }
        // Recurse on sub areas (Left and Right)
        else
        {
          maze.DisconnectCol(origin, wallIdx, height, pathIdx);
          Compute(mt, maze, origin, wallIdx + 1, height);
          Compute(mt, maze, Point(origin.x + wallIdx + 1, origin.y),  width - wallIdx - 1, height);
        }
      }
    };
  }
}

#endif // MODULE_MAZE_DIVISIONG_HXX
