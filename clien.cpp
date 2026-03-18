#include <iostream> // 导入标准输入输出头文件

// 包含用于处理字符串的头文件
#include <string> // C++
#include <cstring> // C

// 导入网络相关的头文件
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>


const int PORT = 8888; // 端口号
const char* ip = "0.0.0.0"; // ip地址
int main()
{
    /*
        创建服务器SOCKET套接字，并指定通信域和使用的协议
        AF_INET Pv4互联网协议进行通讯
        SOCK_STREAM 流试协议
        IPPROTO_TCP TCP协议
    */
    int ServerFD = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(ServerFD == -1) // 判断套接字是否创建成功
    {
        /*
            strerror函数的作用就是来将这个错误代码转化成人类可读的错误信息
            errno存储的就是上次系统调用失败时的错误代码
        */
        std::cout << "socket create failed: " << strerror(errno) << std::endl;
    }
    else
    {
        std::cout << "socket create successful!" << std::endl;
    }

    /*
        sockaddr_in IPv4地址结构，指定使用的通信域和服务器的端口号、ip地址
        那为什么不用 sockaddr 这个地址结构呢，那是因为啊这个地址结构是一个通用地址结构，并不关心地址的类型也就是是IPv4呢还是IPv6呢等等。。
        而 sockaddr_in 表示的就是一个具体地址结构，也就是IPv4的地址结构
    */
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ip);


    /*
        connect函数
        int __fd 指定你要给哪个套接字绑定ip地址和端口
        sockaddr *__addr 指定你要绑定的ip地址和端口号
        socklen_t __len 指定 sockaddr_in的这个结构体的大小

        作用：
        1. 向服务器发送连接请求
    */
    if(connect(ServerFD,(const sockaddr *)&addr,sizeof(addr)) == -1)
    {
        /*
            strerror函数的作用就是来将这个错误代码转化成人类可读的错误信息
            errno存储的就是上次系统调用失败时的错误代码
        */
        std::cout << "socket bind failed: " << strerror(errno) << std::endl;
        return -1;
    }
    else
    {
        std::cout << "server connection successful!" << std::endl;
        std::cout << "server " << "\t" << "ip:" << ip << "\t" << "port: " << PORT << std::endl;
    }

    // std::string MessageBUFF;
    char MessageBUFF[BUFSIZ];
    std::string SendMessage = "";
    while(1)
    {
        // std::cout << "enter while" << std::endl;
        std::cout << "Please enter send message: ";
        std::cin >> SendMessage;
        int SendLen = send(ServerFD,SendMessage.c_str(),sizeof(SendMessage),MSG_MORE);
        if(SendLen == -1)
        {
            /*
                strerror函数的作用就是来将这个错误代码转化成人类可读的错误信息
                errno存储的就是上次系统调用失败时的错误代码
            */
            std::cout << "error: " << strerror(errno) << std::endl;
            break;
        }
        std::cout << "message send successful!" << std::endl;

        int RecvLen = recv(ServerFD,(void *)&MessageBUFF,BUFSIZ,0);
        if(RecvLen == -1)
        {
            /*
                strerror函数的作用就是来将这个错误代码转化成人类可读的错误信息
                errno存储的就是上次系统调用失败时的错误代码
            */
            std::cout << "error: " << strerror(errno) << std::endl;
            break;
        }

        std::cout << "server recv messae: " << MessageBUFF << std::endl;
    }

    close(ServerFD);
    std::cout << "clien close!" << std::endl;
    return 0;
}