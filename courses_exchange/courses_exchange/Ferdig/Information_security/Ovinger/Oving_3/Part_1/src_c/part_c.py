import extract 
import datetime
import numpy as np
import scipy.optimize
from collections import defaultdict


# start_date = datetime.timedelta.min
weights = list(range(1,14))+ list(range(14,0, -1))
time_differences = [ datetime.timedelta(days= i) for i in range(len(weights)) ]

name_entry = lambda : defaultdict(int) # To make a smaller list with all the times

def write_date_to_prob_dict( in_dict, key, date  ):
    for time_difference, weight  in zip(time_differences,weights):
        in_dict[key][date + time_difference] += weight
    # print(in_dict)

def dict_dot_product(dic_1, dic_2):
    accumulator = 0
    # print("Dot product len:", len(dic_1), len(dic_2))
    for key, value in dic_1.items():
        if key in dic_2:
            # print("GOT IT", key)
            accumulator+= dic_2[key]*value 
    denominator = ( 
                      sum( abs(value) for key, value in dic_1.items() ) 
                    + sum( abs(value) for key, value in dic_2.items() )
                )
    if denominator == 0: 
        return 0 # No corrolation 
    return accumulator/denominator

def best_assignment(clear_dict, hashed_dict): 
    Similarity_matrix = np.zeros( (len(clear_dict), len(hashed_dict)) )
    clear_values = [None for i in clear_dict]
    hashed_values = [None for i in hashed_dict]
    clear_pair_list = [ (key, value) for key, value in  clear_dict.items() ]
    hashed_pair_list = [ (key, value) for key, value in hashed_dict.items() ]

    for clear_ind, clear_pair in enumerate(clear_pair_list):
        print(clear_ind, "/", len(clear_pair_list))
        for hashed_ind, hashed_pair in enumerate(hashed_pair_list): 


            dict_dot_product( clear_dict[clear_pair[0]], hashed_dict[hashed_pair[0]] )
    first_indexes, second_indexes = scipy.optimize.linear_sum_assignment(Similarity_matrix)
    # print( np.max( Similarity_matrix))
    print("Total cost",  np.sum(Similarity_matrix[ first_indexes, second_indexes ] ))
    quit()
    best_pairs = []
    for ind_clear, ind_hash in zip(first_indexes, second_indexes): 
        # print( clear_pair_list[ind_clear][0], hashed_pair_list[ind_hash][0] ) 
        best_pairs.append((clear_pair_list[ind_clear][0], hashed_pair_list[ind_hash][0]))
    print(best_pairs)
    return best_pairs


if __name__ == "__main__":
    com_entries = extract.get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/com402-3.csv")
    imdb_entries = extract.get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/imdb-3.csv")
    my_mail = "jorn.hofstad@epfl.ch"

    # Translate mail 
    clear_mail_dict = defaultdict( name_entry )
    hashed_mail_dict = defaultdict( name_entry )


    for ind,el in enumerate(imdb_entries):
        numeric = datetime.datetime.strptime(el.date, "%d/%m/%y")
        write_date_to_prob_dict( clear_mail_dict,el.email, numeric )
        # if my_mail == el.email: 
        #     print( len(clear_mail_dict[my_mail]))
    # quit()

    for ind, el in enumerate(com_entries):
        numeric = datetime.datetime.strptime(el.date, "%d/%m/%y")
        write_date_to_prob_dict( hashed_mail_dict,el.email, numeric )
    
    print( dict_dot_product( clear_mail_dict[my_mail], clear_mail_dict[my_mail] ) )
    
    

    # print(len(clear_mail_dict) , max( max(list( clear_mail_dict.values()  ) , key =len  ).values() ) )
    # print(len(hashed_mail_dict), max( max(list( hashed_mail_dict.values() ) , key =len  ).values() ) )

    # quit()
    email_encoder = { i[0] :i[1] for i in best_assignment(clear_mail_dict, hashed_mail_dict)}

    print(email_encoder[my_mail])
    my_mail_hash = email_encoder[my_mail]

    # Translate movies 

    clear_movie_dict = defaultdict( name_entry )
    hashed_movie_dict = defaultdict( name_entry )

    for ind, el in enumerate(com_entries):
        numeric = datetime.datetime.strptime(el.date, "%d/%m/%y")
        write_date_to_prob_dict( hashed_movie_dict,el.movie, numeric )

    
    for ind,el in enumerate(imdb_entries):
        numeric = datetime.datetime.strptime(el.date, "%d/%m/%y")
        write_date_to_prob_dict( clear_movie_dict,el.movie, numeric )

    movie_encoder = { i[0] :i[1] for i in best_assignment(clear_movie_dict, hashed_movie_dict)}

    movie_decoder = { value: key for key, value in movie_encoder.items() }

    for i in com_entries: 
        if i.email == my_mail_hash:
            print( i.rating, "\t", movie_decoder[ i.movie ].ljust(50) )

    
    my_mail = "jorn.hofstad@epfl.ch"


