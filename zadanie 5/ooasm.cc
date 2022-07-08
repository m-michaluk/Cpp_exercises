#include "ooasm.h"

class InvalidAddressException : public std::exception {
    const char *what() const noexcept override {
        return "invalid address";
    }
};

void Lea::init(Memory &m) {
    variable_addr = m.getAddress(val);
}

word_t Lea::getWordValue() const {
    return (word_t) variable_addr;
}

address_t Lea::getAddressValue() const {
    return variable_addr;
}

word_t Num::getWordValue() const {
    return val;
}

address_t Num::getAddressValue() const {
    if (val < 0) throw InvalidAddressException();
    return val;
}

void Mem::init(Memory &m) {
    rVal->init(m);
    memory_ptr = m.getValueRef(rVal->getAddressValue());
}

word_t Mem::getWordValue() const {
    return *memory_ptr;
}

address_t Mem::getAddressValue() const {
    if (*memory_ptr < 0) throw InvalidAddressException();
    return *memory_ptr;
}

void Mem::setValue(word_t value) {
    *memory_ptr = value;
}


void Data::execute(Memory &m, Processor&) {
    m.declare_variable(id, value->getWordValue());
}

void Arithmetic::execute(Memory &m, Processor &p) {
    lVal->init(m);
    rVal->init(m);
    word_t result = calculate(lVal->getWordValue(), rVal->getWordValue());
    lVal->setValue(result);
    setSF(p, result);
    setZF(p, result);
}

void Arithmetic::setZF(Processor &p, word_t result) { // protected?
    p.setZF(result == 0);
}

void Arithmetic::setSF(Processor &p, word_t result) {
    p.setSF(result < 0);
}

word_t Add::calculate(word_t arg1, word_t arg2) const {
    return arg1 + arg2;
}

word_t Sub::calculate(word_t arg1, word_t arg2) const {
    return arg1 - arg2;
}

void Mov::execute(Memory &m, Processor&) {
    lVal->init(m);
    rVal->init(m);
    lVal->setValue(rVal->getWordValue());
}

void Onez::execute(Memory &m, Processor &p) {
    if (p.ZF()) One::execute(m, p);
}

void Ones::execute(Memory &m, Processor &p) {
    if (p.SF()) One::execute(m, p);
}

std::shared_ptr<Num> num(word_t var) {
    return std::make_shared<Num>(var);
}

std::shared_ptr<Mem> mem(const std::shared_ptr<RValue> &addr) {
    return std::make_shared<Mem>(addr);
}

std::shared_ptr<Lea> lea(const char *arg) {
    return std::make_shared<Lea>(std::make_shared<Id>(arg));
}

std::shared_ptr<Data>
data(const char *id, const std::shared_ptr<Num> &value) {
    return std::make_shared<Data>(std::make_shared<Id>(id), value);
}

std::shared_ptr<Mov>
mov(const std::shared_ptr<LValue> &src, const std::shared_ptr<RValue> &dst) {
    return std::make_shared<Mov>(src, dst);
}

std::shared_ptr<Add>
add(const std::shared_ptr<LValue> &arg1, const std::shared_ptr<RValue> &arg2) {
    return std::make_shared<Add>(arg1, arg2);
}

std::shared_ptr<Sub>
sub(const std::shared_ptr<LValue> &arg1, const std::shared_ptr<RValue> &arg2) {
    return std::make_shared<Sub>(arg1, arg2);
}

std::shared_ptr<Inc> inc(const std::shared_ptr<LValue> &arg) {
    return std::make_shared<Inc>(arg);
}

std::shared_ptr<Dec> dec(const std::shared_ptr<LValue> &arg) {
    return std::make_shared<Dec>(arg);
}

std::shared_ptr<One> one(const std::shared_ptr<LValue> &arg) {
    return std::make_shared<One>(arg);
}

std::shared_ptr<Ones> ones(const std::shared_ptr<LValue> &arg) {
    return std::make_shared<Ones>(arg);
}

std::shared_ptr<Onez> onez(const std::shared_ptr<LValue> &arg) {
    return std::make_shared<Onez>(arg);
}