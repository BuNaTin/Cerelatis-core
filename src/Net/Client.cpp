#include "Net/Client.h"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Logger.h"

namespace Cerelatis::Net {

class UDPClientImpl final : public Client {
    // interface
public:
    bool send(const std::string &data) override;

    // constructors
public:
    UDPClientImpl() = delete;
    UDPClientImpl(Address address, Port port);
    virtual ~UDPClientImpl() = default;

    // data
private:
    int mSocket;
    sockaddr_in server_addr;
};

UDPClientImpl::UDPClientImpl(Address address, Port port) {
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in client_addr;

    int server_len = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = address;

    int client_len = sizeof(client_addr);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0);
    client_addr.sin_addr.s_addr = INADDR_ANY;

    int err_code =
            bind(mSocket, (struct sockaddr *)&client_addr, client_len);

    if (err_code == -1) {
        LOGE() << "Could not bind socket";
        throw;
    }
}

bool UDPClientImpl::send(const std::string &data) {
    int sendResult = sendto(mSocket,
                            data.c_str(),
                            512,
                            0,
                            (sockaddr *)&server_addr,
                            sizeof server_addr);
    LoggerDebug _logger_debug;
    if (sendResult == -1) {
        return false;
    }
    const int size = 512;
    char buffer[size];
    int bytesRecieved = recvfrom(
            mSocket, buffer, size, 0, (struct sockaddr *)NULL, NULL);
    if (bytesRecieved > 0) {
        // Echo the text to the console
        LOGI() << "GET: " << std::string(buffer);
    } else {
        LOGW() << "No resp";
    }
    return true;
}

std::unique_ptr<Client>
Client::create(ServerType type, Address address, Port port) {

    LoggerDebug __logger_debug;
    Logger::setOutput(std::cout);

    switch (type) {
    case ServerType::TCP:
        LOGI() << "Create TCP client";
        throw;

    case ServerType::UDP:
        LOGI() << "Create UDP client";
        return std::make_unique<UDPClientImpl>(address, port);
    }

    LOGE() << "Error at creating server";
    return nullptr;
}

} // namespace Cerelatis::Net
