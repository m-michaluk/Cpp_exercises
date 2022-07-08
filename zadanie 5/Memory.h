#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <map>
#include <memory>
#include "Id.h"

using word_t = int64_t;
using address_t = uint64_t;

class Memory {
public:
    using iterator = std::vector<word_t>::const_iterator;

    explicit Memory(address_t size) : memory(size), variables(size),
                                      elements(0) {}

    void init(word_t value);

    void declare_variable(std::shared_ptr<Id> variable, word_t value);

    word_t *getValueRef(address_t address);

    address_t getAddress(std::shared_ptr<Id> id) const;

    iterator begin() const { return memory.begin(); }

    iterator end() const { return memory.end(); }

private:
    std::vector<word_t> memory;
    std::vector<std::shared_ptr<Id>> variables;
    address_t elements;
};

#endif // MEMORY_H
