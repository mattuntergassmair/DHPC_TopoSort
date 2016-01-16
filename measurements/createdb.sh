#!/bin/bash

# This is just a temporary helper script - should be made nicer

cd TopoSortParser
if [[ -z $1 && $1 != "--noDelete" ]]; then
	ant
else
	ant run -Dargs="--noDelete"
fi
exit 0
