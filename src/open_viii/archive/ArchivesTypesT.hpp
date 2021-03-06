//
// Created by pcvii on 2/10/2021.
//
#ifndef VIIIARCHIVE_ARCHIVESTYPEST_HPP
#define VIIIARCHIVE_ARCHIVESTYPEST_HPP
#include <concepts>
#include <cstdint>
namespace open_viii::archive {
// There are 6 main FIFLFS archives and 2 main zzz archives for ff8 and ff8
// remaster.
enum class ArchiveTypeT : std::uint8_t {
  battle,
  field,
  magic,
  main,
  menu,
  world,
  zzz_main,
  zzz_other,
  count,
  first = battle,
  begin = battle,
  last  = count - 1,
  end   = count,
};
// static constexpr bool ValidArchiveTypeT(ArchiveTypeT archiveType)
//  {
//    return archiveType < ArchiveTypeT::end && archiveType >=
//    ArchiveTypeT::begin;
//  }
template<std::signed_integral T>
static constexpr bool test_valid_archive_type_t(T    archive_type_t,
                                                bool include_end = false)
{
  return (static_cast<std::intmax_t>(archive_type_t)
            >= static_cast<std::intmax_t>(ArchiveTypeT::first)
          && static_cast<std::intmax_t>(archive_type_t)
               <= static_cast<std::intmax_t>(ArchiveTypeT::last))
         || (include_end
             && static_cast<std::intmax_t>(archive_type_t)
                  == static_cast<std::intmax_t>(ArchiveTypeT::end));
}
template<std::signed_integral T>
static constexpr bool
  test_valid_archive_type_t(T minT, T maxT, bool include_end = false)
{
  return test_valid_archive_type_t(maxT, include_end)
         && test_valid_archive_type_t(minT) && maxT >= minT;
}
template<typename T>
requires(
  std::unsigned_integral<
    T> || std::is_same_v<T, ArchiveTypeT>) static constexpr bool test_valid_archive_type_t(T archive_type_t)
{
  return (static_cast<std::size_t>(archive_type_t)
            >= static_cast<std::size_t>(ArchiveTypeT::first)
          || static_cast<std::size_t>(archive_type_t)
               <= static_cast<std::size_t>(ArchiveTypeT::last));
}
//
// template<ArchiveTypeT archiveTypeT>
// concept ValidArchiveTypeT = (static_cast<std::intmax_t>(archiveTypeT) >=
// static_cast<std::intmax_t>(ArchiveTypeT::first)
//                             || static_cast<std::intmax_t>(archiveTypeT)
//                                  <=
//                                  static_cast<std::intmax_t>(ArchiveTypeT::last));
}// namespace open_viii::archive
#endif// VIIIARCHIVE_ARCHIVESTYPEST_HPP
