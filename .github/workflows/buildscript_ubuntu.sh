sudo apt-get install ninja-build
sudo apt-get install lcov
ninja    --version
cmake    --version
gcc      --version
gcc-11   --version
clang    --version
clang-14 --version
lcov     --version
vcpkg    --version

rm -rf out
rm -rf build
mkdir build

export VCPKG_ROOT=/usr/local/share/vcpkg
cmake --preset "clang-release"
cmake --build --preset "clang-release" --target all
cmake --build --preset "clang-release" --target install
