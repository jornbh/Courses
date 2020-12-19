import requests
public_key = "fix_this".encode()
email = "jorn.hofstad@epfl.ch"
def send( encrypted_input , model_id):
    send_json = {
        "email": email,
        "pk": public_key,
        "encrypted_input": encrypted_input,
        "model": model_id,
    }

    address = "http://com402.epfl.ch/hw5/ex3/securehealth/prediction_service"
    response = requests.post(address, data=send_json)
    response.status_code #?
    print(response.text)
    return response

def use_model_1(labels):
    address = "http://com402.epfl.ch/hw5/ex3/get_input"
    send_json = { "email": email, "x": labels }
    reply = requests.post(address, json=send_json)
    
    return reply

def main():
    encrypted_input = "hello".encode()
    # send( encrypted_input, 1 )
    features = [i for i in range(12)]
    reply = use_model_1(features)
    print(reply)


if __name__ == "__main__":
    
    main()