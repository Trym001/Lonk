import socket
import json

a = {"left" : 12,
     "right" : 55,
     "front" : 1000}
b = {"left" : 1134,
     "right" : 234,
     "front" : 13523}
c = {"left" : 3456647,
     "right" : 234234,
     "front" : 42535}
d = {"left" : 234,
     "right" : 234345,
     "front" : 345234}
#server_messag = json.dumps(a)
alp = [a,b,c,d]
#print(type(server_messag))

class TCPServer:
    def __init__(self, ip_adress: str, port : int):
        self.conn = None
        server_address = (ip_adress, port)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print(f"Serving connections on port {server_address[1]}")
        self.sock.bind(server_address)
        self.connected = False

    def listen(self):
        try:
            self.sock.listen()
            self.conn, addr = self.sock.accept()
            self.connected = True
            print(f"Connected by {addr}")

        except Exception as e:
            self.connected = False
            print(e)

    def send_message(self, server_msg: str):
        msg = server_msg.encode("utf-8")
        self.conn.send(len(msg).to_bytes(4, byteorder="little"))
        self.conn.send(msg)

    def get_message(self):
        msgSize = int.from_bytes(self.conn.recv(4), byteorder="little")
        data = self.conn.recv(msgSize).decode("utf-8")
        return data

def main():
    Send = TCPServer(ip_adress='localhost', port=9090)
    #Get = TCPServer(ip_adress='localhost', port=9091)
    while True:
        try:
            print(("test"))
            Send.listen()
            i = 0
            while True:
                msg = alp[i]
                server_messag = json.dumps(msg)
                print(server_messag)
                client_msg = json.loads(Send.get_message())
                print(client_msg)
                #Send.send_message(server_msg=server_messag)
                i = i+1
                i %= 4

        except Exception as e:
            print(e)
            #N1.listen()
            Send.connected

if __name__ == "__main__":

    main()





















"""
def main():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            # Connect the socket to the port where the server is listening
            server_address = ('localhost', 9090)
            print(f"Serving connections on port {server_address[1]}")
            sock.bind(server_address)

            sock.listen()
            conn, addr = sock.accept()
            with conn:
                print(f"Connected by {addr}")
                while True:
                    msgSize = int.from_bytes(conn.recv(4), byteorder="little")
                    data = conn.recv(msgSize).decode("utf-8")
                    #client_message = json.loads(data)
                    msg = server_messag.encode("utf-8")

                    #msg = "Hello, {}!".format(data).encode("utf-8")
                    conn.send(len(msg).to_bytes(4, byteorder="little"))
                    conn.send(msg)
    except Exception as e:
        print(e) """
    #kun ein client
    #laga tråd for hver client, kan fungera



