#include "computer.h"

void Computer::boot(program &p) {
    memory.init(0);

    for (auto &x : p) {
        if (x->hasPriority())
            x->execute(memory, processor);
    }

    for (auto &x : p) {
        if (!x->hasPriority())
            x->execute(memory, processor);
    }
}

void Computer::memory_dump(std::stringstream &ss) const {
    for (auto x : memory)
        ss << x << " ";
}
