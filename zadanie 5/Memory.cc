#include "Memory.h"
#include <iostream>
#include <utility>

class OutOfMemory : public std::exception {
    const char *what() const noexcept override {
        return "not enough memory to declare variable";
    }
};

class VariableNotFound : public std::exception {
    const char *what() const noexcept override {
        return "no such variable";
    }
};

// Inicjalizacja pamięci
void Memory::init(word_t value) {
    elements = 0;
    std::fill(memory.begin(), memory.end(), value);
}

// Jeśli jest miejsce w pamięci, deklaruje zmienną o identyfikatorze <variable>
// i wartości <value>
void Memory::declare_variable(std::shared_ptr<Id> variable, word_t value) {
    if (elements == memory.size()) throw OutOfMemory();
    variables[elements] = std::move(variable);
    memory[elements] = value;
    elements++;
}

// Zwraca wskaźnik do komórki pamięci pod danym adresem
word_t *Memory::getValueRef(address_t address) { return &memory.at(address); }

// Zwraca adres zmiennej o identyfikatorze <id>
address_t Memory::getAddress(std::shared_ptr<Id> id) const {
    for (address_t addr = 0; addr < elements; addr++) {
        if (variables[addr]->getId() == id->getId())
            return addr;
    }
    throw VariableNotFound();
}