#!/bin/bash
# Builds YARPGen, patched for c6502, into fuzz_out/yarpgen-src, for the
# fuzzer `c6502cpp fuzz yarpgen` (cpp/src/fuzz/yarpgen.h). Run it from
# the repository's root.
#
#   bash fuzz/build_yarpgen.sh
#
# YARPGen (https://github.com/intel/yarpgen) generates random C programs
# that are free of undefined behavior under its own model of C's types.
# The script clones it, checks out the commit that fuzz/yarpgen.patch
# applies to, applies the patch, and builds only the generator, with
# -DYARPGEN_INT_BITS=16. YARPGen's own unit tests assume a 32-bit int,
# which is why they aren't built. The binary is
# fuzz_out/yarpgen-src/build/yarpgen.exe on Windows and
# fuzz_out/yarpgen-src/build/yarpgen elsewhere.
#
# The compiler is the clang that comes with Zig 0.17.1, which the script
# downloads from ziglang.org into fuzz_out the first time it runs.
# YARPGen draws every random value through the C++ standard library's
# std::uniform_int_distribution, std::discrete_distribution, and
# std::shuffle, and the C++ standard doesn't specify how they turn the
# engine's numbers into values. A build against another standard library
# therefore writes other programs for the same seed. Zig's clang builds
# against libc++, as the fuzzer's earlier builds did, so a seed in an
# earlier fuzzer report still names the program it named then.
#
# MSVC builds this commit too, in about 20 s, with Visual Studio's CMake,
# the generator "Visual Studio 17 2022", -A x64, and `cmake --build
# <dir> --config Release --target yarpgen`. Its standard library's
# distributions differ from libc++'s, and on 27 Nov 2026 none of seeds 1
# to 100 gave the same programs as the Zig build. Switching to MSVC means
# replacing the Zig steps below with that configure, copying
# build/Release/yarpgen.exe to build/yarpgen.exe, where the fuzzer
# looks for it, and accepting that every seed then names a new program.
#
# CMake and Ninja come from the PATH. On Windows, when either is missing
# from it, they come from the copies that Visual Studio's C++ CMake tools
# install, which the script finds with vswhere.
set -euo pipefail

repo=$(cd "$(dirname "$0")/.." && pwd)
out=$repo/fuzz_out
src=$out/yarpgen-src
url=https://github.com/intel/yarpgen.git
# The commit fuzz/yarpgen.patch applies to.
pin=1adb290f453505838f3aa33dc571f292b0c1810f
zig_version=0.17.1

case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*) os=windows ;;
  Darwin) os=macos ;;
  *) os=linux ;;
esac
arch=$(uname -m)
if [ "$arch" = arm64 ]; then
  arch=aarch64
fi

# Prints a path in the form a native Windows program reads, since CMake
# and Ninja on Windows can't open an MSYS path such as /c/Users.
native() {
  if [ $os = windows ]; then
    cygpath -m "$1"
  else
    printf '%s\n' "$1"
  fi
}

# Downloads Zig into fuzz_out, unless it's already there. The SHA-256 of
# each archive is from https://ziglang.org/download/index.json.
fetch_zig() {
  local name=zig-$arch-$os-$zig_version sha ext=tar.xz
  case $name in
    zig-x86_64-windows-0.17.1)
      sha=b03f477a1943673e02aca0f570e8030dffd536ce1f45cbc4d3326ec31159562e ;;
    zig-aarch64-windows-0.17.1)
      sha=4789895bdb1e312a2ea73b2a7c0a0cc8d2ab62ba6976e834a7c54cf443377c15 ;;
    zig-x86_64-linux-0.17.1)
      sha=3581863e3cbcfd0fce0efc682ce7e9e93da96668a977791d356a26d2a7e9b17a ;;
    zig-aarch64-linux-0.17.1)
      sha=a1546d5646eb184437918d6635ca6b762a0e889eac09cef720ee7a1ed3fe7795 ;;
    zig-x86_64-macos-0.17.1)
      sha=9af07b20319fbd0c92ee46ad23e81d6a782b1aaea2cfb8dcbff35f1d2c7ddbb7 ;;
    zig-aarch64-macos-0.17.1)
      sha=75f1bc7a1e9852f17ec9a0c1895f767c4ec56d46bc8a834920eece1d0359dd8d ;;
    *)
      echo "build_yarpgen.sh: no checksum for $name" >&2
      exit 1 ;;
  esac
  if [ $os = windows ]; then
    ext=zip
  fi
  zig=$out/$name/zig
  if [ $os = windows ]; then
    zig=$zig.exe
  fi
  if [ -x "$zig" ]; then
    return
  fi
  mkdir -p "$out"
  local archive=$out/$name.$ext
  curl -fL --retry 3 --progress-bar -o "$archive" \
    "https://ziglang.org/download/$zig_version/$name.$ext"
  local got
  if command -v sha256sum > /dev/null; then
    got=$(sha256sum "$archive" | cut -d' ' -f1)
  else
    got=$(shasum -a 256 "$archive" | cut -d' ' -f1)
  fi
  if [ "$got" != "$sha" ]; then
    echo "build_yarpgen.sh: $archive has SHA-256 $got, expected $sha" >&2
    exit 1
  fi
  if [ $ext = zip ]; then
    unzip -q -o "$archive" -d "$out"
  else
    tar -xJf "$archive" -C "$out"
  fi
  rm "$archive"
}

# Writes scripts that run `zig c++`, `zig ar`, and `zig ranlib`, since
# CMake takes one executable for each tool.
write_wrappers() {
  local tools=$src/tools name sub
  mkdir -p "$tools"
  for name in cxx:c++ ar:ar ranlib:ranlib; do
    sub=${name#*:}
    name=${name%%:*}
    if [ $os = windows ]; then
      printf '@"%s" %s %%*\r\n' "$(cygpath -w "$zig")" "$sub" \
        > "$tools/z$name.cmd"
    else
      printf '#!/bin/sh\nexec "%s" %s "$@"\n' "$zig" "$sub" > "$tools/z$name"
      chmod +x "$tools/z$name"
    fi
  done
}

find_cmake() {
  if command -v cmake > /dev/null && command -v ninja > /dev/null; then
    return
  fi
  if [ $os != windows ]; then
    echo "build_yarpgen.sh: CMake and Ninja must be on the PATH" >&2
    exit 1
  fi
  local vswhere="/c/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe"
  local install ext
  while IFS= read -r install; do
    ext=$(cygpath -u "${install%$'\r'}")/Common7/IDE/CommonExtensions/Microsoft/CMake
    if [ -x "$ext/CMake/bin/cmake.exe" ] && [ -x "$ext/Ninja/ninja.exe" ]; then
      PATH=$ext/CMake/bin:$ext/Ninja:$PATH
      return
    fi
  done < <("$vswhere" -all -products '*' -property installationPath)
  echo "build_yarpgen.sh: no CMake and Ninja on the PATH or in Visual Studio" >&2
  exit 1
}

if [ ! -e "$src/.git" ]; then
  mkdir -p "$out"
  git clone -q "$url" "$src"
fi
# Checking out with --force undoes the patch of an earlier run, so that it
# applies again.
git -C "$src" checkout -q --force "$pin"
git -C "$src" apply "$repo/fuzz/yarpgen.patch"

fetch_zig
write_wrappers
find_cmake

if [ $os = windows ]; then
  wrapper_ext=.cmd
else
  wrapper_ext=
fi
cmake -S "$(native "$src")" -B "$(native "$src/build")" -G Ninja \
  "-DCMAKE_MAKE_PROGRAM=$(native "$(command -v ninja)")" \
  "-DCMAKE_CXX_COMPILER=$(native "$src/tools/zcxx$wrapper_ext")" \
  "-DCMAKE_AR:FILEPATH=$(native "$src/tools/zar$wrapper_ext")" \
  "-DCMAKE_RANLIB:FILEPATH=$(native "$src/tools/zranlib$wrapper_ext")" \
  -DYARPGEN_INT_BITS=16
cmake --build "$(native "$src/build")" --target yarpgen

if [ $os = windows ]; then
  native "$src/build/yarpgen.exe"
else
  printf '%s\n' "$src/build/yarpgen"
fi
