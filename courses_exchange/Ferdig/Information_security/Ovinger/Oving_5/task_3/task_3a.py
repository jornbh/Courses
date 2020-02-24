from phe import paillier
import requests
import json 
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
print(token, token.json()["token"])
# reply= requests.post(reply_path, json = reply_json)

