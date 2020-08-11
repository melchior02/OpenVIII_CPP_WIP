//
// Created by pcvii on 8/6/2020.
//

#ifndef VIIIARCHIVE_MENUGROUPFILE_H
#define VIIIARCHIVE_MENUGROUPFILE_H
#include "MenuGroupHeader.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Tools/Tools.h"
#include "MenuGroupSectionT.h"
#include "MenuMessages.h"
#include "OpenVIII/SectionData.h"
namespace OpenVIII::MenuGroup {
struct MenuGroupFile
{
private:
  MenuGroupHeader menuGroupHeader_{};
  std::vector<char> dataBuffer_{};
  //std::string_view ToStringView() { return { dataBuffer_.data(), dataBuffer_.size() }; }
  template<MenuGroupSectionT sectionT> auto GetSectionInternal() const
  {
    if constexpr (static_cast<size_t>(sectionT) < MenuGroupHeader::size()) {
      return menuGroupHeader_.Sections().at(static_cast<size_t>(sectionT));
    } else {
      return nullptr;
    }
  }
  static constexpr std::array tkmnmesValueArray = {
    MenuGroupSectionT::tkmnmes1,
    MenuGroupSectionT::tkmnmes2,
    MenuGroupSectionT::tkmnmes3,
  };
  static constexpr std::array refineValueArray = {
    MenuGroupSectionT::refine0,
    MenuGroupSectionT::refine1,
    MenuGroupSectionT::refine2,
    MenuGroupSectionT::refine3,
    MenuGroupSectionT::refine4,
  };
  static constexpr std::array mesValueArray = {
    MenuGroupSectionT::mes1,
    MenuGroupSectionT::mes2,
    MenuGroupSectionT::mes3,
    MenuGroupSectionT::mes4,
    MenuGroupSectionT::mes5,
    MenuGroupSectionT::mes6,
    MenuGroupSectionT::mes7,
    MenuGroupSectionT::mes8,
    MenuGroupSectionT::mes9,
    MenuGroupSectionT::mes10,
    MenuGroupSectionT::mes11,
    MenuGroupSectionT::mes12,
    MenuGroupSectionT::mes13,
    MenuGroupSectionT::mes14,
    MenuGroupSectionT::mes15,
    MenuGroupSectionT::mes16,
    MenuGroupSectionT::mes17,
    MenuGroupSectionT::mes18,
    MenuGroupSectionT::mes19,
    MenuGroupSectionT::mes20,
    MenuGroupSectionT::mes21,
    MenuGroupSectionT::mes22,
    MenuGroupSectionT::mes23,
    MenuGroupSectionT::mes24,
    MenuGroupSectionT::mes25,
    MenuGroupSectionT::mes26,
    MenuGroupSectionT::mes27,
    MenuGroupSectionT::mes28,
    MenuGroupSectionT::mes29,
    MenuGroupSectionT::mes30,
    MenuGroupSectionT::mes31,
    MenuGroupSectionT::mes32,
    MenuGroupSectionT::mes33,
    MenuGroupSectionT::mes34,
    MenuGroupSectionT::mes35,
    MenuGroupSectionT::mes36,
    MenuGroupSectionT::mes37,
    MenuGroupSectionT::mes38,
    MenuGroupSectionT::mes39,
    MenuGroupSectionT::mes40,
    MenuGroupSectionT::mes41,
    MenuGroupSectionT::mes42,
    MenuGroupSectionT::mes43,
    MenuGroupSectionT::mes44,
    MenuGroupSectionT::mes45,
  };
  template<typename refineT, MenuGroupSectionT textSectionT, typename sectionBufferT>
  auto getRefine(const sectionBufferT & sectionBuffer) const
  {
    return OpenVIII::BulkSectionData<refineT,1U>(
      sectionBuffer, GetSectionInternal<textSectionT>().GetSectionBuffer(dataBuffer_));
  }
  template<size_t i, size_t count, typename T> constexpr void static_for_mes(const T t)
  {
    if constexpr (i < count && i < mesValueArray.size()) {
      constexpr auto val{ mesValueArray[i] };
      t(val, GetSection<val>());
      static_for_mes<i + 1U, count>(t);
    }
  }


  template<size_t i, size_t count, typename T> constexpr void static_for_refine(const T t)
  {
    if constexpr (i < count && i < refineValueArray.size()) {
      constexpr auto val{ refineValueArray[i] };
      t(val, GetSection<val>());
      static_for_refine<i + 1U, count>(t);
    }
  }

  template<size_t i, size_t count, typename T> constexpr void static_for_tkmnmes(const T t)
  {
    if constexpr (i < count && i < tkmnmesValueArray.size()) {
      constexpr auto val{ tkmnmesValueArray[i] };
      t(val, GetSection<val>());
      static_for_tkmnmes<i + 1U, count>(t);
    }
  }

public:
  explicit MenuGroupFile(const OpenVIII::Archive::FIFLFS<false> &menuArchive)
    : menuGroupHeader_(menuArchive), dataBuffer_(menuArchive.GetEntryData("mngrp.bin"))
  {}

  template<MenuGroupSectionT sectionT> [[nodiscard]] auto GetSection() const
  {
    [[maybe_unused]] auto section{ GetSectionInternal<sectionT>() };
    if constexpr (std::is_null_pointer_v<decltype(section)>) {
      return nullptr;
    } else {
      // return mngrphd.Sections().at(id).GetSectionBuffer(mngrpBuffer);
      [[maybe_unused]] const auto sectionBuffer{ section.GetSectionBuffer(dataBuffer_) };
      if constexpr (sectionT == MenuGroupSectionT::tkmnmes1 || sectionT == MenuGroupSectionT::tkmnmes2
                    || sectionT == MenuGroupSectionT::tkmnmes3) {
        return SectionData<MenuMessages>(MenuMessages{ sectionBuffer }, sectionBuffer);
      } else if constexpr (Tools::any_of(sectionT, mesValueArray)) {
        return SectionData<MenuMessagesSection>(MenuMessagesSection{ sectionBuffer }, sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine0) {
        return getRefine<OpenVIII::MenuGroup::RefineSection000, MenuGroupSectionT::refineText0>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine1) {
        return getRefine<OpenVIII::MenuGroup::RefineSection001, MenuGroupSectionT::refineText1>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine2) {
        return getRefine<OpenVIII::MenuGroup::RefineSection002, MenuGroupSectionT::refineText2>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine3) {
        return getRefine<OpenVIII::MenuGroup::RefineSection003, MenuGroupSectionT::refineText3>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine4) {
        return getRefine<OpenVIII::MenuGroup::RefineSection004, MenuGroupSectionT::refineText4>(sectionBuffer);
      }
    }
  }
  template<LangT langVal> constexpr bool TestMes()
  {
    static_for_mes<0U, mesValueArray.size()>([&, this](const auto &sectionID, [[maybe_unused]] const auto &mes) {
      std::cout << ':' << static_cast<size_t>(sectionID) << ":  {" << mes.size() << "},\n";
      size_t id = 0;
      for (const auto &subSection : mes) {
        id++;
        if (subSection.Offset() == 0) {
          continue;
        }

        std::cout << "    " << id - 1 << ": {" << subSection.Offset() << "} "
                  << subSection.template DecodedString<langVal>(mes.TextSpan(), 0, true) << '\n';
      }
    });
    return true;
  }
  template<LangT langVal> constexpr bool TestTkMnMes()
  {
    constexpr auto start = 0U;
    static_for_tkmnmes<start, mesValueArray.size() - start>(
      [&, this](const auto &sectionID, [[maybe_unused]] const auto &tkmnmesPair) {
        std::cout << ':' << static_cast<size_t>(sectionID) << ":\n  {" << tkmnmesPair->Sections().size() << ", "
                  << tkmnmesPair->SubSections().size() << "},\n";
        for (size_t id = 0; id < tkmnmesPair->Sections().size() && id < tkmnmesPair->SubSections().size(); id++) {
          [[maybe_unused]] const auto offset = tkmnmesPair->Sections().at(id);
          const auto subSectionGroup = tkmnmesPair->SubSections().at(id);
          [[maybe_unused]] size_t stringNumber{};
          for (const auto &subSection : subSectionGroup) {
            if (subSection.Offset() == 0) {
              continue;
            }
            std::cout << "    " << stringNumber++ << ": {" << subSection.Offset() << "} "
                      << subSection.template DecodedString<langVal>(tkmnmesPair.TextSpan(), offset, true) << '\n';
          }
        }
      });
    return true;
  }


  template<LangT langVal> constexpr bool TestRefine()
  {
    constexpr auto start = 0U;
    static_for_refine<start, refineValueArray.size() - start>(
      [&, this](const auto &sectionID, [[maybe_unused]] const auto &refineBulkSectionData) {
             std::cout << ':' << static_cast<size_t>(sectionID) << ":\n  {" << refineBulkSectionData.size() << "},\n";
//             refineBulkSectionData[0].
//             static_for([&, this]( [[maybe_unused]]const size_t & id, [[maybe_unused]]const auto & sectionData){
//
//                                                     //sectionData.template out<langVal>(std::cout,refineBulkSectionData.TextSpan());
//               });

             for(size_t id = 0U; id < 1U;id++)
             {
               refineBulkSectionData.at(id).template out<langVal>(std::cout,refineBulkSectionData.TextSpan());
             }

//             for (size_t id = 0; id < tkmnmesPair->Sections().size() && id < tkmnmesPair->SubSections().size(); id++) {
//               [[maybe_unused]] const auto offset = tkmnmesPair->Sections().at(id);
//               const auto subSectionGroup = tkmnmesPair->SubSections().at(id);
//               [[maybe_unused]] size_t stringNumber{};
//               for (const auto &subSection : subSectionGroup) {
//                 if (subSection.Offset() == 0) {
//                   continue;
//                 }
//                 std::cout << "    " << stringNumber++ << ": {" << subSection.Offset() << "} "
//                           << subSection.template DecodedString<langVal>(tkmnmesPair.TextSpan(), offset, true) << '\n';
//               }
             //}
      });
    return true;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_MENUGROUPFILE_H