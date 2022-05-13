from pickle import TRUE
import socket

def server(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    address = (ip, port)
    s.bind(address)

    s.listen()

    con, addr = s.accept() 
    while TRUE:
        data = con.recv(1024)
        if len(data.decode("utf-8")) == 0:
            con.close()
            break
        print(data.decode('utf-8'))
        con.sendall(b"hello client this is server\n")
    s.close()

def main():
    server("localhost", 1236)

if __name__ == "__main__":
    main()