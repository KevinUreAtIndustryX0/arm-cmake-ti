#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR

verify=""
restart="-r 0"
while [[ "$#" -gt 0 ]]
do
    case $1 in
        -v|--verify)
            verify="-v"
            ;;
        -n|--no-restart)
            restart=""
            ;;
    esac
    shift
done

plainCode="\e[0m"
redCode="\e[31m"

if [[ -z "$UNIFLASH_PATH" ]]
then
    printf "${redCode}UNIFLASH_PATH environment variable not set, aborting.${plainCode}\n"
    exit 1
fi
echo "Uniflash path: $UNIFLASH_PATH"
if [[ ! -d ./build ]]
then
    printf "${redCode}No build directory found, aborting.${plainCode}\n"
    exit 1
fi
binfile=`find ./build -name "*.bin" -maxdepth 1 -type f`
if [[ -z "$binfile"  ]]
then
    printf "${redCode}No bin file found in build, aborting..${plainCode}\n"
    exit 1
fi
echo "Found Bin File: $binfile"
configfile=`find ./targetConfigs -name "*.ccxml" -maxdepth 1 -type f`
if [[ -z "configfile" ]]
then
    printf "${redCode}No target config ccxml found, aborting.${plainCode}\n"
    exit 1
fi
echo "Found Target Config File: $configfile"

echo "Flashing ${binfile} to device..."
${UNIFLASH_PATH}/dslite.sh --config=${configfile} ${verify} ${restart} -f ${binfile}