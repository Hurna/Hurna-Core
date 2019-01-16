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
  struct CellInfoBase
  {
    CellInfoBase() : isVisited(false) {}
    bool isVisited;
  };

  /// @class Grid
  /// Wrap std::grid adding log operations writing and statisticals informations
  ///
  template <typename CellInfo = CellInfoBase>
  class Grid
  {
  public:
    explicit Grid(uint32_t width, uint32_t height, bool isConnected = false)
    { Init(width, height, isConnected); }

    struct Point
    {
      Point(uint32_t x = 0, uint32_t y = 0) : x(x), y(y) {}
      uint32_t x;
      uint32_t y;
    };

    class Cell {
    public:
      Cell(uint32_t x, uint32_t y) : x(x), y(y) {}

      const uint32_t x; // X coordinate
      const uint32_t y; // Y coordinate

      std::set<std::weak_ptr<Cell>, std::owner_less<std::weak_ptr<Cell>>>
        connectedCells;  // Directed connections from this cell
      CellInfo info;     // Use to store extra information

    private:
      Cell operator=(Cell&); // Not Implemented
    };

    class Edge {
    public:
      Edge(std::shared_ptr<Cell> first, std::shared_ptr<Cell> second) : first(first), second(second) {}

      const std::shared_ptr<Cell> first;  // First Cell
      const std::shared_ptr<Cell> second; // Second Cell

      bool operator<(const Edge& a) const
      { return this->first->x < a.first->x || this->first->y < a.first->y; }

    private:
      Edge operator=(Edge&); // Not Implemented
    };


    void Connect(const std::shared_ptr<Cell> root, const std::shared_ptr<Cell> cell)
    {
      root->connectedCells.insert(cell);
      cell->connectedCells.insert(root);
    }

    void Connect(const std::shared_ptr<Cell> cell, const std::vector<std::shared_ptr<Cell>>& neighbours)
    {
      if (neighbours.size() < 1)
        return;

      for (auto it = neighbours.begin(); it != neighbours.end(); ++it)
      {
        cell->connectedCells.insert(*it);
        (*it)->connectedCells.insert(cell);
      }
    }

    void Disconnect(const std::shared_ptr<Cell> root, const std::shared_ptr<Cell> cell)
    {
      root->connectedCells.erase(cell);
      cell->connectedCells.erase(root);
    }

    void DisconnectCol(const Point& origin, const u_int8_t idx, const u_int8_t height, const uint8_t pathIdx)
    {
      for (auto y = 0; y < height; ++y)
      {
        if (y == pathIdx)
          continue;

        this->Disconnect(this->data[origin.x + idx][origin.y + y],
                         this->data[origin.x + idx + 1][origin.y + y]);
      }
    }

    void DisconnectRow(const Point& origin, const u_int8_t idx, const u_int8_t width, const uint8_t pathIdx)
    {
      for (auto x = 0; x < width; ++x)
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
