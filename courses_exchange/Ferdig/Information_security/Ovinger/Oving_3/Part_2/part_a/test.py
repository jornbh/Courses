import hashlib

# passwords = list(set( ["68gh3","68gh3","idz1l","idz1l","da5thm","2yocds","2yocds","2yocds","35d5u","2yocds","35d5u","2yocds","35d5u","wcjcw"]))


ziped = [
('idz1l',        b'c8bafb3715a9617045bd31b618320ddc5960d5a7f04a00033aef48fdb0ead8fd'),
('35d5u',        b'8419d256870bbc42ab30318a1e10a0b44bf58c5d90f0e1cbdd94f8624bf03e3d'),
('68gh3',        b'a192fc00a574ec79f3a6d788c2efbe96ca3b884ad43bc6a29c1ba209921fa42b'),
('wcjcw',        b'adf121691ea620f62975eb5ec6f72905641d712f3a536a752eaeab343a93d21e'),
('2yocds',       b'7f4fedbb03332996a83992918169490eaffb25d6c23f268f3d9f182ac2e900df'),
('tpwsg',        b'9e9b97fcfcd88938a17afa524598d7205226521582716b26b015ab0c3544044f'),
('da5thm',       b'cd1dce4838ec4907836fe6b3a11b9de46bf7f2a6ff4c0fca5f86bddec2f56258'),
('4kex',         b'c1c75a0137c6b125ffa214b6264bdceb77c7cc9ab2334090333cb64b9dc25c27'),
('rdug',         b'1c70e3d3ada5df41c44776b4dfc90366b08aadc72d8114579e9d33d1335ea87c'),
('0vq9',         b'8ff39e6849b29beb87aa0cab6860c7c912064f023aac042a5117bd2dbe4e35cb'),
]

passswords = [
b'c8bafb3715a9617045bd31b618320ddc5960d5a7f04a00033aef48fdb0ead8fd',
b'8419d256870bbc42ab30318a1e10a0b44bf58c5d90f0e1cbdd94f8624bf03e3d',
b'a192fc00a574ec79f3a6d788c2efbe96ca3b884ad43bc6a29c1ba209921fa42b',
b'adf121691ea620f62975eb5ec6f72905641d712f3a536a752eaeab343a93d21e',
b'7f4fedbb03332996a83992918169490eaffb25d6c23f268f3d9f182ac2e900df',
b'9e9b97fcfcd88938a17afa524598d7205226521582716b26b015ab0c3544044f',
b'cd1dce4838ec4907836fe6b3a11b9de46bf7f2a6ff4c0fca5f86bddec2f56258',
b'c1c75a0137c6b125ffa214b6264bdceb77c7cc9ab2334090333cb64b9dc25c27',
b'1c70e3d3ada5df41c44776b4dfc90366b08aadc72d8114579e9d33d1335ea87c',
b'8ff39e6849b29beb87aa0cab6860c7c912064f023aac042a5117bd2dbe4e35cb',
]
# passwords = ["tpwsg", "rdug", "68gh3", "0vq9", "68gh3", "idz1l", "da5thm", "2yocds", "35d5u", "wcjcw", "4kex"]

# hashes = [b'9e9b97fcfcd88938a17afa524598d7205226521582716b26b015ab0c3544044f',
# b'1c70e3d3ada5df41c44776b4dfc90366b08aadc72d8114579e9d33d1335ea87c',
# b'a192fc00a574ec79f3a6d788c2efbe96ca3b884ad43bc6a29c1ba209921fa42b',
# b'8ff39e6849b29beb87aa0cab6860c7c912064f023aac042a5117bd2dbe4e35cb',
# b'a192fc00a574ec79f3a6d788c2efbe96ca3b884ad43bc6a29c1ba209921fa42b',
# b'c8bafb3715a9617045bd31b618320ddc5960d5a7f04a00033aef48fdb0ead8fd',
# b'cd1dce4838ec4907836fe6b3a11b9de46bf7f2a6ff4c0fca5f86bddec2f56258',
# b'7f4fedbb03332996a83992918169490eaffb25d6c23f268f3d9f182ac2e900df',
# b'8419d256870bbc42ab30318a1e10a0b44bf58c5d90f0e1cbdd94f8624bf03e3d',
# b'adf121691ea620f62975eb5ec6f72905641d712f3a536a752eaeab343a93d21e',
# b'c1c75a0137c6b125ffa214b6264bdceb77c7cc9ab2334090333cb64b9dc25c27',]

def hash(string):
    encoded = string.encode("utf-8")
    hashed = hashlib.sha256(encoded).hexdigest()
    return hashed.encode("utf-8")


for i in ziped:
    test = hash(i[0])
    print( test == i[1] , test, i[1])
    
print(len(passswords), len(ziped))

