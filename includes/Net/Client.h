#pragma once

#include "Net/Address.h"
#include "Net/Port.h"
#include "Net/ServerType.h"

#include <memory>
#include <string>

namespace Cerelatis::Net {

class Client {
    // interface
public:
    virtual bool send(const std::string &data) = 0;

    // constructors
public:
    static std::unique_ptr<Client> create(ServerType = ServerType::TCP,
                                          Address = localhost,
                                          Port = 4321);
    virtual ~Client() = default;
};

} // namespace Cerelatis::Net