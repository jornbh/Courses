from sys import stdin
import re
import hashlib

def hash( password ):
    hashed = hashlib.sha256(password.encode()).hexdigest()
    return int(hashed, 16)

comparator = re.compile("^\('(.*)', (.*$)")
for line in stdin: 
    match_obj = comparator.match( line )
    # print(match_obj)
    if match_obj != None: 
        password = match_obj.group(1)
        hashed = match_obj.group(2)
        print(password, end="=>")
        print(hashed, end ="\t")
        print( hash(password) == hashed )
