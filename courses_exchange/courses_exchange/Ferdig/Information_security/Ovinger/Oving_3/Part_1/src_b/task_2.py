import extract
import numpy as np
import scipy.optimize

# A vector for the distribution of each of them: movie => ratings

def accumulate_ratings( entries , key_fun = None, ):
    table = { key_fun(i) : np.zeros(6) for i in entries }
    for i in entries: 
        table[key_fun(i)][ i.rating ]+=1
    
    # print(table)
    return table
def vec_similarity(x,y):
    
    inner_product = np.sum( x*y )
    norm = lambda vec : np.sum( np.abs(vec) )

    return inner_product / ( norm(x) + norm(y)  )

def get_best_clear_to_hash_map(com_entries, imdb_entries , key_fun= None ):
    com_ratings  = accumulate_ratings(com_entries , key_fun = key_fun)
    imdb_ratings = accumulate_ratings(imdb_entries, key_fun = key_fun)
    
    indexes_to_hashes = { ind : el  for ind, el in enumerate(com_ratings )}
    indexes_to_emails = { ind : el   for  ind, el in enumerate(imdb_ratings)}

    Similarity_matrix = np.zeros( (len(indexes_to_hashes), len(indexes_to_hashes)) )

    for i in range( Similarity_matrix.shape[0] ):
        for j in range( Similarity_matrix.shape[1] ):
            hash_rating = com_ratings[  indexes_to_hashes[i] ]
            mail_rating = imdb_ratings[ indexes_to_emails[j] ]

            Similarity_matrix[i,j] = vec_similarity( hash_rating, mail_rating )
    col_indexes, row_indexes = scipy.optimize.linear_sum_assignment(Similarity_matrix)

    print(" TOTAL COST 1",  np.sum(Similarity_matrix[row_indexes,col_indexes ] ))


    
    relations = { indexes_to_emails[i] : indexes_to_hashes[j] for i,j in zip(row_indexes, col_indexes) }
    return relations
    
if __name__ == "__main__":
    com_entries   = extract.get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/com402-1.csv")
    imdb_entries  = extract.get_contents("../hw3_ex1_jorn.hofstad@epfl.ch/imdb-1.csv")
    my_mail = "jorn.hofstad@epfl.ch"
 
    email_translator            = get_best_clear_to_hash_map(com_entries, imdb_entries , key_fun= lambda x: x.email)
    movie_translator_reversed   = get_best_clear_to_hash_map(com_entries, imdb_entries , key_fun= lambda x: x.movie)
    movie_translator = { value: key for key, value in movie_translator_reversed.items() }
    f = open("filler.txt", "w")
    f.write( str(email_translator))
    f.write( "\n")
    f.write( str(movie_translator))
    f.write( "\n" )

    f.close()

    my_hash = email_translator[my_mail] 
    my_movies = []
    for i in com_entries:
        if i.email == my_hash: 
            my_movies.append( movie_translator[i.movie] )
    print(*my_movies, sep="\n")

    





