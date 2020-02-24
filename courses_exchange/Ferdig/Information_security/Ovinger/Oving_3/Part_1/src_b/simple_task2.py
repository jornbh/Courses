import extract
import numpy as np
import operator
from collections import defaultdict
import scipy.optimize

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

def plausability(imdb_ratings, com_ratings):
    # print(imdb_ratings)
    # print(com_ratings)
    if any(i > j for i, j in zip(imdb_ratings, com_ratings)):
        # print("IMPOSSIBLE", imdb_ratings, com_ratings)
        return -1000000000
    return np.sum(imdb_ratings * com_ratings) / (
        np.sum(np.abs(imdb_ratings) + np.abs(com_ratings))
    )


def best_pairing( clear_bucket, hashed_bucket ):
    similarity_matrix = np.zeros( ( len(clear_bucket), len( hashed_bucket ) ) )
    for c_ind, c_el     in enumerate(clear_bucket):
        for h_ind, h_el in enumerate(hashed_bucket): 
            similarity_matrix[c_ind, h_ind] = -plausability(c_el.all_ratings, h_el.all_ratings)
    first_indecies, second_indecies = scipy.optimize.linear_sum_assignment(similarity_matrix)

    # print(first_indecies)
    pairs = [(clear_bucket[i],hashed_bucket[j]) for i,j in zip( first_indecies, second_indecies )]
    # print(*pairs, sep="\n¤\t")
    # print( " COST", sum( similarity_matrix[first_indecies, second_indecies] ) )
    return pairs

def count_frequencies(entries, key_fun=None):
    counters = defaultdict(rated_element)

    for i in entries:
        counters[key_fun(i)].n_ratings += 1
        counters[key_fun(i)].all_ratings[i.rating] += 1
        counters[key_fun(i)].name = key_fun(i)

    as_list = [value for key, value in counters.items()]


    as_list.sort()
    return as_list



f_hash = open("hash_movie.txt","w")
f_clear = open("clear_movie.txt","w")

if __name__ == "__main__":
    com_entries = extract.get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/com402-2.csv")
    imdb_entries = extract.get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/imdb-2.csv")
    my_mail = "jorn.hofstad@epfl.ch"



    hashed_movie_list = count_frequencies(com_entries, key_fun=lambda x: x.movie)
    clear_movie_list = count_frequencies(imdb_entries, key_fun=lambda x: x.movie)
    

    print( *hashed_movie_list, sep ="\n", file = f_hash)
    print( *clear_movie_list, sep ="\n", file = f_clear)
    
    movie_encoder = { c_el.name : h_el.name for c_el, h_el in zip( clear_movie_list, hashed_movie_list) }
    
    movie_decoder = { value: key for key, value in movie_encoder.items() }
  

    my_movies = [i.movie for i in imdb_entries if i.email == my_mail]

    my_watched_hashes = [movie_encoder[i] for i in my_movies]

    # The ones where I have seen the movies
    relevant_entries_hashed = [i for i in com_entries if i.movie in my_watched_hashes]
    relevant_entries_clear = [i for i in imdb_entries if i.movie in my_movies]

    hashed_mail_list = count_frequencies(
        relevant_entries_hashed, key_fun=lambda x: x.email
    )
    clear_mail_list = count_frequencies(
        relevant_entries_clear, key_fun=lambda x: x.email
    )

    print("[", *hashed_mail_list,"#",  sep ="\n")
    print("[", *clear_mail_list,"#",  sep ="\n")

    my_hash = hashed_mail_list[-1].name

    results = [(movie_decoder[i.movie], i.rating) for i in com_entries if i.email == my_hash]
    for i in my_movies:
        if i not in results:
            print("Not in the result:", i)
    results.sort()
    # print(*results, sep = "\n")

    my_seen_movies_file = open("my_seen_movies.txt","w")
    my_results_file = open("my_results.txt","w")

    my_rated_imdb_movies = [ (i.movie, i.rating) for i in imdb_entries if i.email== my_mail ]
    results.sort(key= lambda x:x[1], reverse=True)
    my_rated_imdb_movies.sort( key = lambda x : x[1], reverse= True)
    print( *map( lambda x:x[0], my_rated_imdb_movies), sep="\n", file = my_seen_movies_file )
    print( *map( lambda x:x[0], results), sep="\n", file = my_results_file )

    print("############", *sorted(my_movies), sep="\n")
