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

#ifndef VIIIARCHIVE_DEVOUR_H
#define VIIIARCHIVE_DEVOUR_H
#include "OpenVIII/Strings/EncodedStringOffset.h"
#include "BattleOnlyStatusesT.h"
#include "PersistentStatusesT.h"
namespace open_viii::kernel {
enum class PercentQuantityT : std::uint8_t {
  // 0% = 0x00
  none [[maybe_unused]] = 0x00,

  // 6.25% = 0x01,
  sixteenth = 0x01,

  // 12.50% = 0x02
  eighth = 0x02,

  // 25% = 0x04
  quarter = 0x04,

  // 50% = 0x08
  half = 0x08,

  // 100% = 0x10
  full = 0x10,
};
enum class DevourStatFlagT : std::uint8_t {
  none = 0x00,
  str = 0x01,
  vit [[maybe_unused]] = 0x02,
  mag = 0x04,
  spr = 0x08,
  spd = 0x10,
};
template<LangT langVal> struct Devour
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Devour
   * Offset	Length	Description
   * 0x0000	2 bytes	Offset to devour description
   * 0x0002	1 byte	Damage or heal HP and Status
   *   0x1E - Cure
   *   0x1F - Damage
   * 0x0003	1 byte	HP Heal/DMG Quantity Flag
   *   0x00 - 0%
   *   0x01 - 6.25%
   *   0x02 - 12.50%
   *   0x04 - 25%
   *   0x08 - 50%
   *   0x10 - 100%
   * 0x0004	4 bytes	status_1; //statuses 8-39
   * 0x0008	2 bytes	status_0; //statuses 0-7
   * 0x000A	1 byte	Raised Stat Flag
   *   0x00 - None
   *   0x01 - STR
   *   0x02 - VIT
   *   0x04 - MAG
   *   0x08 - SPR
   *   0x10 - SPD
   * 0x000B	1 byte	Raised Stat HP Quantity
   * */
private:
  EncodedStringOffset m_description_offset{};
  std::uint8_t m_damage_or_heal{};// HP and Status; If last on right bit is set heal, and if not dmg. Rest looks like
                                  // 0b‭00011110‬.
  PercentQuantityT m_percent_quantity{};
  BattleOnlyStatusesT m_battle_only_statuses{};// statuses 8-39
  PersistentStatusesT m_persistent_statuses{};// statuses 0-7
  DevourStatFlagT m_devour_stat_flag{};
  std::uint8_t m_raised_stat_hp_quantity{};

public:
  [[nodiscard]] auto &description_offset() const noexcept { return m_description_offset; }
  [[nodiscard]] auto damage_or_heal() const noexcept
  {
    return (m_damage_or_heal & 0x1U) == 0;
  }// HP and Status //false is damage, true is heal.
  [[nodiscard]] auto percent_quantity() const noexcept// clang tidy thinks this can be made static...
  {
    const auto flag_set = [this](const PercentQuantityT &flag) {
      return (static_cast<uint8_t>(m_percent_quantity) & static_cast<uint8_t>(flag)) != 0;
    };
    float out{};
    if (flag_set(PercentQuantityT::full)) {
      static constexpr auto full = 1.0F;
      out += full;
    }
    if (flag_set(PercentQuantityT::half)) {
      static constexpr auto half = 1.0F / 2.0F;
      out += half;
    }
    if (flag_set(PercentQuantityT::quarter)) {
      static constexpr auto quarter = 1.0F / 4.0F;
      out += quarter;
    }
    if (flag_set(PercentQuantityT::eighth)) {
      static constexpr auto eighth = 1.0F / 8.0F;
      out += eighth;
    }
    if (flag_set(PercentQuantityT::sixteenth)) {
      static constexpr auto sixteenth = 1.0F / 16.0F;
      out += sixteenth;
    }
    return out;
  }
  [[nodiscard]] auto battle_only_statuses() const noexcept { return m_battle_only_statuses; }// statuses 8-39
  [[nodiscard]] auto persistent_statuses() const noexcept { return m_persistent_statuses; }// statuses 0-7
  [[maybe_unused]] [[nodiscard]] auto devour_stat_flag() const noexcept { return m_devour_stat_flag; }
  [[maybe_unused]] [[nodiscard]] auto raised_stat_hp_quantity() const noexcept { return m_raised_stat_hp_quantity; }
  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(description)) {
      os << Tools::u8_to_sv(description);
    }
    return os << ", " << static_cast<std::uint32_t>(damage_or_heal()) << ", " << percent_quantity() << ", "
              << static_cast<std::uint32_t>(m_battle_only_statuses) << ", "
              << static_cast<std::uint32_t>(m_persistent_statuses) << ", "
              << static_cast<std::uint32_t>(m_devour_stat_flag) << ", "
              << static_cast<std::uint32_t>(m_raised_stat_hp_quantity);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_DEVOUR_H