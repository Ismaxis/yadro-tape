# Pre-interview test task (Yadro TATLIN Data Services)
Build using [vcpkg](https://vcpkg.io/en/) and cmake

### Dependencies
```bash
vcpkg install gflags jsoncpp gtest
```

### Build and run
```bash
cmake . -B ./build -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/vcpkg/scripts/buildsystems/vcpkg.cmake [-DVCPKG_TARGET_TRIPLET=<triplet>]
cd build
make
cd ..
./build/tape-main --input <input file> --output <input file> [--config <config-file>] [-v]
```
