import numpy as np
import matplotlib.pyplot as plt
import glob
import re
import sqlite3
import colortableau as ct

# plt.style.use('ggplot')
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



def plotWeakScaling(algo,graphtype,optim,basesize,hostnamelike):


	fig = plt.figure()
	ax = fig.add_subplot(111)

	fixedwhere = "enable_analysis=0 AND graph_type='{0}' AND debug=0 AND verbose=0 AND optimistic={1} AND processors>=number_of_threads AND hostname LIKE '{2}' AND algorithm='{3}' AND number_of_threads*{4}=graph_num_nodes".format(graphtype,optim,hostnamelike,algo,basesize)
	
	query.execute("SELECT number_of_threads FROM measurements WHERE " + fixedwhere + " GROUP BY number_of_threads")
	number_of_threads = np.array(query.fetchall())
	print "Threads:\n", number_of_threads

	if len(number_of_threads) == 0:
		print "\nNo Data for\n", fixedwhere
		return None



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
	ax.set_title('Weak scaling\nalgorithm={0}, graphtype={1}, basesize={2}'.format(algo,graphtype,basesize), fontsize=fontsize_title)
	ax.set_ylim([0,1.1])
	ax.tick_params(top='off', right='off', length=4, width=1)



	# Save plots
	filename = plotdir + "/weakscaling_{0}_gt{1}_opt{2}.pdf".format(algo,graphtype,optim);
	plt.savefig(filename,format='pdf')
	plt.show()

	print "Done - File written to " + filename


# hostname starts with e*
basesize = 1000000
plotWeakScaling('bitset','SOFTWARE',1,basesize,"e%") 
plotWeakScaling('bitset','RANDOMLIN',1,basesize,"e%") 
plotWeakScaling('bitset','CHAIN',1,basesize,"e%") 
plotWeakScaling('bitset','MULTICHAIN',1,basesize,"e%") 
plotWeakScaling('bitset','SOFTWARE',0,basesize,"e%") 
plotWeakScaling('bitset','RANDOMLIN',0,basesize,"e%") 
plotWeakScaling('bitset','CHAIN',0,basesize,"e%") 
plotWeakScaling('bitset','MULTICHAIN',0,basesize,"e%") 
plotWeakScaling('locallist','SOFTWARE',1,basesize,"e%") 
plotWeakScaling('locallist','RANDOMLIN',1,basesize,"e%") 
plotWeakScaling('locallist','CHAIN',1,basesize,"e%") 
plotWeakScaling('locallist','MULTICHAIN',1,basesize,"e%")


db.close();
