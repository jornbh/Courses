from phe import paillier
import requests
import json 
import numpy as np
email = "jorn.hofstad@epfl.ch"

def use_model_1(labels):
    address = "http://com402.epfl.ch/hw5/ex3/get_input"
    send_json = { "email": email, "x": labels }
    reply = requests.post(address, json=send_json)
    
    return reply


reply = use_model_1([]) #? 
got_data =reply.json() 


#$save
print(got_data)
features = got_data["x"]
print("features",features)
public_key, private_key = paillier.generate_paillier_keypair() 
public_key #? 

encrypted_data = [ public_key.encrypt(x) for x in features ] #?
serializable = [x.ciphertext() for x in encrypted_data]
reply_path = "http://com402.epfl.ch/hw5/ex3/securehealth/prediction_service"

send_json ={
    "email" : email,
    "pk": public_key.n, 
    "encrypted_input": serializable,
    "model":1,
}
final_reply = requests.post(reply_path, json=send_json)

print(final_reply, final_reply.json())
#$save
encrypted_prediction = final_reply.json()["encrypted_prediction"]
encrypted_pallier = paillier.EncryptedNumber(public_key, encrypted_prediction)
result = private_key.decrypt(encrypted_pallier)
print(result)

get_token_json = {
    "email": email, 
    "prediction": result
}
token = requests.post("http://com402.epfl.ch/hw5/ex3/get_token_1", json = get_token_json)
#$save 
x_es = np.eye(12)
b_base = [0]*12 
def serializable_encrypt(features):
    encrypted_data = [ public_key.encrypt(x) for x in features ] #?
    serializable = [x.ciphertext() for x in encrypted_data]
    return serializable
send_json["model"] = 2
send_json["encrypted_input"] = serializable_encrypt(b_base)
print("Send_json", send_json)
b_reply = requests.post("http://com402.epfl.ch/hw5/ex3/securehealth/prediction_service", send_json)
#$save
# print(send_json)
print("Send_json", send_json)
def get_rating(reply): 
    print("Reply", reply)
    print("Reply.text", reply.text, len(reply.text))
    quit()
    rating_json = reply.json()
    value = private_key.decode( rating_json["encrypted_prediction"] )
    return value
rating = get_rating(b_reply)
print(rating)
print(token, token.json()["token"])
# reply= requests.post(reply_path, json = reply_json)

