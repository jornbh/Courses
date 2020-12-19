# THIS SHOWS THAT NOBODY IS AN ACTUAL CANDIDATE
import extract
import numpy as np
import operator
from collections import defaultdict
import scipy.optimize
import sys

f_hash = open("hash_movie.txt","w")
f_clear = open("clear_movie.txt","w")
def count_frequencies(entries, key_fun=None):
    counters = defaultdict(rated_element)

    for i in entries:
        counters[key_fun(i)].n_ratings += 1
        counters[key_fun(i)].all_ratings[i.rating] += 1
        counters[key_fun(i)].name = key_fun(i)

    as_list = [value for key, value in counters.items()]


    as_list.sort()
    return as_list

def p_list(in_list, file = sys.stdout):
    for i in in_list:
        print(i, file = file)
class rated_element:
    def __init__(self):
        self.n_ratings = 0
        self.all_ratings = np.zeros(6)
        self.name = 0

    def __gt__(self, other):
        return self.n_ratings > other.n_ratings

    def __str__(self):
        return "r_el[ {},{},{} ]".format(self.n_ratings, self.all_ratings, self.name)

    def __repr__(self):
        return self.__str__()
if __name__ == "__main__":
    com_entries = extract.get_contents("./data/com402-2.csv")
    imdb_entries = extract.get_contents("./data/imdb-2.csv")
    my_mail = "jorn.hofstad@epfl.ch"
    clear_entries = count_frequencies(imdb_entries, lambda x: x.email)
    hashed_entries = count_frequencies(com_entries, lambda x: x.email)

    my_entry = [ i for i in clear_entries if i.name == my_mail ][0]
    # print(my_entry)

    tot_my_ratings = my_entry.n_ratings
    my_ratings = np.array(my_entry.all_ratings)
    
    candidates = []
    for i in hashed_entries:
        test_rating = np.array(i.all_ratings)
        if np.any( my_ratings > test_rating ):
            # print(i.name)
            print(my_ratings, test_rating )
            continue # Not a candidate
        if i.n_ratings - tot_my_ratings < test_rating - my_ratings:
            print(i)
            continue
        
        print( test_rating,  i.name, my_ratings )
        candidtes.append(i)
    print(candidates)

        

    # p_list(entries)



    
