import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import colortableau as ct


plt.style.use('ggplot')
plotdir = "plots";
db = sqlite3.connect('measurements.db')

query = db.cursor()

def getPerc(where):

	subquery = "(SELECT threads.measurement_id AS mid, AVG(timings.value) AS t, timings.name AS category FROM timings INNER JOIN threads ON threads.id=timings.thread_id GROUP BY threads.id, timings.name) AS timingdetail"

	querystring = "SELECT timingdetail.category AS cat, AVG(timingdetail.t) AS t_det, AVG(m.total_time) AS t_tot FROM measurements AS m INNER JOIN  " + subquery + " ON m.id=timingdetail.mid WHERE " + where + " GROUP BY m.number_of_threads, timingdetail.category ORDER BY m.number_of_threads, timingdetail.category;"
	# print querystring
	query.execute(querystring)
	data = np.array(query.fetchall())
	return data

def plotPercGraph(graphtype,optim,size,host):
	
	fixedwhere = "enable_analysis=1 AND graph_type='{0}' AND debug=0 AND verbose=0 AND optimistic={1} AND graph_num_nodes={2} AND processors>=number_of_threads AND algorithm='locallist' AND hostname={3}".format(graphtype,optim,size,host)

	query.execute("SELECT number_of_threads FROM measurements WHERE " + fixedwhere + " GROUP BY number_of_threads")
	nthreads = np.array(query.fetchall())
	
	query.execute("SELECT name FROM timings GROUP BY name ORDER BY name")
	catnames = np.array(query.fetchall())

	# print catnames
	# print nthreads

	timings = [];

	for nt in nthreads:
		where = fixedwhere + " AND number_of_threads={0}".format(nt[0])
		data = getPerc(where)
		# print "Data:\n", data
		
		cats = dict(zip(catnames.flat,np.zeros(len(catnames)).flat))
		for d in data:
			cats.update({d[0]: d[1]})
		timings.append(list(cats.values()))
		# print "TIMINGS\n", timings

	timings = np.array(timings).astype(np.float)
	# using .astype(float) to avoid integer division
	tpercent = timings / timings.sum(axis=1)[:,None] * 100;

	# TODO: include total time difference as unknown

	fig = plt.figure()
	ax = fig.add_subplot(111)

	fontsize_title=12
	fontsize_label=14

	ax.set_title('Percentage composition of runtime\nGraph Type={0}, Graph Size={1}'.format(graphtype,size), fontsize=fontsize_title)
	ax.set_ylabel('Percentage of total runtime (%)', fontsize=fontsize_label)
	ax.set_xlabel('number of OMP threads', fontsize=fontsize_label)
	ax.tick_params(top='off', right='off', length=4, width=1)


	# Creating stacked area chart
	print "range = ", range(0,nthreads.shape[0])
	print "perc = ", tpercent #.T
	
	ax.stackplot(range(0,nthreads.shape[0]),tpercent.T,edgecolor='white',colors=ct.tableau20)
	
	plt.xticks(np.arange(0, nthreads.shape[0], 1))
	ax.set_xticklabels(nthreads.astype(int))

	ax.margins(0, 0) # Set margins to avoid "whitespace"

	lgnd = []
	lbl = []

	for i in range(0,len(catnames)):
		lgnd.append(plt.Rectangle((0,0),1,1,fc=ct.tableau20[i]))
		lbl.append(catnames[i])

	plt.legend(lgnd,lbl)

	filename = plotdir + "/" + "timepercentage_host{3}_type{0}_size{1}_opt{2}".format(graphtype,size,optim,host) + ".pdf";
	plt.savefig(filename,format='pdf')

	plt.show();

	print "Done - File written to " + filename


plotPercGraph('SOFTWARE',0,5000,'thinkpadE530.localdomain');
plotPercGraph('SOFTWARE',0,50000,'thinkpadE530.localdomain');
plotPercGraph('SOFTWARE',0,500000,'thinkpadE530.localdomain');
plotPercGraph('SOFTWARE',0,1000000,'thinkpadE530.localdomain');


db.close();
