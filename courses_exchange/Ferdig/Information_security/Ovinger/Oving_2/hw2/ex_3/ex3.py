import sys 
import flask
from flask import Flask
from flask import request
from flask import abort 
import json
import sys
import logging
import random
import hashlib
import hmac
# import helper as H
import base64
logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)
GOOD_RETURN_CODE = 200
BAD_RETURN_CODE  = 400
PASSWORD_COOKIE_NAME = "LoginCookie"
ADMIN_PASSWORDS = { "administrator": "42"}
KEY = "JAoEC1xIHAMdEEEFYA0FCw1AQxw="
ADMIN_STATUS = "administrator"
app = Flask(__name__)

# @app.route('/hw2/ex1/login')
@app.route('/ex3/login', methods=[ 'GET','POST']) #DEBUG For debugging
def login_fun(*args):
    try:
        # print("HELLO", file=sys.stderr)
        if request.method != "POST":
            return "Bad request", 404

        contents = request.get_json()
        username = contents["user"]
        password = contents["pass"]
        path_list = request.path.strip("/").split("/")
        path = ",".join(path_list)

        unique_id = random.randint(0,10**13) # to do Add actual safety here (proper random)
        if ADMIN_PASSWORDS.get(username, None) == password: # to do This line screams bad news, but oh'well, we are lazy
            privilege_status = ADMIN_STATUS
            return_status = 200
        else:
            privilege_status ="user"
            return_status = 201


        unhashed_cookie = "{},{},{}, {} ,{},{}".format(username, unique_id, "com402", path, privilege_status, "SECRET_COOKIE" )
        hashed_cookie = make_with_proper_hash(unhashed_cookie)
        # print("Made a cookie:  ", hashed_cookie, file=sys.stderr)


        reply_message = "A cookie is you"
        # print("HASHED COOKIE WE SEND: ", "'",hashed_cookie,"'", file=sys.stderr , sep="")
        print("1", file=sys.stderr , sep="")

        response = flask.make_response(reply_message, GOOD_RETURN_CODE)
        print("2", file=sys.stderr , sep="")

        encoded_cookie = H.encode(hashed_cookie)
        print("3", file=sys.stderr , sep="")

        # print(encoded_cookie)
        response.set_cookie(PASSWORD_COOKIE_NAME , value = encoded_cookie)
        print("4 (end)", file=sys.stderr , sep="")
        print("response for login:",response,  file=sys.stderr , sep="")

        return response
    except Exception  as e:
        print(e)
        print("LOGIN FAILED", file=sys.stderr)
        return "Login crashed", 409

# to do Cookie tests
@app.route('/ex3/list', methods=[ 'GET','POST']) #DEBUG For debugging
def validate_cookie(*args):
    try: 
        print("Start validate", file=sys.stderr)

        if request.method == "POST": 
            suspichious_cookie_encoded = request.cookies.get(PASSWORD_COOKIE_NAME)
            suspichious_cookie =H.decode(suspichious_cookie_encoded)
            print("GOT COOKIE: ", suspichious_cookie, file=sys.stderr)
            if suspichious_cookie == None: 
                return "No cookie received", 409 # Cookie not found
            proper_hashed_cookie = make_with_proper_hash(suspichious_cookie)
            status = compare_cookies( suspichious_cookie, proper_hashed_cookie) 

            if status == 403: 
                return "Nope, that is a modified packet", 403
            elif status == 201:
                return "A user" , 201
            else: 
                return "An admin", 200
    except Exception as e: 
        print(e) 
        return "Not good", 401
            
def compare_cookies(suspichious_cookie, reference_cookie): 
    try:
    # print("SUSPICHIOUS COOKIE: ", suspichious_cookie, file=sys.stderr)
    # print("REFFERENCE COOKIE: ", reference_cookie, file=sys.stderr)
        [
            _username,
            _ ,
            _,
            *_path, 
            suspichious_privilege_status, 
            suspichious_hash
        ]                       = suspichious_cookie.split(",")
        [
            _,
            _,
            _,
            *_,
            trusted_privilege_status, 
            trusted_hash
        ] = reference_cookie.split(",")

        if suspichious_hash != trusted_hash: # This is unsafe, switchi it with hmac.compare_digest() to prevent timing attack
            return 403 # Error 
        elif suspichious_privilege_status == ADMIN_STATUS: 
            return 200 # Admin 
        else: 

            # print("USER, because: ", suspichious_privilege_status, ADMIN_STATUS, file=sys.stderr)
            return 201  # user
    except Exception as e: 
        print(e, file = sys.stderr)
        return 403


def make_with_proper_hash(cookie_contents): 
    without_hash = ",".join( cookie_contents.split(",")[:-1])

    encoded_key = KEY.encode("utf-8")
    encoded_msg = without_hash.encode("utf-8")
    hash_fun = hashlib.sha256
    hash_obj = hmac.new(encoded_key, encoded_msg, hash_fun)
    hash =hash_obj.hexdigest()

    with_hash = "{},{}".format( without_hash, hash)
    return with_hash

def H(): 
    pass

def encode(input): 
    return base64.b64encode( input.encode("utf-8")).decode("utf-8")


def decode(input): 
    return base64.b64decode( input.encode("utf-8")).decode("utf-8")

H.encode = encode 
H.decode = decode

app.run()
