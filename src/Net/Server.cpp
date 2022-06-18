#include "Net/Server.h"

#include <atomic>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils/ThreadPool.h"

#include "Logger.h"

namespace Cerelatis::Net {

class UDPServerImpl final : public Server {
    // interface
public:
    bool setHandler(std::function<std::string(const std::string &)>
                            &&func) override;

    bool start() override;
    bool stop() override;

    // constructors
public:
    UDPServerImpl() = delete;
    UDPServerImpl(Address address, Port port);
    virtual ~UDPServerImpl();

    // in-class interface
private:
    bool mainLoop();

    // data
private:
    std::function<std::string(const std::string &)> mHandler;
    utils::ThreadPool taskList;
    int mSocket;
    std::atomic_bool mActive;
};

UDPServerImpl::UDPServerImpl(Address address, Port port)
        : taskList(4), mActive(false) {
    // LoggerDebug __logger_debug;

    LOGI() << "Create Server on " << (int)port;
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int OptVal = 1;
    int OptLen = sizeof(int);
    int rc = setsockopt(mSocket,
                        SOL_SOCKET,
                        SO_REUSEADDR,
                        (const char *)&OptVal,
                        OptLen);
    if (rc == -1) {
        LOGE() << "Couldn't create socket";
        throw;
    }
    LOGI() << "Socket No: " << mSocket;
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = address;
    rc = bind(mSocket,
              (struct sockaddr *)&server_addr,
              sizeof server_addr);
    if (rc == -1) {
        LOGE() << "Couldn't bind socket";
        throw;
    }
    LOGI() << "Binded No: " << mSocket;
    // Set socket non-block
    timeval read_timeout;
    read_timeout.tv_sec = 1;
    read_timeout.tv_usec = 0;
    setsockopt(mSocket,
               SOL_SOCKET,
               SO_RCVTIMEO,
               &read_timeout,
               sizeof read_timeout);
}
UDPServerImpl::~UDPServerImpl() { close(mSocket); }

bool UDPServerImpl::setHandler(
        std::function<std::string(const std::string &)> &&func) {
    mHandler = std::move(func);
    return true;
}
bool UDPServerImpl::mainLoop() {
    LoggerDebug __logger_debug;

    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    const size_t size = 512;
    char buffer[size];
    mActive = true;
    while (mActive) {
        // Wait for the message
        int bytesRecieved = recvfrom(mSocket,
                                     buffer,
                                     size,
                                     0,
                                     (struct sockaddr *)&client_addr,
                                     (socklen_t *)&client_len);
        if (bytesRecieved == -1) {
            LOGW() << "No data";
            continue;
            // return false;
        }

        taskList.addTask([str = std::string(buffer),
                          out_addr = client_addr,
                          this]() {
            std::string ans = mHandler(str);
            sendto(mSocket,
                   ans.c_str(),
                   ans.size(),
                   0,
                   (sockaddr *)&out_addr,
                   sizeof out_addr);
        });
    }

    // close socket at the end
    return true;
}

bool UDPServerImpl::start() {
    taskList.addTask([this]() {
        this->mainLoop();
    });
    return true;
}

bool UDPServerImpl::stop() {
    mActive = false;
    return true;
}

std::unique_ptr<Server>
Server::create(ServerType type, Address address, Port port) {
    LoggerDebug __logger_debug;

    Logger::setOutput(std::cout);
    switch (type) {
    case ServerType::TCP:
        LOGI() << "Create TCP server";
        return nullptr;

    case ServerType::UDP:
        LOGI() << "Create UDP server";
        return std::make_unique<UDPServerImpl>(address, port);
    }

    LOGE() << "Error at creating server";
    return nullptr;
}

} // namespace Cerelatis::Net