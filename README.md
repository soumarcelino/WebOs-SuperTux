# WebOs SuperTux

A native LG webOS port of [SuperTux 0.6.3](https://github.com/SuperTux/supertux/tree/v0.6.3), using SDL2 and OpenGL ES 2. Tested on a rooted LG webOS TV.

## Build

You need a compatible ARM webOS SDK, CMake, the [SuperTux build dependencies](https://github.com/SuperTux/supertux/blob/v0.6.3/INSTALL.md), and the webOS `ares` CLI tools.

```sh
git clone https://github.com/soumarcelino/WebOs-SuperTux.git
git clone --recursive --branch v0.6.3 https://github.com/SuperTux/supertux.git
cd supertux
git apply ../WebOs-SuperTux/webos-tv.patch
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/sdk/share/buildroot/toolchainfile.cmake \
  -DCMAKE_BUILD_TYPE=Release -DENABLE_OPENGL=ON -DENABLE_OPENGLES2=ON \
  -DBUILD_TESTS=OFF -DBUILD_DOCUMENTATION=OFF
cmake --build build -j 8
```

Compile `launcher.c` for ARM as `package/run-supertux`. Place the built `supertux2`, required shared libraries, and `data.zip` containing the upstream game data in `package/`. Include the generated `build/data/levels/misc/menu.stl` in `data.zip`. Copy `LICENSE.txt` to `package/`, then package the app:

```sh
cd ../WebOs-SuperTux
ares-package -A arm -o . package
```

## Run

With the TV configured as an `ares` device, install the resulting IPK and launch the app. Replace `DEVICE` with your device name.

```sh
ares-install -d DEVICE matias.supertux_0.6.6_arm.ipk
ares-launch -d DEVICE matias.supertux
```

## License

GPL-3.0-or-later. See [LICENSE.txt](LICENSE.txt) and the upstream project's license notices.
