############### Color#############################################################
# These are the "Tableau 20" colors as RGB.  
tableau20 = [(44, 160, 44), (200,82,0), (0, 107,164), (220, 95,189), (225,128,14)]
  
# Scale the RGB values to the [0, 1] range, which is the format matplotlib accepts.  
for i in range(len(tableau20)):  
    r, g, b = tableau20[i]  
    tableau20[i] = (r / 255., g / 255., b / 255.)
##################################################################################