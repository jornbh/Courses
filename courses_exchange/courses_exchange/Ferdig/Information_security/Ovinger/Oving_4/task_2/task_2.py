from flask import Flask
import bcrypt 
import sys 
import flask 
from flask import request
from flask import abort
import json



app = Flask(__name__)





@app.route('/hw4/ex2', methods= ['POST'])
def post_fun(*args): 
    try: 
        contents = request.get_json()
        username = contents["user"]
        password = contents["pass"]
        hashed_password =  bcrypt.hashpw(password.encode("utf-8"), bcrypt.gensalt())


        # print(hashed_password)
        return hashed_password.decode("utf-8")
    except: 
        return "Something crashed", 400

if __name__ == "__main__":
    
    app.run(port= 5000)