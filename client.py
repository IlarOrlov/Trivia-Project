import socket
import json


HOST = '127.0.0.1'
PORT = 4269

class Tester:

    def __init__(self , host , port):
        self.code = 0
        self.decoded_msg = ""
        self.encoded_msg = ""
        self.size = ""
        self.sock = socket.socket()
        # self.sock.connect((host, port))

    def print_msg(self):
       print(f" msg code : {self.code} , msg size : {self.size} , msg content : {self.decoded_msg}")

    def encode(self, code , msg):
        self.encoded_msg = ''.join(format(ord(i), '08b') for i in msg)
        self.encoded_msg = '0' * (8 - (len(bin(code)))) + bin(code).replace("0b", '') + '0' * (32 - len(bin(len(self.encoded_msg)).replace('0b', ''))) + bin(len(self.encoded_msg)).replace('0b', '') + self.encoded_msg
        return self.encoded_msg
        self.encoded_msg = self.encoded_msg.encode()
        

    def decode(self):
        self.encoded_msg = self.encoded_msg.replace("\x00" , "")
        print(self.encoded_msg)
        self.code = int(self.encoded_msg[:8], 2)
        self.size = int(self.encoded_msg[8:40], 2)
        self.decoded_msg = ''.join([chr(int(self.encoded_msg[i:i+8],2)) for i in range(40, len(self.encoded_msg) , 8)])

    def __del__(self):
        self.sock.close()

    def get_data(self):
        self.encoded_msg = ""
        while True:
            buff = self.sock.recv(4096)
            self.encoded_msg += buff.decode()
            if (len(buff)) < 4096:
                break
        self.decode()


    def send_data(self):
        self.sock.sendall(self.encoded_msg)

    def login( self , user : dict ):
        self.encode(1, json.dumps(user))

        self.send_data()
        self.get_data()
        self.print_msg()
        
    def signup(self ,user: dict ):
        self.encode(2, json.dumps(user))

        self.send_data()
        self.get_data()
        self.print_msg()


def main():
    tester = Tester(HOST, PORT)

    # if(input("1 - login , else for signup : ") == "1"):
    #     user = {
    #         "Username" : input("Enter Username: "),
    #         "Password": input("Enter Password : "),
    #     }
    #     tester.login(user)
    # else:
    #     user = {
    #         "Username" : input("Enter Username: "),
    #         "Password": input("Enter Password : "),
    #         "Email": input("Enter Email : "),
    #         "Phone": input("Enter Phone : "),
    #         "Date": input("Enter Date : "),
    #         "Apt": input("Enter Apartment number  : "),
    #         "Street": input("Enter Street  : "),
    #         "City": input("Enter City : "),
    #     }
    #    tester.signup(user)
    print(tester.encode(200, r'{"status":"200"}'))
    
if __name__ == "__main__":
    main()

