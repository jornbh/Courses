import shutil
import os
import re 
current_dir = os.listdir()

# print(current_dir)

for file_name in current_dir:
    if re.match("^best_.*", file_name):
        print("Renaming file:", file_name)
        shutil.copy(file_name, "committed_"+file_name)
    # if "best" in file_name:
    #     shutil.copy(file_name, "committed"+file_name)


a = os.listdir("./Dump/")
b = os.listdir("./")
unique_old =    [i for i in a if i not in b and "committed" in i]

for unique in unique_old:
    print("Copy up", unique)
    # print("./Dump/"+unique,"./"+unique)
    shutil.copy("Dump/"+unique, unique)