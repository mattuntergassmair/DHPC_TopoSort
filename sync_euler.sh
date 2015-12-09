#!/bin/bash

MYABSREPOPATH=/home/matthias/polybox/CSE7/DesignHPC/project/DHPC_TopoSort
EULERREPOPATH=
MYETHUSERNAME=untergam


# copies from current machine to cluster and vice versa
#		-u	keep local files if newer than other file
#		-z	compress files

# send source code to cluster
echo "rsync -aiuz ${MYABSREPOPATH}/src/* ${MYETHUSERNAME}@euler.ethz.ch:dphpc_toposort/ --exclude '*.o' --exclude '*.exe' --exclude '*.xml' --exclude 'results/' --exclude 'graph_output/'"

# receive from cluster (include not strictly necessary, but more explicit)
echo "rsync -aiuz ${MYETHUSERNAME}@euler.ethz.ch:dphpc_toposort/ ${MYABSREPOPATH}/src/* --exclude '*.o' --exclude '*.exe' --include '*.xml' --include 'results/' --include 'graph_output/'"

exit 0

