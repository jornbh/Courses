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
from collections import defaultdict
conversations_list = np.array(getRawDataFromFile(datapath= root+"Datasets/personachat/"))
indexes = np.random.randint(0, len(conversations_list), 2)
#$save

# dataset_statistics = []
# for connversation in conversations_list: 
#     conversation_statistics = []
#     for sentence in connversation:
        
#         dot_free_string = sentence.replace(".", " ") 
#         occurences = defaultdict(lambda : 0) 
#         for word in dot_free_string: 
#             occurences[word] += 1
#         conversation_statistics.append(occurences)

#     dataset_statistics.append(conversation_statistics)
# #$save

# print("DONE")
# dump(dataset_statistics)

# We do common statistics for all of them 

occurences = defaultdict(lambda : 0) 

for connversation in conversations_list: 

    for sentence in connversation:
        occurences[","] += sentence.count(",")
        occurences["."] += sentence.count(".")
        cleaned = sentence.replace(".", " ").replace(",", " ")
        for word in cleaned.split():
            occurences[word] +=1

#$save
for i in occurences: 
    print("##", i , occurences[i] )
# print(occurences)