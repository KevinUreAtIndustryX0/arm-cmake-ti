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
if [[ -z "$UNIFLASH_PATH" ]]
then
    echo '$UNIFLASH_PATH environment variable not set, aborting.'
    exit 1
fi
echo "Uniflash path: $UNIFLASH_PATH"
if [[ ! -d ./build ]]
then
    echo "No build directory found, aborting."
    exit 1
fi
binfile=`find ./build -name "*.bin" -maxdepth 1 -type f`
if [[ -z binfile  ]]
then
    echo "No bin file found in build, aborting."
    exit 1
fi
echo "Found Bin File: $binfile"
configfile=`find ./targetConfigs -name "*.ccxml" -maxdepth 1 -type f`
if [[ -z configfile ]]
then
    echo "No target config ccxml found, aborting."
    exit 1
fi
echo "Found Bin File: $configfile"
# do the thing
${UNIFLASH_PATH}/dslite.sh --config=${configfile} ${verify} ${restart}