import sys 
import flask
from flask import Flask
from flask import request
from flask import abort 
import json
import sys
import logging
import base64
logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)



app = Flask(__name__)
# @app.route('/hw2/ex1')
@app.route('/hw2/ex1', methods=[ 'GET','POST']) #DEBUG For debugging
def helloWorld(*args):
    try:
        # print(args, file=sys.stderr)

        if flask.request.method == 'POST':
            contents = request.get_json()
            # print( "contetns" , contents, file=sys.stderr)
            username = contents["user"]
            password = contents["pass"]
            key = "Never send a human to do a machine's job"
            # print( "Username" , username, file=sys.stderr)
            # print( "password" , password, file=sys.stderr)

            encrypted = encrypt(username, key) 
            # print( "COMPARE:   " , encrypted, password, file=sys.stderr, sep=",")
            if encrypted.strip() == password.strip():
                # print( "In test", file=sys.stderr)
                return "OK, you are logged in"
            else: 
                # print( "login failed" , file=sys.stderr)

                # H = {1:1}
                # b = H[2] # Key error to return a 400 
                return "Invalid username or password", 400
            # print("Post request")

        else:
            # print("Other request")
            return "Hello world"
    except Exception as e: 
        logging.exception(e)
        # raise e
        return "ERROR, I just crashed", 400




def encrypt(msg, key): 
    if (len(key) < len(msg)):
        diff = len(msg) -len(key)
        key += key[0:diff]
    amsg = msg.encode("ascii")
    akey=  key[0:len(msg)].encode("ascii")

    out = []
    for ind, el in enumerate(amsg): 
        result = el ^ akey[ind] 
        out.append(result)

    # print( "out" , out, file=sys.stderr)
    encoded = base64.b64encode(bytes(out))
    decoded_string =encoded.decode() 

    return decoded_string



app.run(debug=True)