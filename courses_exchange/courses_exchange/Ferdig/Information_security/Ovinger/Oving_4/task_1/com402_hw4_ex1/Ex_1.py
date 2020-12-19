import dp_query



database = dp_query.create_db(3)
dp_query.count_ratings(database)

for i in database:
    for j in i: 
        print(j, sep="\n")


