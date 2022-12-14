import io
import socket
import struct
import time
import picamera

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

    def send_message(self, server_msg):
        msg = server_msg.encode("utf-8")
        self.conn.send(len(msg).to_bytes(4, byteorder="little"))
        self.conn.send(msg)

    def get_message(self):
        msgSize = int.from_bytes(self.conn.recv(4), byteorder="little")
        data = self.conn.recv(msgSize).decode("utf-8")
        return data

    def send_vid(self, server_msg):
        self.conn.send(server_msg)


def main():
    Send = TCPServer(ip_adress='10.25.47.143', port=9092)

    try:
        Send.listen()

        #source: https://picamera.readthedocs.io/en/release-1.13/recipes2.html task: 4.9 Rapid capture and streaming
        with picamera.PiCamera() as camera:
            camera.resolution = (640, 480)
            camera.framerate = 30
            time.sleep(2)
            stream = io.BytesIO()
            # Use the video-port for captures..
            for foo in camera.capture_continuous(stream, 'jpeg',
                                                 use_video_port=True):
                Send.send_vid(struct.pack('<L', stream.tell()))
                stream.seek(0)
                Send.send_vid(stream.read())
                stream.seek(0)
                stream.truncate()
                time.sleep(0.0000001)



    except Exception as e:
        print(e)
        #N1.listen()
        #Send.connected

if __name__ == "__main__":

    main()
