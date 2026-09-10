#!/bin/bash
# Non-POSIX compliant
# Needs comm, cut, sed, wc, xmlstarlet

# 1. Verify inputs
[ -z "$1" ] && echo "Missing positional parameter <file.xml>" && exit 1
[ -z "$2" ] && echo "Missing positional parameter <threshold>" && exit 1
if [ ! -s "$1" ]; then
 echo File "$1" does not exist
 exit 1
fi
exit_status="SUCCESS"
exit_code=0

# 2. Get functions from hpp/cpp files
xmlstarlet sel -t -m "//compound[@kind='file' and contains(name, '.hpp')]/member[@kind='function']" -v "concat(../name,':',name)" -n "$1" | sort -u > hpp.funcs
xmlstarlet sel -t -m "//compound[@kind='file' and contains(name, '.cpp')]/member[@kind='function']" -v "concat(../name,':',name)" -n "$1" | sort -u > cpp.funcs

# 3. Get functions from cpp not in hpp
sed -e 's/hpp/ext/g' hpp.funcs > hpp.processed
sed -e 's/cpp/ext/g' cpp.funcs > cpp.processed
# Find lines of cpp.processed not in hpp.processed
comm -13 hpp.processed cpp.processed > diff.funcs
sed -i -e 's/ext/cpp/g' diff.funcs

# 4. Number of documented funcs * 100 / number of funcs
nFuncsHpp=$(wc -l hpp.funcs | cut -d ' ' -f1) 
nFuncsCpp=$(wc -l cpp.funcs | cut -d ' ' -f1)
percentage="$((nFuncsHpp * 100 / nFuncsCpp))"

# 5. Output coverage status, undocumented functions if any then exit
if [ "$percentage" -lt "$2" ]; then
  exit_status="FAILED"
  exit_code=1
fi
if [ "$percentage" -ne 100 ]; then
  echo Undocumented functions:
  cat diff.funcs
fi
rm -f hpp.funcs cpp.funcs hpp.processed cpp.processed diff.funcs
echo "$exit_status: $percentage% of code is documented (threshold: $2)"
exit "$exit_code"
