A general-purpose spoofing / hijacking DLL creator.

## Overview

This tool creates a DLL that spoofs Windows system DLLs, e.g. `winmm.dll`, to trick applications into loading our version of these DLLs at application launch. By putting our code in the spoofed DLLs, we gain full control of the application process as soon as it launches.

For the details of DLL spoofing, see: https://prog.world/dll-spoofing-dll-hijacking/

## Build instructions

The project uses Meson as the build system. You also need to install Python and `pefile` from pip.

To build the spoofing DLL, simply run:

```
meson build
cd build
ninja
```

To build in Release mode, run:

```
meson build --buildtype release
cd build
ninja
```

Note that the generated DLL name is `hook.dll` using MSVC or `winmm.dll` using MinGW. Do not use `libhook.dll`, since it does not contain a correct exports table.

The project contains a sample code that enables HiDPI support for all applications loading the spoofing DLL. This achieves the same effect as by .exe File Properties → Compatibility → Change high DPI settings → Override high DPI scaling behavior, without writing to the registry.

The target DLL to be spoofed can be changed by modifying the `dllname` variable in `build.meson`. Note that some DLLs are always loaded from `System32`, like `kernel32.dll` and `user32.dll`, and therefore cannot be spoofed in this way.

## Supported compilers

- MSVC x86
- MinGW x86 / x64

MSVC x64 is not currently supported because of the lack of support for inline assembly.

## Credits

Thanks to [ysc3839/FontMod](https://github.com/ysc3839/FontMod) and [an article on Tencent Cloud](https://cloud.tencent.com/developer/article/1498506) (in Chinese) for the techniques used in this project.
