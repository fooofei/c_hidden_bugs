#coding=utf-8


import socket



def entry():

  host_this = ('',3333)

  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  sock.bind(host_this)

  while 1:
    data,addr = sock.recvfrom(1024)

    print('[+] recv {} from {}'.format(data,addr))
    sock.sendto('received',addr)


if __name__ == '__main__':
    entry()
