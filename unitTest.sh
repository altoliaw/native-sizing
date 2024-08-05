# Dependency porcess
# Traversal of globalDependencies.json
local Vendors=Vendors
dependenciesTraversal $(pwd)/Settings/.Json/globalDependencies.json $(pwd)/$Vendors/.$Vendors.json

# Cmake process
rm -rf build
mkdir -p build
cmake -S . -B build
cmake --build build
cd build && ctest --verbose --rerun-failed --output-on-failure
cd ..