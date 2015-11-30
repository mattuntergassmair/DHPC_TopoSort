import numpy as np
import matplotlib.pyplot as plt
import sqlite3
import glob
import re

import sqlite3 as db
plt.style.use('ggplot')
plotdir = "plots";
db = sqlite3.connect('measurements.db')

query = db.cursor()


def getAvgAndVariance(field,wherestring,otherfields=""):
	with db:
		querystring = "SELECT {2} AVG({0}), SUM(({0}-sub.field_avg)*({0}-sub.field_avg))/(COUNT()-1) FROM measurements, (SELECT AVG({0}) AS field_avg FROM measurements WHERE {1}) AS sub WHERE {1}".format(field,wherestring,otherfields)
		query.execute(querystring)
		data = query.fetchall()

	return np.array(data)


query.execute("SELECT number_of_threads FROM measurements GROUP BY number_of_threads")
number_of_threads = np.array(query.fetchall())

print "Threads:\n", number_of_threads

fig = plt.figure()
ax = fig.add_subplot(111)

sizes = [500, 10000, 100000]

color_cnt=0
for s in sizes:
	t = []
	var_t = []
	for nt in number_of_threads:
		where = ("number_of_threads=%i" % (nt))
		d = getAvgAndVariance("total_time",where)
		t.append(d[:,0])
		var_t.append(d[:,1])

		# Calculating stddev for speedup
		# stdev_speedup = np.sqrt(np.array(var_t[0]) * np.power((1./np.array(t)),2) + var_t * np.power((-t[0]/np.power(t,2)),2))

	# compute 'real' runtime
	# t = t/number_of_threads
	# var_t = var_t/number_of_threads
	speedup = t[0]/t;
	print t[0]
	print t
	print speedup

	ax.plot(number_of_threads,speedup,'*-',markersize=10,linewidth=2.0)
	# ax.errorbar(number_of_threads,speedup,yerr=stdev_speedup,fmt='d-',markersize=3)
	color_cnt+=1

perfectspeedup = np.linspace(1,max(number_of_threads))
ax.plot(perfectspeedup,perfectspeedup,'r--',label='perfectspeedup')

fontsize_title=12
fontsize_label=14

ax.set_title('Strong scaling', fontsize=fontsize_title)
ax.set_ylabel('Speedup', fontsize=fontsize_label)
ax.set_xlabel('OMP number_of_threads', fontsize=fontsize_label)
ax.legend(sizes,loc=2,title='Graph size')
ax.tick_params(top='off', right='off', length=4, width=1)

# Save plots
filename = plotdir + "/strongscaling.pdf";
plt.savefig(filename,format='pdf')
plt.show()

print "Done - File written to " + filename


db.close();
