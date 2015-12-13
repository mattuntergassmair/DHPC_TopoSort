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
		# for debugging
		querystring = "SELECT {2} AVG({0}) FROM measurements WHERE {1}".format(field,wherestring,otherfields)
		print querystring
		query.execute(querystring)
		data = query.fetchall()
		# print "DATA: \n", data

	return np.array(data)




def plotStrongScaling(algo,graphtype,sizes,optim,hostnamelike):


	fig = plt.figure()
	ax = fig.add_subplot(111)

	fixedwhere = "enable_analysis=0 AND graph_type='{0}' AND debug=0 AND verbose=0 AND optimistic={1} AND processors>=number_of_threads AND hostname LIKE '{2}' AND algorithm='{3}'".format(graphtype,optim,hostnamelike,algo)

	query.execute("SELECT number_of_threads FROM measurements WHERE " + fixedwhere + " GROUP BY number_of_threads")
	number_of_threads = np.array(query.fetchall())
	print "Threads:\n", number_of_threads

	if len(number_of_threads) == 0:
		print "\nNo Data for\n", fixedwhere
		return None

	color_cnt=0
	for s in sizes:
		t = []
		var_t = []
		for nt in number_of_threads:
			where = fixedwhere + " AND number_of_threads={0} AND graph_num_nodes={1}".format(nt[0],s)
			d = getAvgAndVariance("total_time",where)
			# print "DATA=\n", d
			t.append(d[:,0])
			# var_t.append(d[:,1])

			# Calculating stddev for speedup
			# stdev_speedup = np.sqrt(np.array(var_t[0]) * np.power((1./np.array(t)),2) + var_t * np.power((-t[0]/np.power(t,2)),2))


		speedup = t[0]/t;
		# print "TIME\n", t
		# print "SPEEDUP\n", speedup

		ax.plot(number_of_threads,speedup,'*-',markersize=10,linewidth=2.0,c=ct.getFGcolor(color_cnt))
		# ax.errorbar(number_of_threads,speedup,yerr=stdev_speedup,fmt='d-',markersize=3)
		color_cnt+=1

	perfectspeedup = np.linspace(1,max(number_of_threads))
	ax.plot(perfectspeedup,perfectspeedup,'r--',label='perfectspeedup')

	fontsize_title=12
	fontsize_label=14

	ax.set_title('Strong scaling\nalgorithm={0}, graphtype={1}'.format(algo,graphtype), fontsize=fontsize_title)
	ax.set_ylabel('Speedup', fontsize=fontsize_label)
	ax.set_xlabel('OMP number_of_threads', fontsize=fontsize_label)
	ax.legend(sizes,loc=2,title='Graph size')
	ax.tick_params(top='off', right='off', length=4, width=1)

# Save plots
	filename = plotdir + "/strongscaling_{0}_gt{1}_opt{2}.pdf".format(algo,graphtype,optim);
	plt.savefig(filename,format='pdf')
	plt.show()

	print "Done - File written to " + filename


sizes = [1000000]
# hostname starting with e*

'''
plotStrongScaling('locallist','SOFTWARE',sizes,0,'e%') 
plotStrongScaling('locallist','RANDOMLIN',sizes,0,'e%') 
#plotStrongScaling('locallist','CHAIN',sizes,0,'e%') 
#plotStrongScaling('locallist','MULTICHAIN',sizes,0,'e%') 
plotStrongScaling('bitset','SOFTWARE',sizes,0,'e%') 
plotStrongScaling('bitset','RANDOMLIN',sizes,0,'e%') 
#plotStrongScaling('bitset','CHAIN',sizes,0,'e%') 
#plotStrongScaling('bitset','MULTICHAIN',sizes,0,'e%')
plotStrongScaling('bitset','SOFTWARE',sizes,1,'e%') 
plotStrongScaling('bitset','RANDOMLIN',sizes,1,'e%') 
#plotStrongScaling('bitset','CHAIN',sizes,1,'e%') 
#plotStrongScaling('bitset','MULTICHAIN',sizes,1,'e%')
'''

plotStrongScaling('locallist','SOFTWARE',[1000000],0,'thinkpad%') 
plotStrongScaling('locallist','RANDOMLIN',[1000000],0,'thinkpad%') 
plotStrongScaling('bitset','SOFTWARE',[1000000],0,'thinkpad%') 
plotStrongScaling('bitset','RANDOMLIN',[1000000],0,'thinkpad%') 


db.close();
