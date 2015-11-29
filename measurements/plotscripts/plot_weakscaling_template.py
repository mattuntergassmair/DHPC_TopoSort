import numpy as np
import matplotlib.pyplot as plt
import sqlite3
import glob
import re
import colortableau as ct

import sqlite3 as db
plt.style.use('ggplot')
plotdir = "plots";
db = sqlite3.connect('annealing.db')
query = db.cursor()   


def getAvgAndVariance(field,wherestring,otherfields=""):
	with db:
		querystring = "SELECT {2} AVG({0}), SUM(({0}-sub.field_avg)*({0}-sub.field_avg))/(COUNT()-1) FROM results, (SELECT AVG({0}) AS field_avg FROM results WHERE {1}) AS sub WHERE {1}".format(field,wherestring,otherfields)
		query.execute(querystring)
		data = query.fetchall()

	return np.array(data)


omp_threads=8
start_nodes = 26

# Weak scaling
fig = plt.figure()
ax = fig.add_subplot(111)

mpir = [];
nodes = [];
timings_plain = [];
var_plain = [];
timings_adjusted = [];
var_adjusted = [];
for power in [0,1,2,3,4,5]:
	
	nodes.append(start_nodes+power);
	mpir.append(2**power);
	
	where = "graph_size={0} AND mpi_ranks={1} AND graph_type='rand' AND omp_threads={2} AND vec_flag=1 AND m=2 AND n_nodes=1".format(nodes[-1],mpir[-1],omp_threads)

	dpre = getAvgAndVariance("t_pre",where); tpre=dpre[:,0]; var_tpre=dpre[:,1]; 
	dnorm = getAvgAndVariance("t_norm",where); tnorm=dnorm[:,0]; var_tnorm=dnorm[:,1]; 
	dcomm = getAvgAndVariance("t_comm",where); tcomm=dcomm[:,0]; var_tcomm=dcomm[:,1]; 
	de0 = getAvgAndVariance("t_e0",where); te0=de0[:,0]; var_te0=de0[:,1]; 
	dep = getAvgAndVariance("t_ep",where); tep=dep[:,0]; var_tep=dep[:,1]; 

	timings_plain.append(float(tpre+tnorm+tcomm+te0+tep)); # plain timing - no scaling
	var_plain.append(float(var_tpre+var_tnorm+var_tcomm+var_te0+var_tep));
	te0 *= float(nodes[0])/nodes[-1]; var_te0 *= float(nodes[0])/nodes[-1];  # scaling E0
	tep *= (float(nodes[0])/nodes[-1])**2; var_tep *= (float(nodes[0])/nodes[-1])**2; # scaling EP
	timings_adjusted.append(float(tpre+tnorm+tcomm+te0+tep)); # scaled timing
	var_adjusted.append(float(var_tpre+var_tnorm+var_tcomm+var_te0+var_tep));

speedup_plain = timings_plain[0]/np.array(timings_plain/np.array(mpir))
speedup_adjusted = timings_adjusted[0]/np.array(timings_adjusted/np.array(mpir))

stdev_speedup_plain = np.sqrt(np.array(var_plain[0]) * np.power((1./np.array(timings_plain)),2) + var_plain * np.power((-timings_plain[0]/np.power(timings_plain,2)),2))
stdev_speedup_adjusted = np.sqrt(np.array(var_adjusted[0]) * np.power((1./np.array(timings_adjusted)),2) + var_adjusted * np.power((-timings_adjusted[0]/np.power(timings_adjusted,2)),2))

ax.errorbar(mpir,speedup_plain,yerr=stdev_speedup_plain,fmt='d:',markersize=3,c=ct.tableau20[1])
ax.errorbar(mpir,speedup_adjusted,yerr=stdev_speedup_adjusted,fmt='d-',markersize=3, c=ct.tableau20[2])

plt.axhline(y=1,xmin=1,xmax=32,linewidth=2,color='k')
ax.plot([1,32.8],[1,1],'r--')

fontsize_title=12
fontsize_label=14

ax.set_ylabel('Efficiency', fontsize=fontsize_label)
ax.set_xlabel('MPI ranks', fontsize=fontsize_label)
ax.set_title('Weak scaling\nOMP_THREADS={0}'.format(omp_threads), fontsize=fontsize_title)
ax.set_ylim([0,1.1])
ax.legend(['Plain','Load Adjusted'],loc=3)
ax.tick_params(top='off', right='off', length=4, width=1)

filename = plotdir + "/" + "weakscaling_mpi" + ".pdf";
plt.savefig(filename,format='pdf')
# plt.show();
print "Done - File written to " + plotdir + "/" + filename

db.close();
