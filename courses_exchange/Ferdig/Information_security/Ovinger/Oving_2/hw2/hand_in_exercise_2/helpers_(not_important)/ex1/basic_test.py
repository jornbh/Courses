import requests as R
my_json = {"user":"hello", "pass": "World"}
print(my_json)
response = R.post("http://127.0.0.1:5000/hw2/ex1", json=my_json)

print(response.text)
my_json["pass"] = "JgAaCR0="
response = R.post("http://127.0.0.1:5000/hw2/ex1", json=my_json)
print(response.text)