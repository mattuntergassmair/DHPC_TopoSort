############### Color#############################################################
import numpy as np

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


##################################################################################
