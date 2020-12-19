import hashlib
import test_input
import string
import time
import sys
passwords = [
"c8bafb3715a9617045bd31b618320ddc5960d5a7f04a00033aef48fdb0ead8fd",
"8419d256870bbc42ab30318a1e10a0b44bf58c5d90f0e1cbdd94f8624bf03e3d",
"a192fc00a574ec79f3a6d788c2efbe96ca3b884ad43bc6a29c1ba209921fa42b",
"adf121691ea620f62975eb5ec6f72905641d712f3a536a752eaeab343a93d21e",
"7f4fedbb03332996a83992918169490eaffb25d6c23f268f3d9f182ac2e900df",
"9e9b97fcfcd88938a17afa524598d7205226521582716b26b015ab0c3544044f",
"cd1dce4838ec4907836fe6b3a11b9de46bf7f2a6ff4c0fca5f86bddec2f56258",
"c1c75a0137c6b125ffa214b6264bdceb77c7cc9ab2334090333cb64b9dc25c27",
"1c70e3d3ada5df41c44776b4dfc90366b08aadc72d8114579e9d33d1335ea87c",
"8ff39e6849b29beb87aa0cab6860c7c912064f023aac042a5117bd2dbe4e35cb",]
passwords = [ i.encode("utf-8") for i in passwords ]

top_string = ""
number_of_bases = 1
base_number = 1

start_time = time.localtime(time.time())

# start_time = time.process_time()
alphabet = list(string.ascii_lowercase) + list( string.digits )
alphabet_e = [i.encode() for i in alphabet]
solutions = []
max_length = 6

#!
def hash(string):
    encoded = string.encode("utf-8")
    hashed = hashlib.sha256(encoded).hexdigest()
    return hashed.encode("utf-8")

def write_checkpoint(base):
    f= open("task_1_checkkpoint.txt", "w")
    f.write(base+"\n")
    f.close()

#!
def recursive_search(base):
    global passwords
    if len(base) > max_length:
        return 
    if  max_length- len(base) > 2:
        estimate = estimate_time(base)
        write_checkpoint(base)
        print( base, estimate) 
    hashed = hash(base)


    for i in passwords:
        if hashed == i:
            print("\nFOUND: ", base, "=> ", i)
            solutions.append((base, i))
            write_solutions_to_file()
            # passwords.remove(i)
            if len(passwords) == len( solutions ):
                raise(ValueError("Search is Done"))
            break 
    
    for i in alphabet: 
        recursive_search(i+base)

    pass

#! estimating_search
def search(base):
    try: 
        for i in alphabet:
            recursive_search(i+base)
    except ValueError as status: 
        print(status)
    print(solutions)
    write_solutions_to_file()


def estimate_time(base): 
    done_work = 0
    if len(top_string) ==0: 
        gen = reversed(base)
    else:
        gen = reversed(base[:-len(top_string)])
    for i in gen: 
        pos = alphabet.index(i)
        done_work = (done_work*len(alphabet)) + pos
    
    done_work += (sys.argv.index(top_string) -1)*len(alphabet)**(max_length - len(top_string))
    
    total_work = len(alphabet)**(len(base) - len(top_string))*number_of_bases


    out_str= str(done_work)+"/"+str(total_work)
    now = time.localtime(time.time())
    # now = time.process_time()
    elapsed_time = (round(now-start_time))
    estimated_time = round((now-start_time) * total_work/(done_work+0.001) )


    def to_readable(seconds): 
        hours = seconds //3600
        seconds = seconds % 3600

        minutes = seconds // 60 
        seconds = seconds % 60
        out_string = "({},{},{}) ".format(hours, minutes, seconds) 
        return out_string
    time_string = " ".join([ "   Time (h,m,s):", to_readable(elapsed_time), to_readable(estimated_time) ])
    return out_str +  time_string


print(*solutions, sep ="\n")

#!
def start():
    args = sys.argv
    if len(args) >1:
        global top_string
        global number_of_bases
        global solutions
        number_of_bases = len(args) -1
        for i in range(1, len(args)):
            base = args[i] 
            top_string = base
            global base_number
            base_number = i
            search(base)
            write_solutions_to_file(done= True)
            # Everythong found for that solution 
            solutions = []

    else: 
        # base = ""
        search("")
    print("DONE", solutions)

def write_solutions_to_file(done = False): 
    f = open(top_string +  "_part_1_result.txt", "w")
    # print("solutions", solutions) 
    # print(*passwords, sep ="\n")
    
    for i in passwords:
        for j in solutions:
            # print( i == j[1], i, j[1], file = f )

            if i == j[1]:
                print( j, file = f )

                print(j )

    print(solutions, file = f) # Just in case
    if done == True: 
        f.write(top_string+" Done\n")
    f.close()


start()

