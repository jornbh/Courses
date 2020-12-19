#!/usr/bin/env python3
import os
import sys
import populate
from flask import g
from flask import Flask, current_app
from flask import render_template, request, jsonify
import pymysql
from flask import Response

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
        if request.method == "GET":
            sql_command = "SELECT * FROM messages"
            cursor.execute(sql_command)
            rows = cursor.fetchall()
            return_json = [ {"name": row[0], "message": row[1]} for row in rows ]
            return jsonify(return_json),200

        elif request.method !="POST": 
            return "This is bad", 500
        elif request.method =="POST": 

            name = request.form["name"]
            if name == None: 
                base_statement = "SELECT * FROM messages"
                cursor.execute( base_statement )
                rows = cursor.fetchall()

            else: 
                base_statement = "SELECT * FROM messages WHERE name=%s"
                cursor.execute( base_statement, name )
                rows = cursor.fetchall()


    
            return_json = [ {"name": row[0], "message": row[1]} for row in rows ]
 
            


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

    
        limit_str = request.args.get("limit")
        


        
        if limit_str == None:
            sql_command = "SELECT name FROM users" # Add name instead of * if we need the name
            cursor.execute( sql_command )
            rows = cursor.fetchall()
            user_list = [ i[0] for i in rows]

            
        else:
            try: 
                limit = int(limit_str)
                if limit < 0: 
                    
                    return Response(status=500)
            except: 
                return  Response(status=500)
            sql_command = "SELECT name FROM users LIMIT %s"
            cursor.execute( sql_command , (limit))
            rows = cursor.fetchall()
            user_list = [ i[0] for i in rows]
            
            
            
        # print(parameters, user_list)
        return_json =  jsonify( {"users": user_list} )
        print( user_list ,limit_str ,file=sys.stderr, sep ="\n")
        return return_json

        
            
            



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
