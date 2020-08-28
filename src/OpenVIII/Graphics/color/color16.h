//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_COLOR16_H
#define VIIIARCHIVE_COLOR16_H

#include <cstdint>
#include <bitset>
#include "OpenVIII/concepts.h"
#include "OpenVIII/Tools/Tools.h"
namespace OpenVIII::Graphics {
/**
 *
 * @see https://github.com/myst6re/deling/blob/master/FF8Image.cpp#L30
 */
struct color16
{
private:
  static constexpr auto bits = 16U;
  mutable uint16_t value{};
  static constexpr std::bitset<bits> BlueMask{ 0x7C00 };
  static constexpr std::bitset<bits> GreenMask{ 0x3E0 };
  static constexpr std::bitset<bits> RedMask{ 0x1F };
  [[maybe_unused]] static constexpr std::bitset<bits> STPMask{ 0x8000U };
  static constexpr std::bitset<bits> AllColorMask{ 0x7FFF };
  static constexpr std::uint_fast8_t BlueShift{ 10U };
  static constexpr std::uint_fast8_t GreenShift{ 5U };
  static constexpr std::uint_fast8_t RedShift = { 0U };
  [[maybe_unused]] static constexpr std::uint_fast8_t STPShift = { 15U };
  static constexpr std::uint_fast8_t ConvertShift = { 3U };
  static constexpr std::uint_fast8_t GetHighBitShift = { 2U };
  static constexpr std::uint_fast8_t largest5BitValue{ 0b0001'1111 };
  [[nodiscard]] std::bitset<bits> valueBit() const { return std::bitset<bits>{ value }; }
  void valueBit(const std::bitset<bits> &newValue) const { value = static_cast<std::uint16_t>(newValue.to_ulong()); }
  [[nodiscard]] std::uint8_t convert(const std::bitset<bits> &mask, const std::uint_fast8_t &shift) const
  {
    auto temp = ((valueBit() & mask) >> shift);
    return static_cast<std::uint8_t>(((temp << ConvertShift) | temp >> GetHighBitShift).to_ulong());
  }

  template<std::floating_point T> void set(T input, std::bitset<bits> mask, const std::uint_fast8_t &shift) const
  {
    std::bitset<bits> val{ static_cast<std::uint_fast8_t>(Tools::clamp(input, 0.0F, 1.0F) * largest5BitValue) };
    val <<= shift;
    valueBit((valueBit() & mask.flip()) | val);
  }

  template<std::integral T> void set(T input, std::bitset<bits> mask, const std::uint_fast8_t &shift) const
  {
    std::bitset<bits> val{ static_cast<std::uint_fast8_t>(Tools::clamp(input, 0, UINT8_MAX)) };
    val >>= ConvertShift;
    val <<= shift;
    valueBit((valueBit() & mask.flip()) | val);
  }

public:
  color16() = default;
  explicit color16(std::uint16_t rawColor) : value(rawColor) {}
  template<Color cT> explicit color16(cT color)
  {
    R(color.R());
    G(color.G());
    B(color.B());
    // pass stp bit? assuming alpha is 100% *shrugs*
    if (value == 0) {
      stp(true);
    } else {
      stp(false);
    }
  }
  /**
   * Color Blue stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t B() const { return convert(BlueMask, BlueShift); }
  /**
   * Color Green stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t G() const { return convert(GreenMask, GreenShift); }
  /**
   * Color Red stored as 5 bit.
   * @return 8 bit color value.
   */
  [[nodiscard]] std::uint8_t R() const { return convert(RedMask, RedShift); }
  /**
   * Special transparency bit
   * @return true or false
   */


  /**
   * Color Blue stored as 5 bit.
   * @return 8 bit color value.
   * @tparam T is an integral value. Clamped to 0-255.
   * @param b is a new blue value.
   */
  template<Number T> [[maybe_unused]] std::uint8_t B(const T &b) const
  {
    set(b, BlueMask, BlueShift);
    return B();
  }
  /**
   * Color Green stored as 5 bit.
   * @return 8 bit color value.
   * @tparam T is an integral value. Clamped to 0-255.
   * @param g is a new blue value.
   */
  template<Number T> [[maybe_unused]] std::uint8_t G(const T &g) const
  {
    set(g, GreenMask, GreenShift);
    return G();
  }
  /**
   * Color Red stored as 5 bit.
   * @return 8 bit color value.
   * @tparam T is an integral value. Clamped to 0-255.
   * @param r is a new blue value.
   */
  template<Number T> [[maybe_unused]] std::uint8_t R(const T &r) const
  {
    set(r, RedMask, RedShift);
    return R();
  }
  /**
   * Special transparency bit
   * @return true or false
   */
  [[maybe_unused]] [[nodiscard]] bool stp() const { return (valueBit() & STPMask).any(); }

  /**
   * Special transparency bit
   * @return true or false
   */
  [[maybe_unused]] bool stp(bool enabled) const
  {
    if (enabled) {
      valueBit(valueBit() | STPMask);
    } else {
      valueBit(valueBit() & AllColorMask);
    }
    return enabled;
  }
  /**
   * @return true if color is black. at least all the color bits are 0.
   */

  [[nodiscard]] bool isBlack() const { return (valueBit() & AllColorMask).none(); }

  /**
   * @return true if color is transparent Black. all bits are 0.
   */
  [[nodiscard]] [[maybe_unused]] bool isTransparentBlack() const { return value == 0; }
  [[nodiscard]] std::uint8_t A() const
  {
    if (isBlack()) {
      return 0;
    }
    return UINT8_MAX;
  }
  friend std::ostream &operator<<(std::ostream &os, const color16 &color)
  {
    return os << std::uppercase << std::hex << '{' << static_cast<std::size_t>(color.R()) << ", "
              << static_cast<std::size_t>(color.G()) << ", " << static_cast<std::size_t>(color.B()) << ", "
              << static_cast<std::size_t>(color.A()) << '}' << std::dec << std::nouppercase;
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_COLOR16_H