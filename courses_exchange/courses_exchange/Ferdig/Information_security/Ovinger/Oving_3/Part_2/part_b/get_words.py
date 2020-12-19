

f = open("valid_ignored_input.txt", "r")
# f = open("subset.txt", "r")


text = f.read()
f.close()

words = [i.strip() for i in text.split()]
words.append("")

if __name__ == "__main__":
    print(words)