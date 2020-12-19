from phe import paillier
import requests
import json 
import numpy as np
email = "jorn.hofstad@epfl.ch"

public_key, private_key = paillier.generate_paillier_keypair() 


###########################################################




def use_model_1(labels):
    address = "http://com402.epfl.ch/hw5/ex3/get_input"
    send_json = { "email": email, "x": labels }
    reply = requests.post(address, json=send_json)
    
    return reply

reply = use_model_1([]) #? 
got_data =reply.json() 


#$save
print("got data", got_data)
features = got_data["x"]
print("Features", features)








########################################################################
#$save


def get_json_template(): 
    send_json ={
    "email" : email,
    "pk": public_key.n, 
    "encrypted_input": None,
    "model":2,
    }
    return send_json
def encrypt_serializeable(in_features):
    encrypted_data = [ public_key.encrypt(x) for x in in_features ] #?
    serializable = [x.ciphertext() for x in encrypted_data]
    return serializable

def decrypt_rating(encrypted_rating):
    encrypted_pallier = paillier.EncryptedNumber(public_key, encrypted_rating)
    rating = private_key.decrypt(encrypted_pallier)
    return rating


def get_rating(features):
    s_encrypted_features = encrypt_serializeable(features)
    send_json = get_json_template()
    send_json["encrypted_input"] = s_encrypted_features
    path = "http://com402.epfl.ch/hw5/ex3/securehealth/prediction_service"
    reply = requests.post(path, json=send_json)
    # print(reply)
    # print( "send_json", send_json )
    # print("rating.text", "'{}'".format(reply.text))
    encrypted_rating = reply.json()["encrypted_prediction"]
    rating = decrypt_rating(encrypted_rating)
    return rating
    
print(len([1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0]))

N = 12
b_querry = [0]*N
b = get_rating(b_querry)

print(b)
x_querries = [ [ 0 for i in range(N) ] for j in range(N) ]
for i in range(len(x_querries)):
    x_querries[i][i] =1 
results = [ 0 for i in x_querries ]
for i in range(len(x_querries)):
    results[i] = get_rating(x_querries[i])
    print(x_querries[i], "=>",  results[i])

#$save



# print("results", results)
x = [None for i in x_querries]
for ind, el in enumerate(results):
    x[ind] = el -b

print("x =", x)
print("results=", results)
print("b=", b)
print("b_buerry=", b_querry)
print("x_querries=", *x_querries, sep="\n")
token_path = "http://com402.epfl.ch/hw5/ex3/get_token_2"
token_json = {
    "email": email, 
    "weights": x, 
    "bias": b
}

token_reply = requests.post( token_path, json=token_json )
print(token_reply)
print("TOken_reply:", token_reply.text)
encrypted_token = token_reply.json()["token"]
pailler_token = paillier.EncryptedNumber(public_key, encrypted_token)
token = private_key.decrypt(pailler_token)