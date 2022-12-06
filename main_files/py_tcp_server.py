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
        msg = server_msg#.encode("utf-8")
        #self.conn.send(len(msg).to_bytes(4, byteorder="little"))
        self.conn.send(msg)


def main():
    Send = TCPServer(ip_adress='10.25.47.143', port=9091)

    try:
        Send.listen()
        #connect = Send.sock.makefile('wb')
        with picamera.PiCamera() as camera:
            camera.resolution = (640, 480)
            camera.framerate = 30
            time.sleep(2)
            stream = io.BytesIO()
            # Use the video-port for captures...
            for foo in camera.capture_continuous(stream, 'jpeg',
                                                 use_video_port=True):
                Send.send_vid(struct.pack('<L', stream.tell()))
                stream.seek(0)
                Send.send_vid(stream.read())
                stream.seek(0)
                stream.truncate()
                time.sleep(0.0000001)
            #Send.send_message(struct.pack('<L', 0))
        #     with picamera.PiCamera() as camera:
        #         camera.resolution = (640, 480)
        #         camera.framerate = 30
        #         time.sleep(2)
        #         start = time.time()
        #         count = 0
        #         stream = io.BytesIO()
        #         # Use the video-port for captures...
        #         for foo in camera.capture_continuous(stream, 'jpeg',
        #                                              use_video_port=True):
        #             Send.send_message(struct.pack('<L', stream.tell()))
        #             #connect.write(struct.pack('<L', stream.tell()))
        #             #print(struct.pack('<L', stream.tell()))
        #             #connect.flush()
        #             stream.seek(0)
        #
        #             Send.send_message(stream.read())
        #             #connect.write(stream.read())
        #             #print(stream.read())
        #             count += 1
        #             if time.time() - start > 30:
        #                 break
        #             stream.seek(0)
        #             stream.truncate()
        #     Send.send_message(struct.pack('<L', 0))
        # finally:
        #     Send.conn.close()
        #     #connect.close()
        #     finish = time.time()
        # print('Sent %d images in %d seconds at %.2ffps' % (
        #     count, finish - start, count / (finish - start)))


    except Exception as e:
        print(e)
        #N1.listen()
        #Send.connected

if __name__ == "__main__":

    main()

# while True:
#     msg = alp[i]
#     server_messag = json.dumps(msg)
#     print(server_messag)
#     client_msg = json.loads(Send.get_message())
#     print(client_msg)
#     # Send.send_message(server_msg=server_messag)
#     i = i + 1
#     i %= 4