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

def getAvgAndVariance(field,wherestring,otherfields=""):
	with db:
		querystring = "SELECT {2} AVG({0}), SUM(({0}-sub.field_avg)*({0}-sub.field_avg))/(COUNT()-1) FROM measurements, (SELECT AVG({0}) AS field_avg FROM measurements WHERE {1}) AS sub WHERE {1}".format(field,wherestring,otherfields)
		# print querystring
		query.execute(querystring)
		data = query.fetchall()

	return np.array(data)


query.execute("SELECT number_of_threads FROM measurements GROUP BY number_of_threads")
number_of_threads = np.array(query.fetchall())
print "Threads:\n", number_of_threads


def plotStrongScaling(graphtype,optim,basesize,hostnamelike):


	fig = plt.figure()
	ax = fig.add_subplot(111)

	fixedwhere = "enable_analysis=0 AND graph_type='{0}' AND debug=0 AND verbose=0 AND optimistic={1} AND processors>=number_of_threads AND hostname LIKE '{3}'".format(graphtype,optim,hostnamelike)

	t = []
	var_t = []
	for nt in number_of_threads:
		s = basesize*nt[0]
		where = fixedwhere + " AND number_of_threads={0} AND graph_num_nodes={1}".format(nt[0],s)
		d = getAvgAndVariance("total_time",where)
		# print "DATA=\n", d
		t.append(d[:,0])
		var_t.append(d[:,1])

		# Calculating stddev for speedup
		# stdev_speedup = np.sqrt(np.array(var_t[0]) * np.power((1./np.array(t)),2) + var_t * np.power((-t[0]/np.power(t,2)),2))


	speedup = t[0]/t;
	# print t[0]
	# print t
	# print speedup

	ax.plot([number_of_threads[0],number_of_threads[-1]],[1,1],'r--',label='perfectspeedup')
	ax.plot(number_of_threads,speedup,'*-',markersize=10,linewidth=2.0)
	# ax.errorbar(number_of_threads,speedup,yerr=stdev_speedup,fmt='d-',markersize=3)

	fontsize_title=12
	fontsize_label=14

	ax.set_ylabel('Efficiency', fontsize=fontsize_label)
	ax.set_xlabel('number of OMP threads', fontsize=fontsize_label)
	ax.set_title('Weak scaling\nBase size 1000000', fontsize=fontsize_title)
	ax.set_ylim([0,1.1])
	# ax.legend(['Plain','Load Adjusted'],loc=3)
	ax.tick_params(top='off', right='off', length=4, width=1)



# Save plots
	filename = plotdir + "/weakscaling.pdf";
	plt.savefig(filename,format='pdf')
	plt.show()

	print "Done - File written to " + filename


basesize = 1000000
plotStrongScaling('SOFTWARE',0,basesize,"e%") # hostname starts with e*


db.close();
