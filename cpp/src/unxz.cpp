#include "unxz.h"

#include <cstdint>
#include <memory>
#include <stdexcept>

// XZ Embedded's header declares its functions `extern "C"` for C++.
#include "xz.h"

namespace c6502 {

std::string xz_decompress(std::string_view data) {
  xz_crc32_init();
  // The dictionary is allocated when the stream's header gives its size,
  // up to 64 MiB, the dictionary of xz's preset 9.
  const std::unique_ptr<xz_dec, void (*)(xz_dec*)> dec(xz_dec_init(XZ_DYNALLOC, 1u << 26),
                                                       &xz_dec_end);
  if (!dec) throw std::runtime_error("the xz decoder can't be allocated");
  std::string out;
  static thread_local unsigned char buf[1 << 16];
  xz_buf b{};
  b.in = reinterpret_cast<const std::uint8_t*>(data.data());
  b.in_size = data.size();
  b.out = buf;
  b.out_size = sizeof buf;
  for (;;) {
    b.out_pos = 0;
    const xz_ret ret = xz_dec_run(dec.get(), &b);
    out.append(reinterpret_cast<const char*>(buf), b.out_pos);
    if (ret == XZ_STREAM_END) return out;
    if (ret != XZ_OK) {
      throw std::runtime_error("the xz stream can't be decoded (XZ Embedded's error " +
                               std::to_string(static_cast<int>(ret)) + ")");
    }
  }
}

}  // namespace c6502
