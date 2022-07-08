#ifndef COMPUTER_H
#define COMPUTER_H

#include <sstream>
#include <memory>
#include <vector>
#include "ooasm.h"

class program {
private:
    using instruction_t = std::shared_ptr<Instruction>;
    std::vector<instruction_t> instr;
public:
    using iterator = std::vector<instruction_t>::const_iterator;

    iterator begin() const noexcept {
        return instr.begin();
    }

    iterator end() const noexcept {
        return instr.end();
    }

    program(std::initializer_list<instruction_t> l) : instr(l) {}
};

class Computer {
public:

    explicit Computer(size_t size) : memory(size), processor() {}

    ~Computer() = default;

    void boot(program &p);

    void memory_dump(std::stringstream &ss) const;

private:
    Memory memory;
    Processor processor;
};


#endif // COMPUTER_H
