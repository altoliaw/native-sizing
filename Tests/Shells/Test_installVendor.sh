#!/bin/bash
# All tests are executed from the project root (i.e., the current working directory is the project root)
# Including the libraries

source $(pwd)/Shells/installVendor.sh
echo "==========================="

# Obtaining the length from the array
dependencies='[ { "name": "cJson", "download": "git clone https://github.com/DaveGamble/cJSON.git cJson", "command": "cd cJson && make", "includes": [], "libs": [], "reference": "https://github.com/DaveGamble/cJSON", "remove": "cd ../ && rm -rf cJson" }, { "name": "cJson2", "download": "git clone https://github.com/DaveGamble/cJSON.git cJson", "command": "cd cJson && make", "includes": [], "libs": [], "reference": "https://github.com/DaveGamble/cJSON", "remove": "cd ../ && rm -rf cJson" } ]'
echo $(obtainArrayLength "$dependencies")

echo "==========================="
# Discovering the block where the name is equal to the term which user specified
searchElement "$dependencies" '.[] | select(.name == "cJson")' ''
searchElement "$dependencies" '.[] | select(.name == "cJson3")' '-r'

echo "==========================="
# Intiailzation the file, .Vendors.json, in the folder, Vendors
Vendors=Vendors
vendorDependenciesInitailization "$Vendors/.$Vendors.json"

echo "==========================="
# Traversal of globalDependencies.json
dependenciesTraversal $(pwd)/Settings/.Json/globalDependencies.json $(pwd)/$Vendors/.$Vendors.json

