############### Color#############################################################
myFGcolors = [(44, 160, 44), (0, 107,164), (220, 95,189), (225,128,14), (193,179,215)]
myFGcolors = list([tuple([x/255. for x in tup]) for tup in myFGcolors])
myBGcolors = [(154,226,223), (100,82,129), (191,228,118), (255,250,129), (193,179,215), (228,82,75)]
myBGcolors = list([tuple([x/255. for x in tup]) for tup in myBGcolors])
  
def getFGcolor(i):
	return (myFGcolors[i%len(myFGcolors)])

def getBGcolor(i):
	return (myBGcolors[i%len(myBGcolors)])

##################################################################################
