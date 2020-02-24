# Code in order to make AREPL work for us ( In the notebook, this can be ignored ) 
############################################################################################################################################################
import sys
from arepldump import dump 
try:
    root = "/home/jorn/Documents/Fag/courses_exchange/Neural_networks/Artifichial_neural_networks_miniprojects/miniproject_2/"
    sys.path.insert(0, root+"src/")
    from initialize_miniproject import *
except: # This means we are in the notebook 
    root = "./"

############################################################################################################################################################
# The code starts here
conversations_list = np.array(getRawDataFromFile(datapath= root+"Datasets/personachat/"))
indexes = np.random.randint(0, len(conversations_list), 2)
#$save


print(type(conversations_list))
print( *conversations_list[indexes], sep="\n" )
