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
#include "paths.h"
std::array<std::string, Paths::count> Paths::get()
{
  return { R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII/Data/lang-en)",
    R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)",
    R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII)",
    R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII)",
    R"(/mnt/k/ff82000)",
    R"(K:\ff82000)",
    R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered)",
    R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII Remastered)" };
}