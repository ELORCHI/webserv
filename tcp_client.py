import socket

def download(server, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    address = (server, port)
    s.connect(address)
    s.sendall(b"hello server this is client")
    
    data = s.recv(1024)
    s.close()
    print(data.decode("utf-8"))

def main():
    download("localhost", 8081)

if __name__ == "__main__":
    main()