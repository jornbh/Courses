import wave
base_string = b"COM"

path = "/home/jorn/Documents/Fag/courses_exchange/Information_security/Ovinger/Oving_5/task_1/jorn.hofstad@epfl.ch.wav"
song = wave.open(path, mode="rb")

N = song.getnframes()
# N =100000
raw_frames = (song.readframes( N))
raw_framelist =list(raw_frames)
frame_bytes = bytearray(raw_framelist)


# Find the bitsdeeded to wearch for the letters COM (Planed to use this for a search, but never got to use it)
bit_list = []
for i in base_string:
    temp_list = []
    for bit in range(8):
        temp_list.append( (i >> bit)  & 1 )
    bit_list+= reversed(temp_list)

# The bit list now lets us search for "COM"
# Extract the LSB of each byte

extracted = [] 
jump_length = 2

for i in range(0, len(frame_bytes), jump_length):
    current_bit = frame_bytes[i] & 1
    extracted.append(current_bit)

# extracted_2 = [frame_bytes[i] & 1 for i in range(0,len(frame_bytes,), jump_length)] # Array of the bits


print(sum(extracted))


string_list = []
for i in range(0, len(extracted)-8, 8):
    num = 0
    for ind_bit in range(8):
        num = num << 1
        num |= extracted[i+ind_bit]
    string_list.append( chr(num) ) # Cast the binary value to a character
    
string = "".join(string_list)


print(string)
print(string.find("COM"))
print("end")