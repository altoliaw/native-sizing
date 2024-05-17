# Including the libraries
source $(pwd)/Shells/iniParser.sh

# Definition of the associated array
declare -A result_dict
getIniValues $(pwd)/.Ini/globalCompiling.ini result_dict

echo "==========================="

for key in "${!result_dict[@]}"; do
    printf '%s = %s\n' "$key" "${result_dict[$key]}"
done

echo "==========================="

echo "${result_dict[compiler.CC]}"
echo $(iniParser_getIni $(pwd)/.Ini/globalCompiling.ini "compiler.CC")
echo $(iniParser_getIni $(pwd)/.Ini/globalCompiling.ini)

echo "==========================="


variableString="compiler.STD=-std=c++11;compiler.DETAILINFO=-g0;compiler.WALL=-Wall;compiler.CFLAGS=;compiler.LDFLAGS=;compiler.CC=g++;compiler.CMPOPT=-O0;compiler.FMSG=-fmessage-length=0 -pthread;"
variableName="compiler.DETAILINFO"
echo $(getVariableValue "$variableString" "$variableName")

echo "==========================="

variableName="compiler.LDFLAGS"
echo $(getVariableValue "$variableString" "$variableName")

echo "==========================="

variableName="compiler.FMSG"
echo $(getVariableValue "$variableString" "$variableName")

echo "==========================="