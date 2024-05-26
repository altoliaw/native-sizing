rm -rf build
mkdir -p build
cmake -S . -B build
cmake --build build
cd build && ctest
cd ..