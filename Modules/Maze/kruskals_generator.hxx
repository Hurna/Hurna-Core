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
#ifndef MODULE_MAZE_KRUSKALSG_HXX
#define MODULE_MAZE_KRUSKALSG_HXX

#include <DataStructures/grid.hxx>

// STD
#include <random>

namespace huc
{
  namespace maze
  {
    /// Kruskal's Generator - Generate a maze using a kruskal's strategy.
    ///
    /// Kruskal's Maze Generator is a randomized version of Kruskal’s algorithm:
    /// a method for producing a minimal spanning tree for a weighted graph.
    ///
    /// Kruskal's is interesting because it doesn't "grow" the Maze like a tree,
    /// but rather carves passage segments all over the Maze at random, making it very fun to watch.
    /// Still, it results in a perfect Maze in the end.
    ///
    /// The counterpart is to require storage proportional to the size of the Maze,
    /// along with the ability to enumerate each edge between cells in random order
    /// (Using here a set of edges and taking them randomly).
    ///
    /// @param width desired width for the maze.
    /// @param height desired height for the maze.
    /// @param seed number used to initiate the random generator.
    ///
    /// @return Operator() returns Maze Grid pointer to be owned, nullptr if construction failed.
    class KruskalsGenerator
    {
    public:
      struct CellInfoBucket : CellInfoBase
      { uint64_t bucketId; };  // Precision should be above uint32_t * uint32_t - Limit on maze

      typedef Grid<CellInfoBucket> Maze;
      typedef Maze::Cell Cell;
      typedef Maze::Edge Edge;
      typedef Maze::Point Point;

      std::unique_ptr<Maze> operator()(const uint32_t width, const uint32_t height, const uint32_t seed = 0)
      {
        if (width < 1 || height < 1)
          return nullptr;

        auto maze(std::unique_ptr<Maze>(new Maze(width, height)));
        std::mt19937 mt(seed);   // Random generator - Mersenne Twister algorithm
        std::set<Edge> edges;    // Big sac with all possible edges
        std::vector<std::vector<std::shared_ptr<Cell>>> bucketCells; // List of all buckets

        // Fill big sac of edges and set the bucket ids to each cell
        uint64_t nodeId = 0;
        bucketCells.resize(height * width); // (height - 1) * width + (width - 1) * height);
        for (uint32_t x = 0; x < maze->Width(); ++x)
          for (uint32_t y = 0; y < maze->Height(); ++y, ++nodeId)
          {
            (*maze)[x][y]->info.bucketId = nodeId;
            bucketCells[nodeId].push_back((*maze)[x][y]);

            // Insert Right edge
            if (x + 1 < maze->Width())
              edges.insert(Edge((*maze)[x][y], (*maze)[x+1][y]));
            // Insert Bottom edge
            if (y + 1 < maze->Height())
              edges.insert(Edge((*maze)[x][y], (*maze)[x][y+1]));
          }

        // While the set of edges is not empty randomly get an edge (connecting two cells):"
        while (!edges.empty())
        {
          auto edgeIt = edges.begin();
          std::advance(edgeIt, mt() % edges.size());
          const auto firstBucket = edgeIt->first->info.bucketId;
          const auto secondBucket = edgeIt->second->info.bucketId;

          // Add connection and merge buckets if not already in the same one
          if (firstBucket != secondBucket)
          {
            maze->Connect(edgeIt->first, edgeIt->second);
            MergeBucket(bucketCells, firstBucket, secondBucket);
          }

          // Remove computed cell from the set
          edges.erase(edgeIt);
        }

        return maze;
      }

    private:
      /// MergeBucket - Merge two buckets of node together and update node bucket Id.
      ///
      /// @param buckets vector containing all the buckets
      /// @param fromId bucket id that will be merged within the other bucket
      /// @param ToId bucket id that will receive the other bucket cells
      ///
      /// @return void.
      static void MergeBucket(std::vector<std::vector<std::shared_ptr<Cell>>>& buckets,
                              uint32_t fromId, uint32_t ToId)
      {
        if (fromId == ToId)
          return;

        // Set new bucket id for each melding cell
        for (auto it = buckets[fromId].begin(); it != buckets[fromId].end(); ++it)
        {
          (*it)->info.bucketId = ToId;
          buckets[ToId].push_back(*it);
        }

        buckets[fromId].clear();
      }
    };
  }
}

#endif // MODULE_MAZE_KRUSKALSG_HXX
