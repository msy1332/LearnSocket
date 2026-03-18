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
    std::cout << "Server Starting up...." << std::endl;

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
        sockaddr_in IPv4地址结构，指定使用的通信域、端口号、ip地址
        那为什么不用 sockaddr 这个地址结构呢，那是因为啊这个地址结构是一个通用地址结构，并不关心地址的类型也就是是IPv4呢还是IPv6呢等等。。
        而 sockaddr_in 表示的就是一个具体地址结构，也就是IPv4的地址结构
    */
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ip);


    /*
        bind函数
        int __fd 指定你要给哪个套接字绑定ip地址和端口
        sockaddr *__addr 指定你要绑定的ip地址和端口号
        socklen_t __len 指定 sockaddr_in的这个结构体的大小

        那为什么还要指定大小呢？
        1. addr 是一个具体的地址类型结构类型
        2. (const sockaddr *)&addr 而这里我们通过强制转化来将他转化成通用类型
        3. 又因为通过类型他是不管你用的是哪个地址类型，所以这个我们才需要传这个结构体的大小，来让他知道我们传得是什么地址类型，并让他正确的解析这个地址

        那为什么服务器还要绑定ip地址呢，不是绑定一个要监听的端口就可以了吗
        1. 因为一个电脑他是可以又很多网卡的
        2. 所以我们这个才需要指定ip地址，来确认我们要从哪个网卡里面去接收数据
    */
    if(bind(ServerFD,(const sockaddr *)&addr,sizeof(addr)) == -1) // 判断是否绑定成功
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
        std::cout << "socket bind successful!" << std::endl;
    }


    /*
        listen函数
        作用：
        1. 将套接字设为被动模式
        2. 启动监听并等待客户端的连接请求
        3. 设置连接请求的最大队列大小
    */
    listen(ServerFD,5);
    std::cout << "server listen!" << "ip: " << ip << "port: " << PORT << std::endl;
    std::cout << "Server startup successful!" << std::endl;
    std::cout << "Waiting for the client's connection...." << std::endl;

    /*
        accept函数来接收一个客户端的连接并为其创建一个新的套接字，之后的话我们就可以使用这个套接字与客户端进行通信来了
        作用： 
        1. 从新连接的队列中拿出一个客户端的连接请求
    */
   
    sockaddr_in client;
    int len = 0;
    int ClientFd = accept(ServerFD,(sockaddr *)&client,(socklen_t *)&len);
    std::cout << "client online" << "\t" << "ip: " << inet_ntoa(client.sin_addr) << std::endl;

    // std::string MessageBUFF;
    char MessageBUFF[BUFSIZ];
    while(1)
    {
        int RecvLen = recv(ClientFd,(void *)&MessageBUFF,BUFSIZ,0);
        if(RecvLen == 0)
        {
            std::cout << "client close" << "\t" << "ip: " << inet_ntoa(client.sin_addr) << std::endl;
            break;
        }
        else if(RecvLen == -1)
        {
            /*
                strerror函数的作用就是来将这个错误代码转化成人类可读的错误信息
                errno存储的就是上次系统调用失败时的错误代码
            */
            std::cout << "error: " << strerror(errno) << std::endl;
            break;
        }
        std::cout << "client recv message: " << MessageBUFF << std::endl;

        int SendLen = send(ClientFd,(const void *)&MessageBUFF,sizeof(MessageBUFF),MSG_MORE);
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
        
    }

    close(ClientFd);
    close(ServerFD);
    std::cout << "server stop!" << std::endl;
    return 0;
}