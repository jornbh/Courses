# Import libraries
import requests
import urllib.request
import time
from bs4 import BeautifulSoup

# Set the URL you want to webscrape from
url = "https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_tutorials.html"

# Connect to the URL
response = requests.get(url)

# Parse HTML and save to BeautifulSoup object¶
soup = BeautifulSoup(response.text, "html.parser")

# To download the whole data set, let's do a for loop through all a tags
print(*soup.findAll('a'), sep="\n")
for i in range(1,len(soup.findAll('a'))): #'a' tags are for links
    one_a_tag = soup.findAll('a')[i]
    print(i, one_a_tag)
    link = one_a_tag['href']
    download_url = 'https://docs.opencv.org/'+ link
    print("download_url:\t", download_url)
    print("link:\t", link)
    # if "py_tutorials" in link:
    #     urllib.request.urlretrieve(download_url,'./'+link[link.find('/turnstile_')+1:]) 
time.sleep(0.2) #pause the code for a sec