/*===========================================================================================================
 *
 * HUL - Hurna Lib
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/Hurna/Hurna-Lib/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#ifndef MODULE_DS_GRID_HXX
#define MODULE_DS_GRID_HXX

// STD
#include <memory>
#include <vector>
#include <set>

namespace huc
{
  /// CellInfoBase - Used to store extra information associated with a Grid::Cell.
  /// It is used as template argument for the Grid class.
  struct CellInfoBase
  {
    CellInfoBase() : isVisited(false) {}
    bool isVisited;
  };

  /// Grid - Conveniently wraps a double std::vector<Grid::Cell> and add cell connection managment.
  /// The grid class may be used to generate all kind of networks/graphs/mazes with uniform and othonormal
  /// cell positionning.
  ///
  /// @tparam CellInfo a struct used to store extra information associated with each Grid::Cell
  template <typename CellInfo = CellInfoBase>
  class Grid
  {
  public:
    /// Grid constructor.
    ///
    /// @param width the desired width for the Grid.
    /// @param height the desired height for the Grid.
    /// @param isConnected whether or not the cells of the grid are connected at the initialization.
    explicit Grid(uint32_t width, uint32_t height, bool isConnected = false)
    { Init(width, height, isConnected); }

    /// Point struct is a convenient struct use to represent a 2D point for a grid (index position).
    struct Point
    {
      Point(uint32_t x = 0, uint32_t y = 0) : x(x), y(y) {}
      uint32_t x;
      uint32_t y;
    };

    /// Cell struct is a convenient struct use to represent a 2D cell for a grid (index position).
    /// It handles a set of its connection to other nodes and extra information passed as template parameter.
    class Cell {
    public:
      Cell(uint32_t x, uint32_t y) : x(x), y(y) {}

      const uint32_t x; // X coordinate
      const uint32_t y; // Y coordinate

      std::set<std::weak_ptr<Cell>, std::owner_less<std::weak_ptr<Cell>>>
        connectedCells;  // Direct connections from this cell (!do not use shared_ptr to avoid cycle!)
      CellInfo info;     // Use to store extra information

    private:
      Cell operator=(Cell&); // Not Implemented
    };

    /// Edge struct is a convenient struct use to store two cell pointers connected by a link/edge.
    class Edge {
    public:
      Edge(std::shared_ptr<Cell> first, std::shared_ptr<Cell> second) : first(first), second(second) {}

      const std::shared_ptr<Cell> first;  // First Cell
      const std::shared_ptr<Cell> second; // Second Cell

      bool operator<(const Edge& a) const
      { return this->first->x < a.first->x || this->first->y < a.first->y ||
               this->second->x < a.second->x || this->second->y < a.second->y; }

    private:
      Edge operator=(Edge&); // Not Implemented
    };

    /// Connect with a bidirectionnal link first and second.
    ///
    /// @param first the cell to be connected to the second cell.
    /// @param second the cell to be connected to the first cell.
    ///
    /// @return void.
    void Connect(const std::shared_ptr<Cell> first, const std::shared_ptr<Cell> second)
    {
      first->connectedCells.insert(second);
      second->connectedCells.insert(first);
    }

    /// Connect with bidirectionnal links the root with the neighbours.
    ///
    /// @param root the root to be connected to the neighbours.
    /// @param neighbours list of cells to be connected to the root.
    ///
    /// @return void.
    void Connect(const std::shared_ptr<Cell> root, const std::vector<std::shared_ptr<Cell>>& neighbours)
    {
      if (neighbours.size() < 1)
        return;

      for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
      {
        root->connectedCells.insert(*it);
        (*it)->connectedCells.insert(root);
      }
    }

    /// Disconnect the bidirectionnal link between first and second.
    ///
    /// @param first the cell to be disconnected to the second cell.
    /// @param second the cell to be disconnected to the first cell.
    ///
    /// @return void.
    void Disconnect(const std::shared_ptr<Cell> first, const std::shared_ptr<Cell> second)
    {
      first->connectedCells.erase(second);
      second->connectedCells.erase(first);
    }

    /// Disconnect a column of cells - Build a wall between connected cells.
    /// Each cell of this column is disconnected to its right (East) neighboor except the one at the pathIdx.
    /// If the cells does not have a east neighboor connected : nothing append.
    ///
    /// @param origin the origin point on the grid setting the current relative position.
    /// @param idx the relative index of the column of cells to be disconnected.
    /// @param height the size of wall constructed. If it exceeds the size of the Grid: wall will be
    /// construted until the border.
    /// @param pathIdx the relative index of the path (door) within the wall.
    /// No path is created if pathIdx >= height.
    ///
    /// @return void.
    void DisconnectCol(const Point& origin, const uint32_t idx, const uint32_t height, const uint32_t pathIdx)
    {
      if (origin.x + idx >= this->Width() - 1 || origin.y >= this->Height())
        return;

      for (uint32_t y = 0; y < std::min(height, this->Height() - origin.y); ++y)
      {
        if (y == pathIdx)
          continue;

        this->Disconnect(this->data[origin.x + idx][origin.y + y],
                         this->data[origin.x + idx + 1][origin.y + y]);
      }
    }

    /// Disconnect a row of cells - Build a wall between connected cells.
    /// Each cell of this row is disconnected to its right (East) neighboor except the one at the pathIdx.
    /// If the cells does not have a east neighboor connected : nothing append.
    ///
    /// @param origin the origin point on the grid setting the current relative position.
    /// @param idx the relative index of the row of cells to be disconnected.
    /// @param width the size of wall constructed. If it exceeds the size of the Grid: wall will be
    /// construted until the border.
    /// @param pathIdx the relative index of the path (door) within the wall.
    /// No path is created if pathIdx >= width.
    ///
    /// @return void.
    void DisconnectRow(const Point& origin, const uint32_t idx, const uint32_t width, const uint32_t pathIdx)
    {
      if (origin.y + idx >= this->Height() - 1 || origin.x >= this->Width())
        return;

      for (uint32_t x = 0; x < std::min(width, this->Width() - origin.x); ++x)
      {
        if (x == pathIdx)
          continue;

        this->Disconnect(this->data[origin.x + x][origin.y + idx],
                         this->data[origin.x + x][origin.y + idx + 1]);
      }
    }

    uint32_t Width() const { return data.size(); }
    uint32_t Height() const { return (data.size() > 0) ? data[0].size() : 0; }

    // Accessors
    std::vector<std::shared_ptr<Cell>>& operator[] (size_t n) { return this->data[n]; }
    const std::vector<std::shared_ptr<Cell>>& operator[] (size_t n) const { return this->data[n]; }

  private:
    Grid operator=(Grid&);                                // Not Implemented
    std::vector<std::vector<std::shared_ptr<Cell>>> data; // Grid wrapper

    void Init(uint32_t width, uint32_t height, bool isConneccted)
    {
      // Generate the Grid
      this->data.resize(width);
      for (uint32_t x = 0; x < width; ++x)
      {
        this->data[x].reserve(height);
        for (uint32_t y = 0; y < height; ++y)
        {
          this->data[x].push_back(std::shared_ptr<Cell>(new Cell(x, y)));

          // Connect West
          if (isConneccted && x > 0)
            this->Connect(this->data[x][y], this->data[x-1][y]);
          // Connect North
          if (isConneccted && y > 0)
            this->Connect(this->data[x][y], this->data[x][y-1]);
        }
      }
    }
  };
}
#endif // MODULE_DS_GRID_HXX
