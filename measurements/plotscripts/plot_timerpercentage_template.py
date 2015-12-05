import numpy as np
import matplotlib.pyplot as plt
import sqlite3
import glob
import re
import colortableau as ct


import sqlite3 as db
plt.style.use('ggplot')
plotdir = "plots";
db = sqlite3.connect('measurements.db')

graphsize = 28

def plotTimerpercentage(omp_threads):
	with db:    
		
		query = db.cursor()    
		querystring = "SELECT mpi_ranks, AVG(t_norm), AVG(t_pre), AVG(t_e0), AVG(t_comm), AVG(t_ep), COUNT(*) FROM results WHERE graph_size={0} AND graph_type='rand' AND omp_threads={1} AND m=2 AND n_nodes=1 AND vec_flag=1 GROUP BY mpi_ranks ORDER BY mpi_ranks;".format(graphsize,omp_threads)
		query.execute(querystring)
		data = np.array(query.fetchall())

	ranks = data[:,0]
	timings = data[:,1:-1]

	# using .astype(float) to avoid integer division
	tpercent = timings / timings.sum(axis=1).astype(float)[:,None] * 100 

	fig = plt.figure()
	ax = fig.add_subplot(111)

	fontsize_title=12
	fontsize_label=14

	ax.set_title('Percentage composition of runtime\nGraph Size={0}, OMP_THREADS={1}'.format(graphsize, omp_threads), fontsize=fontsize_title)
	ax.set_ylabel('Percentage of total runtime (%)', fontsize=fontsize_label)
	ax.set_xlabel('MPI ranks', fontsize=fontsize_label)
	ax.tick_params(top='off', right='off', length=4, width=1)


	# Creating stacked area chart
	ax.stackplot(range(0,ranks.shape[0]),tpercent.T,edgecolor='white',colors=ct.tableau20)
	
	plt.xticks(np.arange(0, ranks.shape[0], 1))
	ax.set_xticklabels(ranks.astype(int))

	ax.margins(0, 0) # Set margins to avoid "whitespace"

	pt1 = plt.Rectangle((0, 0), 1, 1, fc=ct.tableau20[0])
	pt2 = plt.Rectangle((0, 0), 1, 1, fc=ct.tableau20[1])
	pt3 = plt.Rectangle((0, 0), 1, 1, fc=ct.tableau20[2])
	pt4 = plt.Rectangle((0, 0), 1, 1, fc=ct.tableau20[3])
	pt5 = plt.Rectangle((0, 0), 1, 1, fc=ct.tableau20[4])

	plt.legend([pt1,pt2,pt3,pt4,pt5], ["T_NORM","T_PRE","T_E0","T_COMM","T_EP"])

	filename = plotdir + "/" + "timepercentage_size{0}_threads{1}".format(graphsize,omp_threads) + ".pdf";
	plt.savefig(filename,format='pdf')

	# plt.show();

	print "Done - File written to " + filename

plotTimerpercentage(1)
plotTimerpercentage(8)

db.close();
