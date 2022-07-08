#ifndef ID_H
#define ID_H

#include <string>

class Id {
public:
    explicit Id(const char *id);

    const std::string &getId() const;

private:
    std::string id_;
};


#endif //ID_H
