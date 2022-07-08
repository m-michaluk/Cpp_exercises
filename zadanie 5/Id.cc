#include "Id.h"
#include <cstring>


class InvalidId : public std::exception {
    const char *what() const noexcept override {
        return "invalid id";
    }
};

Id::Id(const char *id) {
    size_t size = std::strlen(id);
    if (size < 1 || size > 10)
        throw InvalidId();
    id_ = id;
}

const std::string &Id::getId() const {
    return id_;
}