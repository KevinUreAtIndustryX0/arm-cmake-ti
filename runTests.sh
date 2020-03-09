#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
pushd $DIR
touch ./test/CMakeLists.txt
cd test
mkdir -p build
cd build
rm -rf bin
cmake .. -Wno-dev
if [ $? -ne 0 ]
then
  echo "cmake failed discovering changes and rebuilding build directory"
  exit 1
fi
cmake --build .
if [ $? -ne 0 ]
then
  echo "cmake failed to compile executables for testing"
  exit 1
fi
cd bin
if [ $? -ne 0 ]
then
  echo "bin was not made, no test executables compiled"
  exit 1
fi
fileCount=0
runCount=0
passCount=0
failCount=0
namesOfFailures=()
exitCode=0
for f in *; do
  ((fileCount++))
  output="$(script -q /dev/null bash -c './'$f'; exit $?' | tr -d '\r' | cat)"
  if [ ${PIPESTATUS[0]} -ne 0 ]
  then
    exitCode=1
  fi
  printf "\n$output\n"
  count=$(echo "$output" | awk "/RUN/ {print $2}" | wc -l)
  runCount=$((runCount+count))
  count=$(echo "$output" | awk "/OK/ {print $2}" | wc -l)
  passCount=$((passCount+count))
  count=$(echo "$output" | awk "/FAILED.*?\([0-9]+? ms\)/ {print $2}" | wc -l)
  failCount=$((failCount+count))
  matches=` echo "$output" | perl -ne 'print "$1\n" if s/.*?FAILED.*?\[m(?<cap>.*?)(,.*?)?\(\d+\sms\)/$+{cap}/'`
  matcharray=`echo "$matches"`
  # readarray -t matcharray <<< "$matches"
  # matches=($(echo "$output"| grep -oP "(?<=FAILED\s\s\]\s.\[m)([\w\.\s\/\(\)\=\-\<\>\d]*?)(?=(,.*?)?\s\(\d+?\sms\))"))
  for i in "${matcharray[@]}" 
  do
    namesOfFailures+=($i)
  done
done


plainCode="\e[0m"
redCode="\e[31m"
greenCode="\e[32m"
totalColor=$plainCode
passedColor=$plainCode
failedColor=$plainCode
if [ $failCount -gt 0 ]
then
  totalColor=$plainCode
  failedColor=$redCode
  passedColor=$plainCode
else
  totalColor=$greenCode
  failedColor=$plainCode
  passedColor=$greenCode
fi
printf "\n\n$plainCode------------------------------------------------------------$plainCode\n"
printf "${totalColor}Total Tests Run: ${runCount}${plainCode}\n"
printf "${passedColor}Total Passes: ${passCount}${plainCode}\n"
printf "${failedColor}Total Failures: ${failCount}${plainCode}\n"
if [ $failCount -gt 0 ]
then
  printf "${failedColor}Failed tests:${plainCode}\n"
  for i in "${namesOfFailures[@]}" 
  do
    printf "${failedColor} - $i${plainCode}\n"
  done
fi
printf "\n"
popd >/dev/null

exit $exitCode
