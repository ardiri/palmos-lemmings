# Building on macOS (Apple Silicon)

By default `src/Makefile` builds as before, with prc-tools 2.x
(`palmdev-prep`), `arm-elf-gcc` and `prc2bin` in the `PATH`.

Setting `PALMDEV` switches to a local, sudo-free toolchain on macOS
(Apple Silicon) instead.

## Toolchain

| Tool | Source | Location (`src/Makefile`) |
|------|--------|-----------------------------------|
| m68k-palmos-gcc 2.95.3, multigen, build-prc, arm-palmos-gcc 3.3.1 | [palmdev-macos](https://github.com/User7142/palmdev-macos) with [prc-tools-remix](https://github.com/savaughn/prc-tools-remix/releases/tag/macos-arm) unpacked to `toolchain/` | `PALMDEV` (e.g. `~/tools/palmdev-macos`) |
| Palm OS SDK 4.0 | `sdk/sdk-4` of palmdev-macos | – |
| pilrc 3.2 + 64-bit BMP fix | see below | `PILRC_DIR` (default `~/tools/pilrc-3.2-64bit/bin`) |
| prc2bin | `tools/prc2bin.py` (this directory) | – |

No `palmdev-prep` run is needed: the Makefile passes the `-B` search paths
and all SDK include directories itself.

## pilrc and 24-bit BMPs

pilrc 3.2 declares the fields of `BITMAPINFOHEADER` as `long`, which is
8 bytes on 64-bit hosts. The header is then read with a wrong layout and
BMPs fail with *"Bitmap not monochrome, 16, 256, 16bit, 24bit or 32bit
color"*. The fix is to use 32-bit `int` fields (`bitmap.c`, lines 72–79):

```sh
tar xzf pilrc-3.2.tar.gz && cd pilrc-3.2
sed -i '' -E '72,79s/^  long /  int /' bitmap.c
mkdir build && cd build
CFLAGS="-O2 -Wno-implicit-function-declaration -Wno-int-conversion" \
  ../unix/configure --prefix=$HOME/tools/pilrc-3.2-64bit
make install
```

## Build

```sh
cd src && make PALMDEV=~/tools/palmdev-macos     # -> ../lemmings_en.prc
```

Without `PALMDEV`, `make` uses the tools in the `PATH` as before.
