#pragma once

#include "Net/Address.h"
#include "Net/Port.h"
#include "Net/ServerType.h"

#include <functional>
#include <memory>

namespace Cerelatis::Net {

class Server {
    // interface
public:
    virtual bool setHandler(
            std::function<std::string(const std::string &)> &&func) = 0;

    virtual bool start() = 0;
    virtual bool stop() = 0;

    // constructors
public:
    static std::unique_ptr<Server> create(ServerType = ServerType::TCP,
                                          Address = localhost,
                                          Port = 4321);
    virtual ~Server() = default;
};

} // namespace Cerelatis::Net