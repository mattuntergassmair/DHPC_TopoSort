import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import helper
import addline

def plotStrongScaling(allsize=1000000,allgraphtype='SOFTWARE',alladditionalwhere=' AND total_time>0 ',suffix=''):
	fig = plt.figure()
	ax = fig.add_subplot(111)
	addline.addStrongScaling(axis=ax, algorithm='locallist', optimistic='0', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=0,linelabel='Globallist',additionalwhere=alladditionalwhere)
	addline.addStrongScaling(axis=ax, algorithm='dynamic_nobarrier', optimistic='1', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=0,linelabel='DynNoBarrier',additionalwhere=alladditionalwhere)
	addline.addStrongScaling(axis=ax, algorithm='bitset', optimistic='1', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=1,linelabel='Bitset Opt',additionalwhere=alladditionalwhere)
	addline.addStrongScaling(axis=ax, algorithm='bitset', optimistic='0', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=4,linelabel='Bitset NoOpt',additionalwhere=alladditionalwhere)
	addline.addStrongScaling(axis=ax, algorithm='worksteal', optimistic='1', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=2,linelabel='Worksteal Opt',additionalwhere=alladditionalwhere)
	addline.addStrongScaling(axis=ax, algorithm='worksteal', optimistic='0', size=allsize, graphtype=allgraphtype, hostnamelike='e%',colorindex=5,linelabel='Worksteal NoOpt',additionalwhere=alladditionalwhere)
	ax.plot(range(1,24),range(1,24),'r--') # ideal scaling
	ax.legend(loc='upper left')
	ax.minorticks_on()
	
	filename = helper.plotdir + 'strongscaling_gt' + allgraphtype + '_n' + str(allsize)

	plt.title('Speedup vs. Number of Threads',fontsize=helper.fontsize_label)
	if(suffix==''):
		plt.suptitle('Strong Scaling for ' + allgraphtype + ' Graph (' + str(allsize) + 'nodes)',fontsize=helper.fontsize_title)
	else:
		plt.suptitle('Strong Scaling for ' + allgraphtype + ' Graph (' + str(allsize) + 'nodes, ' + suffix + ')',fontsize=helper.fontsize_title)
		filename = filename + '_' + suffix

	filename = filename + '.pdf'
	
	plt.savefig(filename, format='pdf',bbox_inches='tight',dpi=1000)
	print "File written to:\t", filename
	if(helper.show):
		plt.show()



############################################################
# Call Plotting functions 
############################################################
plotStrongScaling(allsize=1000000,allgraphtype='SOFTWARE') # software graph
plotStrongScaling(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=7999910',suffix='deg8') # degree 8
plotStrongScaling(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=15999722',suffix='deg16') # degree 16
plotStrongScaling(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=31998947',suffix='deg32') # degree 32
plotStrongScaling(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=63995794',suffix='deg64') # degree 64



#############################################################
# Compare different graph types
#############################################################
fig = plt.figure()
ax = fig.add_subplot(111)
allalgorithm = 'bitset'
allsize = 1000000
addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='MULTICHAIN100', hostnamelike='e%',colorindex=1,markertype='o-',linelabel='MultiChain 100')
addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='MULTICHAIN10000', hostnamelike='e%',colorindex=1,markertype='p-',linelabel='MultiChain 1000')
# addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='RANDOMLIN8', hostnamelike='e%',colorindex=1,markertype='s-',linelabel='Random (Deg8)')
# addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='RANDOMLIN16', hostnamelike='e%',colorindex=1,markertype='v-',linelabel='Random (Deg16)')
# addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='RANDOMLIN32', hostnamelike='e%',colorindex=1,markertype='D-',linelabel='Random (Deg32)')
addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='RANDOMLIN', hostnamelike='e%',colorindex=1,markertype='s-',linelabel='Random (Deg8)',additionalwhere=' AND graph_num_edges=7999910')
addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='RANDOMLIN', hostnamelike='e%',colorindex=1,markertype='D-',linelabel='Random (Deg16)',additionalwhere=' AND graph_num_edges=15999722')
addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='RANDOMLIN', hostnamelike='e%',colorindex=1,markertype='v-',linelabel='Random (Deg32)',additionalwhere=' AND graph_num_edges=31998947')
addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='RANDOMLIN', hostnamelike='e%',colorindex=1,markertype='>-',linelabel='Random (Deg64)',additionalwhere=' AND graph_num_edges=63995794')
addline.addStrongScaling(axis=ax, algorithm=allalgorithm, optimistic='1', size=allsize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=1,markertype='*-',linelabel='Software')
ax.plot(range(1,24),range(1,24),'r--') # ideal scaling
ax.legend(loc='upper left')
ax.minorticks_on()


filename = helper.plotdir + 'strongscaling_gtALL_n' + str(allsize)

plt.suptitle('Strong Scaling for different graph types',fontsize=helper.fontsize_title)
plt.title('Speedup vs. Number of Threads',fontsize=helper.fontsize_label)

filename = filename + '.pdf'

plt.savefig(filename, format='pdf',bbox_inches='tight',dpi=1000)
print "File written to:\t", filename
if(helper.show):
	plt.show()
