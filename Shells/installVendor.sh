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
#  * Obtaining the element from the .json file
#  *
#  * @param $0 The function called by users
#  * @param $1 The contents for searching
#  * @param $2 The syntax which users search
#  * @return The block which users search
#  */
function searchElement() {
    local content="$1"
    local seachedSyntax="$2"
    echo $( echo "$content" | jq "$seachedSyntax" )
}

# /**
#  * Public Caller, generation and initialization of the .json file (defined in the "Vendors" folder)
#  *
#  * @param $0 The function called by users
#  * @param $1 The dependent file name of the .json file (defined in the "Vendors" folder)
#  */
function vendorDependenciesInitailization() {
    local vendorFilePath=$1
    # Determining the existence of the file; if the file does not exist, please generate the
    # file and put the basic syntax into the .json file
    if [ ! -f "$vendorFilePath" ]; then
        touch "$vendorFilePath" # A file for maintaining the modules in the folder, Vendors
        jq -n '{"dependencies": []}' > "$vendorFilePath"
    fi
}

# /**
#  * Public Caller, calculating the length in the array
#  *
#  * @param $0 The function called by users
#  * @param $1 The content of an array
#  * @return The number of the array
#  */
function dependenciesTraversalLength() {
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
    # To ensure that the folder and the files by using the command in the Makefile in the root of the project
    if [ ! -f /tmp/foo.txt ]; then
        make vendor
    fi

    # Obtaining the file name
    local jsonFile="$1"
    local vendorJsonFile="$2"

    #Reading the content from the .json file
    local jsonContent="$(<$jsonFile)"
    local dependencies=$(echo $jsonContent | jq '.dependencies') # Obtaining the array from the attribute, "dependencies"



    # Obtaining the length of the arrary
    local dependenciesLength=$(dependenciesTraversalLength "$dependencies")

    # Traversal of all elements from the array which obtains from the attribute, "dependencies"
    for ((i = 0; i < $dependenciesLength; i++)); do
        local name=$(echo $dependencies | jq ".[$i].name")
        local download=$(echo $dependencies | jq ".[$i].download")
		local command=$(echo $dependencies | jq ".[$i].command")
		local includes=$(echo $dependencies | jq ".[$i].includes")
		local libs=$(echo $dependencies | jq ".[$i].libs")
		local reference=$(echo $dependencies | jq ".[$i].reference")
		local remove=$(echo $dependencies | jq ".[$i].remove")
        # echo $name
        # echo $download
        # echo $command
        # echo $includes
        # echo $libs
        # echo $reference
        # echo $remove

        # The contents from the attribute, .dependencies, in the Vendors/.Vendors.json
        local vendorContent=$(echo "$(<$vendorJsonFile)" | jq '.dependencies')

        # Verifying if the element in .Json/globalDependencies is defined in "Vendors/.Vendors.json";
        # if the element exists, the return value will be obtained the block
        local block=$(searchElement "$vendorContent" ".[] | select(.name == "$name")")

        # If the returned value is null, ...
        if [ -z "$block" ]; then
            echo "$i not"
        else # If the returned value is not null, ...
            echo "$i"
        fi


    done

}