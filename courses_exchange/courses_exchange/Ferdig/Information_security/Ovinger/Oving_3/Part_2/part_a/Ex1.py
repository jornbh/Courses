import hashlib 
import binascii
passwords = [
0xc8bafb3715a9617045bd31b618320ddc5960d5a7f04a00033aef48fdb0ead8fd,
0x8419d256870bbc42ab30318a1e10a0b44bf58c5d90f0e1cbdd94f8624bf03e3d,
0xa192fc00a574ec79f3a6d788c2efbe96ca3b884ad43bc6a29c1ba209921fa42b,
0xadf121691ea620f62975eb5ec6f72905641d712f3a536a752eaeab343a93d21e,
0x7f4fedbb03332996a83992918169490eaffb25d6c23f268f3d9f182ac2e900df,
0x9e9b97fcfcd88938a17afa524598d7205226521582716b26b015ab0c3544044f,
0xcd1dce4838ec4907836fe6b3a11b9de46bf7f2a6ff4c0fca5f86bddec2f56258,
0xc1c75a0137c6b125ffa214b6264bdceb77c7cc9ab2334090333cb64b9dc25c27,
0x1c70e3d3ada5df41c44776b4dfc90366b08aadc72d8114579e9d33d1335ea87c,
0x8ff39e6849b29beb87aa0cab6860c7c912064f023aac042a5117bd2dbe4e35cb,]

print(*passwords, sep="\n")
# for length in [4,5,4]: 
#     for characters in 
text = "abcd".encode()
hashed = hashlib.sha256(text).hexdigest()
print(int(hashed, 16))
# print(passwords[0].to_bytes(passwords[0].bit_length(), "big"))