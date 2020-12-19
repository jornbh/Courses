import hashlib

def make_test_input(): 
    test_strings =  ["baa", "aca"]
    encoded_strings = [i.encode() for i in test_strings]

    encrypted_stuff = [ hashlib.sha256(i).hexdigest() for i in encoded_strings]
    as_ints = [int(i,16) for i in encrypted_stuff]
    return as_ints


