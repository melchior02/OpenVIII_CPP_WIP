// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIIIARCHIVE_POINT_H
#define VIIIARCHIVE_POINT_H
#include <iostream>
#include <concepts>
namespace open_viii::graphics {
template<typename dimT> requires(std::integral<dimT> || std::floating_point<dimT>) struct Point
{
private:
  mutable dimT m_x{};
  mutable dimT m_y{};

public:
  Point() = default;
  Point(const dimT &in_x, const dimT &in_y) noexcept : m_x(in_x), m_y(in_y){};
  friend auto operator==(const Point<dimT> &left, const Point<dimT> &right) noexcept
  {
    return left.m_x == right.m_x && left.m_y == right.m_y;
  }
  friend auto operator<(const Point<dimT> &left, const Point<dimT> &right) noexcept
  {
    return left.m_x < right.m_x && left.m_y < right.m_y;
  }
  friend auto operator>(const Point<dimT> &left, const Point<dimT> &right) noexcept
  {
    return left.m_x > right.m_x && left.m_y > right.m_y;
  }
  [[nodiscard]] auto abs()
  {
    if constexpr (std::signed_integral<dimT> || std::floating_point<dimT>) {
      return Point<dimT>(static_cast<dimT>(std::abs(m_x)), static_cast<dimT>(std::abs(m_y)));
    } else {
      return *this;
    }
  }
  /**
   * X coordinate.
   * @return x
   */
  [[nodiscard]] const auto &x() const noexcept { return m_x; }

  /**
   * Y coordinate.
   * @return y
   */
  [[nodiscard]] const auto &y() const noexcept { return m_y; }

  /**
   *
   * @param x is new X coordinate.
   * @return x
   */
  [[nodiscard]] const auto &x(const dimT &x) const noexcept { return m_x = x; }

  /**
   *
   * @param y is new Y coordinate.
   * @return y
   */
  [[nodiscard]] const auto &y(const dimT &y) const noexcept { return m_y = y; }


  friend std::ostream &operator<<(std::ostream &os, const Point<dimT> &input)
  {
    return os << '{' << input.m_x << ", " << input.m_y << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_POINT_H