#encoding=utf-8

'''

UDP 发送疑惑：
  有些 C 代码在 sendto 时，防止发送不完全 会做 while 循环，代码如下：

  char buffer[buffer_size];
  //... 填充数据
  int rest_size = buffer_size;
  const char * p = buffer;
  int rc;

  for(;rest_size>0;)
  {
    rc = sendto(fd, p, rest_size,0, addr,addrlen);
    if(rc<0){
      break; // error occured
    }
    rest_size -= rc;
    p += rc;
  }
  // send all success

  疑惑是一次 sendto 调用，对方应该需要一次 recvfrom 对应，
  这里是循环 sendto，对方也需要循环 recvfrom

  这个 python 代码证明 两次 sendto 对方也需要两次 recvfrom，
  并不是 一次 recv 会把两次 sendto 组成一次消息

 运行结果

 server
   [+] recv hh from ('127.0.0.1', 63049)
   [+] recv ee from ('127.0.0.1', 63049)

 client
  [+] recv received from ('127.0.0.1', 3333)

'''

import socket


def entry():

  host_this = ('127.0.0.1',3333)

  sock= socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

  r = sock.sendto("hh", host_this)
  r = sock.sendto('ee',host_this)

  data,addr = sock.recvfrom(1024)
  print('[+] recv ({})[{}] from {}'.format(len(data),data,addr))

if __name__ == '__main__':
    entry()
