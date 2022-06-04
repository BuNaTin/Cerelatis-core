#pragma once

#include "Handler/Data.h"

#include <functional>
#include <memory>

namespace Cerelatis::Handler {

class Handler {
    // interface
public:
    virtual bool process(const Data &) = 0;

    // constructors
public:
    virtual ~Handler() = default;
};

std::unique_ptr<Handler> createLoggerHandler();

} // namespace Cerelatis::Handler