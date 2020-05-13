#ifndef OPENVIII_FIFLFS_H
#define OPENVIII_FIFLFS_H

#include "FI.hpp"
#include "FL.h"
#include "FS.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <iterator>

namespace OpenVIII::Archive {
struct FIFLFS
{
private:
  std::filesystem::path fi_{ "" };
  std::filesystem::path fs_{ "" };
  std::filesystem::path fl_{ "" };
  size_t fiOffset_{ 0U };
  size_t fsOffset_{ 0U };
  size_t flOffset_{ 0U };
  size_t count_{ 0U };
  std::vector<char> fiData_{};
  std::vector<char> fsData_{};
  std::basic_string<char> flData_{};// this is char because the file contains strings.
  std::string fiBase_{};
  std::string fsBase_{};
  std::string flBase_{};

public:
  [[maybe_unused]] [[nodiscard]] auto FI() const { return fi_; }

  [[maybe_unused]] [[nodiscard]] auto FS() const { return fs_; }

  [[maybe_unused]] [[nodiscard]] auto FL() const { return fl_; }

  [[maybe_unused]] [[nodiscard]] auto FIData() const { return fiData_; }

  [[maybe_unused]] [[nodiscard]] auto FSData() const { return fsData_; }

  [[maybe_unused]] [[nodiscard]] auto FLData() const { return flData_; }
  [[maybe_unused]] void FI(const std::filesystem::path &fi, const size_t &offset = 0U, size_t size = 0U)
  {
    fi_ = fi;
    fiOffset_ = offset;
    if (size == 0U) {
      if (std::filesystem::exists(fi_)) { size = std::filesystem::file_size(fi_); }
    }
    count_ = FI::GetCount(size);
  }
  void FI(const std::filesystem::path &fi, const std::vector<char> &vector, const size_t &offset = 0U)
  {
    FI(fi, offset, vector.size());
    if (!vector.empty()) { fiData_ = vector; }
  }

  void FS(const std::filesystem::path &fs, const size_t &offset = 0U)
  {
    fs_ = fs;
    fsOffset_ = offset;
  }
  void FS(const std::filesystem::path &fs, const std::vector<char> &vector, const size_t &offset = 0U)
  {
    FS(fs, offset);
    if (!vector.empty()) { fsData_ = vector; }
  }

  void FL(const std::filesystem::path &fl, const size_t &offset = 0U)
  {
    fl_ = fl;
    flOffset_ = offset;
  }

  void FL(const std::filesystem::path &fl, const std::vector<char> &vector, const size_t &offset = 0U)
  {
    FL(fl, offset);
    if (!vector.empty()) {
      flData_ = std::basic_string<char>(vector.begin(), vector.end());
      // check for garbage
      auto index =
        flData_.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_\n\r\\:/.-");
      if (index != std::string::npos) {
        std::cerr << "\nError: Invalid character (" << static_cast<int>(flData_[index])
                  << ") found in FL data! = " << fl << '\n';
        exit(EXIT_FAILURE);
      }
    }
  }

  [[maybe_unused]] void FL(const std::filesystem::path &fl, const std::string_view &vector, const size_t &offset = 0U)
  {
    FL(fl, offset);
    if (!vector.empty()) { flData_ = vector; }
  }

  [[nodiscard]] bool AllSet() const { return (fi_ != "") && (fs_ != "") && (fl_ != ""); }

  FIFLFS() = default;

  FIFLFS(const std::filesystem::path &fi,
    const std::filesystem::path &fs,
    const std::filesystem::path &fl,
    const size_t &fiOffset = 0U,
    const size_t &fsOffset = 0U,
    const size_t &flOffset = 0U)
  {
    fi_ = fi;
    fs_ = fs;
    fl_ = fl;
    fiOffset_ = fiOffset;
    flOffset_ = flOffset;
    fsOffset_ = fsOffset;
  }

  [[nodiscard]] friend std::ostream &operator<<(std::ostream &os, const FIFLFS &data)
  {
    os << '{' << data.GetBaseName() << ", " << '{' << data.fi_ << ", " << data.fl_ << ", " << data.fs_ << "}}";
    return os;
  }

  [[nodiscard]] auto GetEntry(const unsigned int &id) const
  {
    if (id < count_ || count_ == 0) {
      if (!fiData_.empty()) {
        return FI::GetEntry(fiData_, id, fiOffset_);
      } else {
        return FI::GetEntry(fi_, id, fiOffset_);
      }
    }
    return OpenVIII::Archive::FI();
  }

  [[maybe_unused]] [[nodiscard]] auto GetEntry(const std::string_view &needle) const
  {
    auto found = FL::GetEntry(fl_, needle, fiOffset_);
    const auto &[id, path] = found;
    return GetEntry(id);
  }
  auto TryAdd(const std::string_view &strPath, const std::vector<char> &buffer)
  {
    const auto directoryEntry = std::filesystem::directory_entry(strPath);
    return TryAdd(directoryEntry, buffer);
  }
  auto static TryAddTestReset(FIFLFS &archive, const std::string_view &strPath, const std::vector<char> &buffer)
  {
    switch (archive.TryAdd(strPath, buffer)) {
    case 1:
      return true;
    case 2:
      std::cout << archive << std::endl;
      archive.Test();
      archive = FIFLFS();
      return true;
    }
    return false;
  }
  void Test() const
  {
    std::cout << "Getting Filenames from : " << fl_ << std::endl;
    const auto entries = Archive::FL::GetAllEntriesData(fl_, flData_, flOffset_,count_);
    auto archive = FIFLFS();
    for (const auto &item : entries) {
      const auto &[id, strPath] = item;
      auto fi = GetEntry(id);

      std::vector<char> buffer{};
      if (fi.UncompressedSize() > 0) {
        if (!fsData_.empty()) {
          buffer = FS::GetEntry(fsData_, fi, fsOffset_);
        } else {
          buffer = FS::GetEntry(fs_, fi, fsOffset_);
        }
      }

      if (buffer.size() == 0) {
        std::cout << '{' << id << ", "
                  << "Empty!"
                  << ", " << strPath << "}" << fi << std::endl;
        if (fi.UncompressedSize() > 0) { exit(EXIT_FAILURE); }
      } else {
        if (TryAddTestReset(archive, strPath, buffer)) { continue; }
        std::cout << '{' << id << ", " << buffer.size() << ", " << strPath << "}" << std::endl;
        Tools::WriteBuffer(buffer, strPath);
      }
      if (fi.UncompressedSize() != buffer.size()) { exit(EXIT_FAILURE); }
    }
  }

  char static CheckExtension(const std::filesystem::path &path)
  {

    char i = 0;// 0 does not match;
    if (path.has_extension())
      for (const auto &ext : { OpenVIII::Archive::FL::Ext, OpenVIII::Archive::FS::Ext, OpenVIII::Archive::FI::Ext }) {
        i++;
        if (OpenVIII::Tools::iEquals(path.extension().string(), ext)) { return i; }
      }

    return 0;
  }
  /*
   * 0 = didn't add
   * 1 = added
   * 2 = added and all set
   * */
  char TryAdd(const std::filesystem::directory_entry &fileEntry, const std::vector<char> &vector)
  {
    switch (CheckExtension(fileEntry.path())) {
    case 1:
      FL(fileEntry, vector, 0U);
      flBase_ = FLBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 2:
      FS(fileEntry, vector, 0U);
      fsBase_ = FSBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    case 3:
      FI(fileEntry, vector, 0U);
      fiBase_ = FIBaseName();
      compareBaseNames();
      return AllSet() ? 2 : 1;
    default:
      break;
    }
    return 0;
  }
  void compareBaseNames()
  {
    if ((flBase_ == fsBase_ && fiBase_ == fsBase_) || std::empty(flBase_) || std::empty(fiBase_)
        || std::empty(fsBase_)) {
      return;
    }
    if (flBase_ != fsBase_ || fiBase_ != fsBase_) {
      if (flBase_ == fiBase_) {
        std::cerr << "Removing FS Data: " << fs_ << "\n";
        fsBase_ = std::string{};
        fsData_ = std::vector<char>{};
        fsOffset_ = 0;
        fs_ = std::filesystem::path{};

        exit(EXIT_FAILURE);
      }
    } else if (fiBase_ != flBase_) {
      if (flBase_ == fsBase_) {
        std::cerr << "Removing FL Data: " << fl_ << "\n";
        fiBase_ = std::string{};
        fiData_ = std::vector<char>{};
        fiOffset_ = 0;
        fi_ = std::filesystem::path{};

        exit(EXIT_FAILURE);
      } else if (fiBase_ == fsBase_) {
        std::cerr << "Removing FI Data: " << fi_ << "\n";
        flBase_ = std::string{};
        flData_ = std::basic_string<char>{};
        flOffset_ = 0;
        fl_ = std::filesystem::path{};

        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "No files matched!\n";
      exit(EXIT_FAILURE);
    }
  }

  // todo move get files to here
  using FIFLFSmap = std::map<std::string, OpenVIII::Archive::FIFLFS>;

  [[maybe_unused]] static FIFLFSmap GetFilesFromPath(const std::string_view path)
  {
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    return GetFilesFromIterator(std::filesystem::directory_iterator(path, options));// todo may need sorted.
  }
  template<class iter_t> [[nodiscard]] static FIFLFSmap GetFilesFromIterator(iter_t iter)
  {
    auto tmp = FIFLFSmap();
    auto archive = OpenVIII::Archive::FIFLFS();
    for (const auto &fileEntry : iter) {
      if (archive.TryAdd(fileEntry, std::vector<char>())) {
        if (archive.AllSet()) {// todo confirm basename matches right now i'm assuming the 3 files are together.
          // todo check for language codes to choose correct files
          tmp.insert(std::make_pair(archive.GetBaseName(), archive));
          archive = OpenVIII::Archive::FIFLFS();
        }
      }
    }
    return tmp;
  }
  [[nodiscard]] std::string FIBaseName() const { return GetBaseName(FI()); }
  [[nodiscard]] std::string FLBaseName() const { return GetBaseName(FL()); }
  [[nodiscard]] std::string FSBaseName() const { return GetBaseName(FS()); }
  [[nodiscard]] std::string static GetBaseName(const std::filesystem::path &path)
  {
    if (path.string().empty()) return std::string{};
    auto name = path.filename().stem().string();
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    return name;
  }
  [[nodiscard]] std::string GetBaseName() const
  {
    std::string name;
    for (const auto &path : { FI(), FL(), FS() }) {
      if (!path.string().empty()) {
        name = path.filename().stem().string();
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        break;
      }
    }
    return name;
  }

  [[maybe_unused]] static void testPair(const std::pair<std::string_view, OpenVIII::Archive::FIFLFS> &pair)
  {
    const auto &[name, paths] = pair;
    std::cout << paths << '\n';
    paths.Test();
    // testFLPath(paths.FL(),paths.FI());
  }
  // todo make get files allow recursive archives
};
}// namespace OpenVIII::Archive
#endif// !OPENVIII_FIFLFS_H