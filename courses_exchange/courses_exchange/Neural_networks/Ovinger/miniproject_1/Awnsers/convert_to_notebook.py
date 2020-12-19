from nbformat import v3, v4

temp_file = open("joined_file.py", "w")
def w(*args,file=temp_file,**kwargs):
    print(*args, file=file, **kwargs)

for i in range(1,8):
    python_file =       open("../vanilla_python/task_{}.py".format(i))
    markdown_file =     open(                  "Task_{}.md".format(i))            
    w("# -*- coding: utf-8 -*-")
    w("# <nbformat>3.0</nbformat>")
    w("# <markdowncell>")
    for i in markdown_file:
        w("#", i.strip("\n"))
    w("# <codecell>")
    for i in python_file:
        w(i.strip("\n"))

temp_file.close()
    



with open("joined_file.py") as fpin:
    text = fpin.read()

nbook = v3.reads_py(text)
nbook = v4.upgrade(nbook)  # Upgrade v3 to v4

jsonform = v4.writes(nbook) + "\n"
with open("ad_hoc_notebook.ipynb", "w") as fpout:
    fpout.write(jsonform)