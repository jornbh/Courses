import numpy as np


A = np.array(  [
                    [ 0.4 , 0.5 , 0   , 0    ] ,  
                    [ 0.6 , 0.5 , 0   , 0    ] ,  
                    [  0  , 0   , 0.5 , 0.5  ] ,    
                    [  0  , 0   , 0.5 , 0.5  ] ,    

] )

x  = np.array( [ 1,0,0,1 ] )
x = x / np.sum(x) #? 

x_es = [x]

for i in range(10):
    x_es.append( A@ x_es[-1] ) 


for i in x_es: 
    i 

x_es[-1] #? 


la, e_vecs = np.linalg.eig(A)




for i in range(e_vecs.shape[0]):
    
    index = (i, slice(0, e_vecs.shape[1]))
    index = ( slice(0, e_vecs.shape[0]), i)

    e_vecs[index] #?
    norm = np.sum( np.abs(e_vecs[ index]) ) #? 
    normalized = ( e_vecs[ index] / norm ) #?
    correcting_sign = max(np.sign(  normalized ) , key = abs) #?
    e_vecs[ index] = normalized*correcting_sign
    e_vecs[ index] #?
    norm = np.sum( np.abs(e_vecs[ index]) ) #? 

for i in la: 
    print( round(i, 3) )

print(e_vecs)

# We now saw that the vector converges to one of the e-vectors
# The stationary distribution is depdendent on the initial value
# All subspaces are independent, so all conditions seem to be met