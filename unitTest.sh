rm -rf build
mkdir -p build
cmake -S . -B build
cmake --build build
cd build && ctest --verbose --rerun-failed --output-on-failure
cd ..