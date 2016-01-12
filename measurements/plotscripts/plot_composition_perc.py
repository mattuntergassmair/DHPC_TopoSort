import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import colortableau as ct
from collections import defaultdict


plt.style.use('ggplot')
plotdir = "plots";
db = sqlite3.connect('measurements.db')

query = db.cursor()

def getPerc(where):

	# subquery = "(SELECT threads.measurement_id AS mid, AVG(timings.value) AS t, timings.name AS category FROM timings INNER JOIN threads ON threads.id=timings.thread_id GROUP BY threads.id, timings.name) AS timingdetail"
	subquery = "(SELECT threads.measurement_id AS mid, SUM(timings.value) AS t, timings.name AS category FROM timings INNER JOIN threads ON threads.id=timings.thread_id GROUP BY threads.id, timings.name) AS timingdetail"

	querystring = "SELECT timingdetail.category AS cat, AVG(timingdetail.t) AS t_det, AVG(m.total_time) AS t_tot FROM measurements AS m INNER JOIN  " + subquery + " ON m.id=timingdetail.mid WHERE " + where + " AND timingdetail.t>0 GROUP BY m.number_of_threads, timingdetail.category ORDER BY m.number_of_threads, timingdetail.category;"
	# print querystring
	query.execute(querystring)
	data = np.array(query.fetchall())
	return data

def plotPercGraph(algo,graphtype,size,optim,hostnamelike):
	
	fixedwhere = "enable_analysis=1 AND graph_type='{0}' AND debug=0 AND verbose=0 AND optimistic={1} AND graph_num_nodes={2} AND processors>=number_of_threads AND hostname LIKE '{3}' AND algorithm='{4}'".format(graphtype,optim,size,hostnamelike,algo)

	query.execute("SELECT number_of_threads FROM measurements WHERE " + fixedwhere + " GROUP BY number_of_threads")
	number_of_threads = np.array(query.fetchall())

	print "Threads:\n", number_of_threads

	if len(number_of_threads) == 0:
		print "\nNo Data for\n", fixedwhere
		return None

	percentagedict = defaultdict(list)

	for nt in number_of_threads:
		where = fixedwhere + " AND number_of_threads={0}".format(nt[0])
		data = getPerc(where)
		# print "Data:\n", data

		for i in range(0,data.shape[0]):
			percentagedict[data[i][0]].append(data[i][1].astype(float)/data[i][2].astype(float))

	orderedlist = sorted(percentagedict.items())
	cats = [i[0] for i in orderedlist]
	timefrac = np.array([i[1] for i in orderedlist])

	# Account for time that is not measured
	cats.append(u'other')
	measuredfraction = np.sum(timefrac,axis=0)
	missingfraction = np.ones(measuredfraction.shape) - measuredfraction
	timeperc = 100 * np.vstack([timefrac,missingfraction])

	fig = plt.figure()
	ax = fig.add_subplot(111)

	fontsize_title=12
	fontsize_label=14

	ax.set_title('Percentage composition of runtime\nalgorithm={0}, graphtype={1}, graphsize={2}'.format(algo,graphtype,size), fontsize=fontsize_title)
	ax.set_ylabel('Percentage of total runtime (%)', fontsize=fontsize_label)
	ax.set_xlabel('number of OMP threads', fontsize=fontsize_label)
	ax.tick_params(top='off', right='off', length=4, width=1)

	# Creating stacked area chart
	print "\n\n\n\n\n\n\n\n\n=====================================\nPLOTTING\n"
	print "NT: ", number_of_threads, " s-", number_of_threads.size
	print "TC: ", timeperc, " s-", timeperc.size
	ax.stackplot(number_of_threads.flat,timeperc,edgecolor='white',colors=ct.myBGcolors)
	
	plt.xticks(number_of_threads)
	ax.set_xticklabels(number_of_threads.astype(int).flat);


	ax.margins(0, 0) # Set margins to avoid "whitespace"

	lgnd = []
	lbl = []

	for i in range(0,len(cats)):
		col = ct.getBGcolor(i)
		lgnd.append(plt.Rectangle((0,0),1,1,fc=col))
		lbl.append(cats[i])

	plt.legend(lgnd,lbl)

	filename = plotdir + "/" + "timepercentage_{0}_gt{1}_s{2}_opt{3}".format(algo,graphtype,size,optim) + ".pdf";
	plt.savefig(filename,format='pdf',bbox_inches='tight',dpi=1000)

	plt.show();

	print "Done - File written to " + filename


# hostname starting with e (euler)
plotPercGraph('locallist','SOFTWARE',1000000,0,'e%',)
plotPercGraph('locallist','RANDOMLIN',1000000,0,'e%',) 
#plotPercGraph('locallist','CHAIN',1000000,0,'e%',) 
#plotPercGraph('locallist','MULTICHAIN',1000000,0,'e%') 
plotPercGraph('bitset','SOFTWARE',1000000,1,'e%',) 
plotPercGraph('bitset','RANDOMLIN',1000000,1,'e%',) 
#plotPercGraph('bitset','CHAIN',1000000,1,'e%',) 
#plotPercGraph('bitset','MULTICHAIN',1000000,1,'e%') 




db.close();
