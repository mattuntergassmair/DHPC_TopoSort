import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import helper



def addWeakScaling(axis, algorithm, optimistic, size, graphtype='SOFTWARE', hostnamelike='e%',colorindex=0,linelabel='nolabel'):

	fixedwhere = "enable_analysis=0 AND debug=0 AND verbose=0 AND processors>=number_of_threads AND algorithm='{0}' AND optimistic={1} AND graph_type='{2}' AND hostname LIKE '{3}' AND graph_num_nodes={4}*number_of_threads".format(algorithm,optimistic,graphtype,hostnamelike,size)

	numthreads = helper.getData('number_of_threads', fixedwhere + ' GROUP BY number_of_threads')

	print(numthreads)

	avgtimings = []

	if (np.size(numthreads)==0):
		return

	for nt in numthreads.flat:
		# print "NUMTHREADS = ", nt
		where = fixedwhere + ' AND number_of_threads={0}'.format(nt)
		timings = helper.getData('total_time',where)
		avgtimings.append(np.mean(timings))
		speedups = avgtimings[0]/timings

		violin_parts = ax.violinplot(speedups,[nt],widths=0.8)

		for pc in violin_parts['bodies']:
			pc.set_color(helper.getFGcolor(colorindex))

	speedup = avgtimings[0]/avgtimings

	ax.plot(numthreads,speedup,'D-',markersize=4,linewidth=1,color=helper.getFGcolor(colorindex),label=linelabel) # connecting dots







############################################################
# Set up Plot and add scaling lines 
############################################################

##########################
# Software Graph
fig = plt.figure()
ax = fig.add_subplot(111)

basesize=100000

addWeakScaling(axis=ax, algorithm='locallist', optimistic='0', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=0,linelabel='Globallist')
addWeakScaling(axis=ax, algorithm='bitset', optimistic='1', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=1,linelabel='Bitset Opt')
addWeakScaling(axis=ax, algorithm='bitset', optimistic='0', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=4,linelabel='Bitset NoOpt')
addWeakScaling(axis=ax, algorithm='worksteal', optimistic='1', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=2,linelabel='Worksteal Opt')
addWeakScaling(axis=ax, algorithm='worksteal', optimistic='0', size=basesize, graphtype='SOFTWARE', hostnamelike='e%',colorindex=5,linelabel='Worksteal NoOpt')

ax.plot([1,24],[1,1],'r--')

# handles, labels = ax.get_legend_handles_labels()
# ax.legend(handles,labels)
ax.legend()

plt.title('Weak Scaling',fontsize=helper.fontsize_title)
plt.xlabel('Number of threads',fontsize=helper.fontsize_label)
plt.ylabel('Speedup',fontsize=helper.fontsize_label)

ax.minorticks_on()

plt.savefig(helper.plotdir + 'weakscaling_gtSOFTWARE.pdf',format='pdf',bbox_inches='tight',dpi=1000)
plt.show()
