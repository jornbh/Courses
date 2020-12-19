import modifyers
import hashlib
import get_words
import time
import sys
class FoundException(ValueError):
    def __init__(self,*args, **kwargs):
        ValueError.__init__(self, *args, **kwargs)


def hash(string):
    encoded = string.encode("utf-8")
    hashed = hashlib.sha256(encoded).hexdigest()
    return hashed.encode("utf-8")

def check_passwords( attempt ):
    for i in passwords: 
        if attempt == i: 
            
            return True 
    return False

passwords = [   "0x546ba2dc3e745128ecb5076cc6f7cfed10b8d8d25d058f0a246742f5430abdaf",
                "0x6ab4af66e346c9b2675d4dbf8ca2fce72918f444d3eb5d91ea82016ed6150347",
                "0x7d6e1024b20907f53e168e1f9ac8edd2595820f17578faf9a4fd935b6ff303dc",
                "0x2871be6887e193835506cdffe9522f5b293035d60d4804feebe046c2ff4b31e6",
                "0x2b18a30df458970e509eabeb746c6f860aa95d9cf2c2d0227cc13f1e80d08458",
                "0x4de2f3b7a76e1769512af766a2ffc998cbc997a262afdc623fa3a00814e0a674",
                "0xbc6864a709df27f43e33009c6a003a5581107b7925316ad96046fff0f492c02d",
                "0xec855f2d382bc69842e73e0537ee1cea658fb8077d8f55685022736fdce72b58",
                "0x4a2ca2e323e09db51f8700df7e87a660daf957062d1d385de1ad302bcd718e8d",
                "0x7e7a827729182b1265cb8d64888d6324270dff1d4b8a96837bd31cc996318927",]
passwords = [i.encode("utf-8") for i in passwords]
# passwords = [hash("iloveyou")]

words = get_words.words
solutions = set()
partition = (0,len(words))
estimated_depth = 1
start_time = time.process_time()

def to_readable(seconds): 
        hours = seconds //3600
        seconds = seconds % 3600

        minutes = seconds // 60 
        seconds = seconds % 60
        out_string = "({},{},{}) ".format(hours, minutes, seconds) 
        return out_string
def estimate_time(counter): 
    branching = len(words)
    total_work = (branching**(estimated_depth+1)  -1)/(branching -1 )
    my_work = total_work* ((partition[1] - partition[0])/ len(words) )
    now = time.process_time()
    used_time = now-start_time
    estimated_work_time =round( (my_work/counter)*used_time)
    print( to_readable(round(used_time)) ,to_readable(estimated_work_time) )
    # print(counter,  my_work )


def make_loop_string( depth):
    
    create_string    =  "current_list =" '[""]*{}'.format(depth)
    looping_list = [
            "for ind_{}, el_{} in enumerate(words[partition[0]:partition[1]]):".format(0,0)
        ]
    line_change=  "current_list[{}] = el_{}".format(0,0)
    looping_list.append("\t"+ line_change)
    
    for i in range(1,depth): 
        line_loop = "for ind_{}, el_{} in enumerate(words):".format(i,i)
        line_change=  "current_list[{}] = el_{}".format(i,i)
        looping_list.append("\t"*i     + line_loop)
        looping_list.append("\t"*(i+1) + line_change)


    looping_text = "\n".join(looping_list)


    execution_string = "\t"*depth + "test_current_string(current_list)"
    
    completed_text = "\n".join([create_string,looping_text, execution_string])
    # print(completed_text)
    return completed_text
    
counter =0
def test_current_string(current_list):
    # print(current_list)
    global counter
    counter +=1
    if counter % 10000 ==0:
        estimate_time(counter)
    current_string = "".join(current_list)

    

    for ind, modifyer_list in enumerate(modifyers.all_uncurried): 
        modified_string = modifyers.execute_curry( modifyer_list, current_string)
        test_hash = hash(modified_string)
        
        if check_passwords(test_hash):
            solutions.add( (modified_string, test_hash))
            print(" GOT IT", modified_string, test_hash)
            write_to_file()
            if len(passwords) == len(solutions): 
                raise( FoundException("Found everyting") )

    # Save to file
def write_to_file(): 
    f = open(str(partition[0])+ "_of_" +str(partition[1])+"_results.txt", "w")
    for i in solutions:
        cleartext = i[0]
        hashtext = i[1] 
        f.write( "{} \t=>\t {}".format(cleartext, hashtext))
        f.write("\n")
        print(i)
    f.close()

if __name__ == "__main__":

    if len(sys.argv) == 3: 
        n_workers   = int(sys.argv[2]) 
        self_id     = int(sys.argv[1]) 
        
        partition   = ( round(len(words) *  self_id   /n_workers ), 
                        round(len(words) * (self_id+1)/n_workers ) )
        print(partition)
    try:
        depth =0
        while(len(solutions) < len(passwords)): 
            depth +=1
            loop_string = make_loop_string(depth)
            exec(loop_string)
            print(depth)

            break 
    except FoundException as e:
        print(type(e))
        print("Found everything")
        



    print("DONE, the found solutions are", solutions)

    write_to_file()
    print(counter)
