#!/bin/bash
# /**
#  * The shell script is a pre-implementation for installation of the dependencies before C++ compiling.
#  *
#  * @author Nick
#  * @Date 2024/08/02
#  *
#  * @note [2024/08/02] The structure of the .Vendors.json is shown as below.
#  * {
#  *   "dependencies": [
#  *     {
#  *       "name": "cJson",
#  *       "includes": [
#  *         "cJson.h" ...
#  *       ],
#  *       "libs": [
#  *         "cJson.a" ...
#  *       ]
#  *     }, ...
#  *   ]
#  * }
#  */

# /**
#  * Obtaining the element from the .json file and the temporary file
#  *
#  * @param $0 The function called by users
#  * @param $1 The contents for searching
#  * @param $2 The syntax which users search
#  * @param $3 The parameter for jq
#  * @return The block which users search
#  */
function searchElement() {
    local content="$1"
    local searchedSyntax="$2"
    local jqParameter="$3"

    # If the "jqParameter" is null, ...
    if [ -z "$jqParameter" ]; then
        echo "$content" | jq "$searchedSyntax"
    else # If the "jqParameter" is not null, ...
        echo "$content" | jq "$jqParameter" "$searchedSyntax"
    fi
}

# /**
#  * Public Caller, vendor generation and initialization of the .json file (defined in the "Vendors" folder)
#  *
#  * @param $0 The function called by users
#  * @param $1 The dependent file name of the .json file (defined in the "Vendors" folder)
#  */
function vendorDependenciesInitailization() {
    local vendorFilePath=$1
    local vendorFolder=$(echo "$vendorFilePath" | sed 's|/[^/]*$||')

    mkdir -p ${vendorFolder}          # Creating the folder for the third party software
    mkdir -p ${vendorFolder}/Libs     # Creating the folder for the static/dynamic libraries
    mkdir -p ${vendorFolder}/Includes # Creating the folder for the headers file

    # Determining the existence of the file; if the file does not exist, please generate the
    # file and put the basic syntax into the .json file
    if [ ! -f "$vendorFilePath" ]; then
        touch "$vendorFilePath" # A file for maintaining the modules in the folder, Vendors
        jq -n '{"dependencies": []}' >"$vendorFilePath"
    fi

    # Determining the existence of the file; if the file exists, please generate the
    # file and put the basic syntax into the .json file
    if [ -f "$vendorFilePath.tmp" ]; then
        rm -rf "$vendorFilePath.tmp"
    fi
    # touch "$vendorFilePath.tmp" # A file for maintaining the modules in the folder, Vendors
    # jq -n '{"dependencies": []}' >"$vendorFilePath.tmp"
}

# /**
#  * Public Caller, calculating the length in the array
#  *
#  * @param $0 The function called by users
#  * @param $1 The content of an array
#  * @return The number of the array
#  */
function obtainArrayLength() {
    local jsonArray="$1"
    echo $jsonArray | jq '. | length'
}

# /**
#  * Public Caller, traversal dependencies from the input json file; in each dependency, the shell script will
#  * execute the command and register information in the file, .Vendors.json in the folder, Vendors;
#  * to execute the function, please call the function from the root of the project (i.e., that implies
#  * the current working directory is the root of the project)
#  *
#  * @param $0 The function called by users
#  * @param $1 The dependent file name of the .json file (defined in the "Setting/.Json" folder)
#  * @param $2 The dependent file name of the .json file (defined in the "Vendors" folder)
#  */
function dependenciesTraversal() {
    # Obtaining the file name
    local jsonFile="$1"
    local vendorJsonFile="$2"
    
    # To ensure that the folder and the files by using the command in the "Makefile" in the root of the project
    if [ ! -f $vendorJsonFile ]; then
        # make createVendor
        vendorDependenciesInitailization "$vendorJsonFile"
    fi

    #Reading the content from the .json file
    local jsonContent="$(<$jsonFile)"
    local dependencies=$(echo $jsonContent | jq '.dependencies') # Obtaining the array from the attribute, "dependencies"

    # Obtaining the length of the arrary
    local dependenciesLength=$(obtainArrayLength "$dependencies")
    local vendorContent=$(echo "$(<$vendorJsonFile)" | jq '.dependencies')


    # Traversal of all elements from the array which obtains from the attribute, "dependencies"; the time complexity here is
    # O(n^{3}); however, the the editor guessed that the number of n is small; as a result, the time complexity is okay in this case
    for ((i = 0; i < $dependenciesLength; i++)); do
        # The vendor has not been installed; installing the vendor automatically
        local name=$(searchElement "$dependencies" ".[$i].name" "")
        local download=$(searchElement "$dependencies" ".[$i].download" "")
        local command=$(searchElement "$dependencies" ".[$i].command" "")
        local includes=$(searchElement "$dependencies" ".[$i].includes" "-c")
        local libs=$(searchElement "$dependencies" ".[$i].libs" "-c")
        local reference=$(searchElement "$dependencies" ".[$i].reference" "")
        local remove=$(searchElement "$dependencies" ".[$i].remove" "")

        # The contents from the attribute, .dependencies, in the Vendors/.Vendors.json
        # Verifying if the element in .Json/globalDependencies is defined in "Vendors/.Vendors.json";
        # if the element exists, the return value will be obtained the block
        local block=$(searchElement "$vendorContent" ".[] | select(.name == "$name")" "")

        # If the returned value is null, ...
        if [ -z "$block" ]; then
            # Printing the message
            echo "Vendor, $name is installing."

            # Removing the double quotes from the string because the string is equal to the command
            local folderName=$(echo "$name" | sed 's/"//g')
            download=$(echo "$download" | sed 's/"//g')
            command=$(echo "$command" | sed 's/"//g')
            remove=$(echo "$remove" | sed 's/"//g')

            # Executing the download, command, installation and removing the download at last
            cd $(dirname "$vendorJsonFile") &&
                eval "$remove" &&
                eval "$download $folderName" &&
                cd "$folderName" &&
                eval "$command" &&
                cd ../ && eval "$remove" && \
                cd ../ # Leaving the folder, Vendors

            # Registering the information into the temporary file
            jq ".dependencies += [{\"name\": $name, \"includes\": $includes, \"libs\": $libs, \"reference\":$reference}]" "$vendorJsonFile" >"$vendorJsonFile.tmp"
            cp "$vendorJsonFile.tmp" "$vendorJsonFile"
        else # If the returned value is not null, ...
            # The vendor has been installed; printing the result
            echo "Vendor, $name has been installed."
        fi
    done
    rm -rf "$vendorJsonFile.tmp"
}
