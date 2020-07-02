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

#ifndef VIIIARCHIVE_JUNCTIONABILITIES_H
#define VIIIARCHIVE_JUNCTIONABILITIES_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include <cstring>
#include "JunctionFlagsT.h"
namespace OpenVIII::Kernel {
struct JunctionAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Junction-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	J-Flag
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t apRequired_{};
  std::array<std::uint8_t, 3> junctionFlags_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto APRequired() const noexcept { return apRequired_; }
  [[nodiscard]] auto JunctionFlags() const
  {
    JunctionFlagsT out{};
    std::memcpy(&out, junctionFlags_.data(), junctionFlags_.size());
    return out;
  }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os << ", " << static_cast<uint32_t>(APRequired()) << ", " << static_cast<uint32_t>(JunctionFlags());
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_JUNCTIONABILITIES_H