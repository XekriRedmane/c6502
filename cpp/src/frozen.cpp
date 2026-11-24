#include "frozen.h"

#include <fstream>
#include <stdexcept>

#include "unxz.h"

namespace c6502::frozen {

std::size_t unpack(std::string_view data, const std::filesystem::path& dir) {
  const std::string text = xz_decompress(data);
  constexpr std::string_view kFormat = "c6502-frozen 1\n";
  if (text.compare(0, kFormat.size(), kFormat) != 0) {
    throw std::runtime_error("not an archive of frozen captures");
  }
  std::size_t pos = kFormat.size();
  std::size_t files = 0;
  while (pos < text.size()) {
    const std::size_t end = text.find('\n', pos);
    if (end == std::string::npos || text.compare(pos, 5, "file ") != 0) {
      throw std::runtime_error("an archive's file line can't be read");
    }
    const std::string line = text.substr(pos + 5, end - pos - 5);
    const std::size_t space = line.rfind(' ');
    const std::string path = line.substr(0, space);
    const std::size_t length = std::stoull(line.substr(space + 1));
    pos = end + 1;
    if (path.find("..") != std::string::npos || length > text.size() - pos) {
      throw std::runtime_error("an archive names a file it doesn't hold: " + path);
    }
    const std::filesystem::path target = dir / std::filesystem::path(
                                                   std::u8string(path.begin(), path.end()));
    std::filesystem::create_directories(target.parent_path());
    std::ofstream(target, std::ios::binary).write(text.data() + pos,
                                                  static_cast<std::streamsize>(length));
    pos += length;
    ++files;
  }
  return files;
}

}  // namespace c6502::frozen
