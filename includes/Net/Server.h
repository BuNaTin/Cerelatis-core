#pragma once

#include "Net/Address.h"
#include "Net/Port.h"
#include "Net/ServerType.h"

#include <Handler/Handler.h>

#include <memory>

namespace Cerelatis::Net {

class Server {
    // interface
public:
    virtual bool
    setHandler(std::unique_ptr<Handler::Handler> &&new_handler) = 0;

    virtual bool start() = 0;

    // constructors
public:
    static std::unique_ptr<Server> create(ServerType = ServerType::TCP,
                                          Address = localhost,
                                          Port = 1234);
    virtual ~Server() = default;
};

} // namespace Cerelatis::Net