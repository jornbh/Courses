@app.route("/users",methods=["GET"])
def contact():
    with db.cursor() as cursor:
        ## your code here 

        if request.method != "GET":
            return "WRONG METHOD", 500

    
        parameters = request.get_json()
        if "limit" in parameters: 
            try: 
                limit = parameters["limit"].strip()
                num_limit = int(limit)
                if num_limit ==1:
                    command_string = "SELECT * FROM users" 
                    cursor.execute( command_string )
                    relevant_rows = [cursor.fetchone()]
                else:
                    command_string = "SELECT * FROM users select first %s rows only" 
                    cursor.execute( command_string, str(num_limit) )
                    relevant_rows = cursor.fetchall()
                users = [ user for _, user, _ in relevant_rows ]
                json = { "users": users }
                return jsonify(json), 200 
            except:
                return "Something crashed the server", 500
            return "Something else is fishy", 501

        else: # Return everything 
            try:
                cursor.execute( "SELECT * FROM users" )
                entire_database = cursor.fetchall()
                users = [ user for _, user, _ in entire_database ]

                json = { "users": users }
                return jsonify(json), 200
            except:
                return "Something crashed the server when no limit was givedn ", 502
    return "END OF FLOW AND STILL NOTHING, users", 503

