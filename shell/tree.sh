#!/bin/bash


################################################################################
# List contents of directories in a tree-like format.
# 
# Parameter list:
#   1. File Hierarchy (Optional)
################################################################################


# initialise macro
tab_cnt=0
if [ -z $1 ] ; then
    path=$PWD
else
    path=$1
fi


# print function
function print {
    # assign parameters
    local item=$1
    local tab_cnt=$2

    # print item
    for _ in `seq 1 $tab_cnt` ; do
        printf "    "
    done
    echo $item
}


# list contents of directories
function tree_func {
    # assign parameters
    local path=$1
    local tab_cnt=$[tab_cnt + 1]

    # create child Shell
    (
        # temporarily change directory
        cd $path

        # walk directory
        list=`ls -A . 2>/dev/null`
        for item in $list ; do
            # recursive DFS
            if [ -L $item ] ; then
                item="$item -> `readlink $item`"
                print "$item" $tab_cnt
            elif [ -d $item ] ; then
                print $item $tab_cnt
                tree_func $item
            else
                print $item $tab_cnt
            fi
        done
    )
}


# start listing
echo $path
tree_func $path
