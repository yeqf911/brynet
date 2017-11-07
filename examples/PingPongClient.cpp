#include <iostream>
#include <string>

#include <brynet/net/SocketLibFunction.h>
#include <brynet/net/WrapTCPService.h>
#include <brynet/net/Connector.h>

using namespace brynet;
using namespace brynet::net;

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        fprintf(stderr, "Usage: <host> <port> <net work thread num> <session num> <packet size> \n");
        exit(-1);
    }

    std::string tmp(atoi(argv[5]), 'a');

    auto server = std::make_shared<WrapTcpService>();
    server->startWorkThread(atoi(argv[3]));

    auto connector = AsyncConnector::Create();
    connector->startWorkerThread();

    for (auto i = 0; i < atoi(argv[4]); i++)
    {
        try
        {
            connector->asyncConnect(argv[1], atoi(argv[2]), std::chrono::seconds(10), [server, tmp](sock fd) {
                std::cout << "connect success" << std::endl;
                ox_socket_nodelay(fd);
                server->addSession(fd, [tmp](const TCPSession::PTR& session) {
                    session->setDataCallback([](const TCPSession::PTR& session, const char* buffer, size_t len) {
                        session->send(buffer, len);
                        return len;
                    });
                    session->send(tmp.c_str(), tmp.size());
                }, false, nullptr, 1024 * 1024);
            }, []() {
                std::cout << "connect failed" << std::endl;
            });
        }
        catch (std::runtime_error& e)
        {
            std::cout << "error:" << e.what() << std::endl;
        }
    }

    std::cin.get();
}
