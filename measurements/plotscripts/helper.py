############### Color#############################################################
import numpy as np
import scipy as sp
import scipy.stats
import sqlite3

myFGcolors = [	(150,50,50), (50,150,50), (50,50,150), \
				(150,100,50), (50,150,100), (100,50,150), \
				(150,0,100), (100,150,0), (0,100,150), \
				(150,150,0), (0,150,150), (150,0,150), \
				(50,50,50), (100,100,100), (150,150,150) ]
myFGcolors = list([tuple([x/255. for x in tup]) for tup in myFGcolors])
myBGcolors = list([tuple([np.min([x+0.4,1.0]) for x in tup]) for tup in myFGcolors])
 
def getFGcolor(i):
	return (myFGcolors[i%len(myFGcolors)])

def getBGcolor(i):
	return (myBGcolors[i%len(myBGcolors)])



fontsize_title=14
fontsize_label=12


def mean_confidence_interval(data, confidence=0.95):
	a = np.array(data)
	n = len(a)
	m, se = np.mean(a), scipy.stats.sem(a)
	h = se * sp.stats.t._ppf((1+confidence)/2., n-1)
	return m, m-h, m+h



# plt.style.use('ggplot')
plotdir = "plots/";



def getData(field, wherestring):

	with sqlite3.connect('measurements.db') as db:
		query = db.cursor()
		querystring = "SELECT {0} FROM measurements WHERE {1}".format(field,wherestring)
		query.execute(querystring)
		data = query.fetchall() 
		
	return np.array(data)





##################################################################################
