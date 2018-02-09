
有些人在编码 C socket 程序时，习惯有下面的用法：
```c
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
```

我的疑惑是：假如出现了第 1 次没完全发送完成，需要继续发送的情景，

那么会有多次 `sendto` 调用，相应的对方也应该有多次 `recvfrom` 调用。


测试结果验证：
目前只测试到以下内容：

1 `SO_SNDBUF` `SO_RCVBUF` 两个大小有参考，加上 Windows 独有的 `SO_MAX_MSG_SIZE`
1 本机发送数据在本地限制以内， `sendto` 1 次调用成功，但是对方 `recvfrom` 提供的存储不足时（在接收能力范围内 -- Windows 上的接收能力是 `SO_MAX_MSG_SIZE` 不是 `SO_RCVBUF`），返回失败，`return -1`，`errno` 是 `EMSGSIZE`; 增大存储， `recvfrom` 成功。

2 本机发送数据在本地限制以内，超过对方接收能力，本机`sendto`失败，对方`recvfrom`无感知；减少本地发送数据，知道对方有能力 `recvfrom`，直到发送成功。

整个测试过程未出现 `需要多次 sendto` 的情景。 
  
