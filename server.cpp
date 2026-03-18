#include <iostream> // 导入标准输入输出头文件

// 导入网络相关的头文件
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    /*
        创建服务器SOCKET套接字，并指定通信域和使用的协议
        AF_INET Pv4互联网协议进行通讯
        SOCK_STREAM 流试协议
        IPPROTO_TCP TCP协议
    */
    int ServerFD = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    /*
        sockaddr_in IPv4地址结构，指定使用的通信域、端口号、ip地址
        那为什么不用 sockaddr 这个地址结构呢，那是因为啊这个地址结构是一个通用地址结构，并不关心地址的类型也就是是IPv4呢还是IPv6呢等等。。
        而 sockaddr_in 表示的就是一个具体地址结构，也就是IPv4的地址结构
    */
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(888);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");


    /*
        bind函数
        int __fd 指定你要给哪个套接字绑定ip地址和端口
        sockaddr *__addr 指定
    */
    bind(ServerFD,(const sockaddr *)&addr,sizeof(addr));

    return 0;
}


