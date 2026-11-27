#include "sha512.h"

#include <cstdint>

namespace c6502 {

namespace {

constexpr std::array<std::uint64_t, 80> kRound = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

std::uint64_t rotr(std::uint64_t x, int n) { return (x >> n) | (x << (64 - n)); }

// Folds the 128-byte block at `p` into the state `h`.
void compress(std::array<std::uint64_t, 8>& h, const unsigned char* p) {
  std::array<std::uint64_t, 80> w{};
  for (int i = 0; i < 16; ++i) {
    std::uint64_t v = 0;
    for (int k = 0; k < 8; ++k) v = (v << 8) | p[8 * i + k];
    w[i] = v;
  }
  for (int i = 16; i < 80; ++i) {
    const std::uint64_t s0 = rotr(w[i - 15], 1) ^ rotr(w[i - 15], 8) ^ (w[i - 15] >> 7);
    const std::uint64_t s1 = rotr(w[i - 2], 19) ^ rotr(w[i - 2], 61) ^ (w[i - 2] >> 6);
    w[i] = w[i - 16] + s0 + w[i - 7] + s1;
  }
  std::uint64_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], f = h[5], g = h[6], k = h[7];
  for (int i = 0; i < 80; ++i) {
    const std::uint64_t t1 =
        k + (rotr(e, 14) ^ rotr(e, 18) ^ rotr(e, 41)) + ((e & f) ^ (~e & g)) + kRound[i] + w[i];
    const std::uint64_t t2 =
        (rotr(a, 28) ^ rotr(a, 34) ^ rotr(a, 39)) + ((a & b) ^ (a & c) ^ (b & c));
    k = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }
  h[0] += a;
  h[1] += b;
  h[2] += c;
  h[3] += d;
  h[4] += e;
  h[5] += f;
  h[6] += g;
  h[7] += k;
}

}  // namespace

std::array<unsigned char, 64> sha512(std::string_view data) {
  std::array<std::uint64_t, 8> h = {0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b,
                                    0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f,
                                    0x1f83d9abfb41bd6b, 0x5be0cd19137e2179};
  const auto* bytes = reinterpret_cast<const unsigned char*>(data.data());
  std::size_t i = 0;
  for (; i + 128 <= data.size(); i += 128) compress(h, bytes + i);
  // The last block holds the rest of the data, a 1 bit, zeros, and the
  // length in bits as a big-endian 128-bit number, in one block or two.
  std::array<unsigned char, 256> tail{};
  const std::size_t rest = data.size() - i;
  for (std::size_t k = 0; k < rest; ++k) tail[k] = bytes[i + k];
  tail[rest] = 0x80;
  const std::size_t blocks = rest + 17 <= 128 ? 1 : 2;
  const std::uint64_t bits = static_cast<std::uint64_t>(data.size()) * 8;
  for (int k = 0; k < 8; ++k) {
    tail[blocks * 128 - 1 - k] = static_cast<unsigned char>(bits >> (8 * k));
  }
  for (std::size_t b = 0; b < blocks; ++b) compress(h, tail.data() + 128 * b);
  std::array<unsigned char, 64> out{};
  for (int w = 0; w < 8; ++w) {
    for (int k = 0; k < 8; ++k) out[8 * w + k] = static_cast<unsigned char>(h[w] >> (56 - 8 * k));
  }
  return out;
}

}  // namespace c6502
