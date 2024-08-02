#!/bin/bash
# /**
#  * The shell script is a pre-implementation for installation of the dependencies before C++ compiling.
#  *
#  * @author Nick
#  * @Date 2024/08/02
#  */

# /**
#  * Traversal dependencies from the input json file; in each dependency, the shell script will
#  * execute the command and register information in the file, .Vendors.json in the folder, Vendors;
#  * to execute the function, please call the function from the root of the project (i.e., that implies 
#  * the current working directory is the root of the project)
#  *
#  * @param $0 The function called by users
#  * @param $1 The dependent file name of the .json file
#  */
function dependenciesTraversal() {
    local jsonFile="$1"

    # Reading the attribute, "dependencies" in the json file, and obtaing the array;
	# in the array, each item is an object
    local dependencies
    dependencies=$(jq -c '.dependencies[]' "$jsonFile")

    # Traversal of the items from the array, dependencies
    for element in $dependencies; do
        local name
        local download
        local buildCommand
        local includes
        local libs
        local reference
        local removeCommand

        name=$(echo "$element" | jq -r '.name')
        download=$(echo "$element" | jq -r '.download')
        buildCommand=$(echo "$element" | jq -r '.command')
        includes=$(echo "$element" | jq -r '.includes | @csv' | sed 's/"//g')
        libs=$(echo "$element" | jq -r '.libs | @csv' | sed 's/"//g')
        reference=$(echo "$element" | jq -r '.reference')
        removeCommand=$(echo "$element" | jq -r '.remove')

		# Executing the download process
        #eval "$download"

        # Builing with the command
        #eval "$buildCommand"

        # Removing the
        #eval "$removeCommand"
    done
}


#$(process_dependencies "dependencies.json")
