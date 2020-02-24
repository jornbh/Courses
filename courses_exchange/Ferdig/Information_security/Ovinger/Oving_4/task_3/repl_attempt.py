#!/usr/bin/env python3
import os
import sys
import populate
from flask import g
from flask import Flask, current_app
from flask import render_template, request, jsonify
import pymysql


app = Flask(__name__)
username = "root"
password = "root"
database = "hw4_ex3"

## This method returns a list of messages in a json format such as 
## [
##  { "name": <name>, "message": <message> },
##  { "name": <name>, "message": <message> },
##  ...
## ]
## If this is a POST request and there is a parameter "name" given, then only
## messages of the given name should be returned.
## If the POST parameter is invalid, then the response code must be 500.
@app.route("/messages",methods=["GET","POST"])
def messages():
    with db.cursor() as cursor:
        ## your code here 
        parameters = request.get_json()
        if request.method !="POST": 
            return 500
        elif request.method =="POST": 
            if "name" not in parameters or len(parameters["name"]) ==0: 
                return "Bad value", 500
            # No errors found 
            name = parameters["name"]
            base_statement = "SELECT * FROM messages WHERE name=%s"
            cursor.execute( base_statement, name )
            rows = cursor.fetchall()


            cursor.execute( "SELECT * FROM messages")
            entire_database = cursor.fetchall()
            log = open("log.txt", "w")
            # print(entire_database, file = log)
            log.close()
            return_json = [ {"name": row[0], "message": row[1]} for row in rows ]
            print(rows)
            


            return jsonify(return_json),200
        else:
            print("UNKNOWN ERROR")
            return "Bad value messages", 500

## This method returns the list of users in a json format such as
## { "users": [ <user1>, <user2>, ... ] }
## This methods should limit the number of users if a GET URL parameter is given
## named limit. For example, /users?limit=4 should only return the first four
## users.
## If the paramer given is invalid, then the response code must be 500.
@app.route("/users",methods=["GET"])
def contact():
    with db.cursor() as cursor:
        ## your code here 

        if request.method != "GET":
            return "WRONG METHOD", 500

    
        parameters = request.get_json()
        if parameters == None or  "limit" not in parameters: 
            sql_command = "SELECT * FROM users"
            cursor.execute( sql_command )
            rows = cursor.fetchall()
            user_list = list(set(i[1] for i in rows))
            print("#"*25, "User list, limit = {} ".format(None), *rows, sep="\n")

            return jsonify( {"users": user_list} )

        else: 
            str_limit = str(parameters["limit"]).strip()
            try:
                limit = int(str_limit)
            except:
                return "NOT A STRING", 500
        if limit <0: 
            return "NEGATIVE INPUT", 500
        if limit == 0: 
            user_list = []
            
        else:
            
            sql_command = "SELECT * FROM users LIMIT %s"
            cursor.execute( sql_command, limit )
            rows = cursor.fetchall()
            
            user_list = [i[1] for i in rows]
            print("#"*25, "User list, limit = {} ".format(limit), *user_list, sep="\n")

                

        return jsonify( {"users": user_list} )

        
            
            



if __name__ == "__main__":
    seed = "randomseed"
    if len(sys.argv) == 2:
        seed = sys.argv[1]

    db = pymysql.connect("localhost",
                username,
                password,
                database)
    with db.cursor() as cursor:
        populate.populate_db(seed,cursor)             
        db.commit()
    print("[+] database populated")

    app.run(host="0.0.0.0",port=80)
