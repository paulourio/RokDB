import socket

class RokDB(object):
    HOST = 'localhost'
    PORT = 3666
    
    def __init__(self, server=HOST, port=PORT):
        self.con = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.con.connect((server, port))
        
    def raw_send(self, data):
        self.con.send(data.encode())
        
    def raw_read(self, size=1024):
        return self.con.recv(size)
        
    def __del__(self):
        if self.con is not None:
            self.con.close()
