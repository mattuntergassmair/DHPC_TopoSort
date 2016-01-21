import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import helper
import addline



def plotWeakScaling(allsize=1000000,allgraphtype='SOFTWARE',alladditionalwhere=' AND total_time>0 ',suffix='',basesize=100000):
	fig = plt.figure()
	ax = fig.add_subplot(111)
	addline.addWeakScaling(axis=ax, algorithm='locallist', optimistic='1', size=basesize, graphtype=allgraphtype, hostnamelike='e%',colorindex=0,linelabel='Scatter-Gather')
	#addline.addWeakScaling(axis=ax, algorithm='dynamic_nobarrier', optimistic='1', size=basesize, graphtype=allgraphtype, hostnamelike='e%',colorindex=0,linelabel='DynNoBarrier')
	addline.addWeakScaling(axis=ax, algorithm='bitset', optimistic='1', size=basesize, graphtype=allgraphtype, hostnamelike='e%',colorindex=1,linelabel='Node-Lookup Atomic')
	addline.addWeakScaling(axis=ax, algorithm='worksteal', optimistic='1', size=basesize, graphtype=allgraphtype, hostnamelike='e%',colorindex=2,linelabel='Worksteal CAS')
	ax.plot([1,24],[1,1],'r--')
	ax.legend(loc='upper right')
	ax.minorticks_on()
	
	filename = helper.plotdir + 'weakscaling_gt' + allgraphtype + '_n' + str(allsize)

	plt.title('Speedup vs. Number of Threads',fontsize=helper.fontsize_label)
	if(suffix==''):
		plt.suptitle('Weak Scaling for ' + allgraphtype + ' Graph (' + str(allsize) + 'nodes)',fontsize=helper.fontsize_title)
	else:
		plt.suptitle('Weak Scaling for ' + allgraphtype + ' Graph (' + str(allsize) + 'nodes, ' + suffix + ')',fontsize=helper.fontsize_title)
		filename = filename + '_' + suffix

	filename = filename + '.pdf'
	
	plt.savefig(filename, format='pdf',bbox_inches='tight',dpi=1000)
	print "File written to:\t", filename
	if(helper.show):
		plt.show()

############################################################
# Call Plotting functions
############################################################
plotWeakScaling(allsize=1000000,allgraphtype='SOFTWARE') # software graph
plotWeakScaling(allsize=1000000,allgraphtype='RANDOMLIN8',suffix='deg8') # degree 8
plotWeakScaling(allsize=1000000,allgraphtype='RANDOMLIN16',suffix='deg16') # degree 16
plotWeakScaling(allsize=1000000,allgraphtype='RANDOMLIN32',suffix='deg32') # degree 32
#plotWeakScaling(allsize=1000000,allgraphtype='RANDOMLIN',alladditionalwhere=' AND graph_num_edges=63995794',suffix='deg64') # degree 64
