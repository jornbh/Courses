import hashlib
passwords = list(set( ["68gh3","68gh3","idz1l","idz1l","da5thm","2yocds","2yocds","2yocds","35d5u","2yocds","35d5u","2yocds","35d5u","wcjcw"]))
def hash(string):
    encoded = string.encode("utf-8")
    hashed = hashlib.sha256(encoded).hexdigest()
    return hashed.encode("utf-8")

test = hash("iloveyou")
hashed = "e4ad93ca07acb8d908a3aa41e920ea4f4ef4f26e7f86cf8291c5db289780a5ae".encode("utf-8")
print( test == hashed )

