#!/bin/bash


################################################################################
# Find files matching certain pattern.
#
# Parameter list:
#   1. File Hierarchy (Optional)
#   2. Parttern String
################################################################################


# parameter assignment
if [ -z $2 ] ; then
    cwd=.   # file hierarchy
    ptn=$1  # pattern string
else
    cwd=$1  # file hierarchy
    ptn=$2  # pattern string
fi


# file content match
set -x
grep --colour=auto --line-number --recursive --include *.[ch] $ptn $cwd


# # walk file hierarchy
# find_list=`find $cwd -iname *.[ch] -type f 2>/dev/null`
# grep_list=`grep -l $ptn $find_list`


# # file content match
# for file in $grep_list ; do
#   echo "============================================="
#   echo $file":"
#   grep --colour=auto -n $ptn $file 2>/dev/null
#   echo "============================================="
# done
