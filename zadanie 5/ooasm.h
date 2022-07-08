#ifndef OOASM_H
#define OOASM_H

#include "Processor.h"
#include "Memory.h"
#include "Id.h"
#include <memory>

class ElementASM {
public:
    virtual void init(Memory &m) = 0;

    virtual ~ElementASM() = default;
};

class RValue : public ElementASM {
public:
    // Zwraca wartość intepretowaną jako słowo
    virtual word_t getWordValue() const = 0;

    // Zwraca wartość interpretowaną jako adres
    virtual address_t getAddressValue() const = 0;

    ~RValue() override = default;;
};

class LValue : public RValue {
public:
    virtual void setValue(word_t value) = 0;

    ~LValue() override = default;
};

class Lea : public RValue {
private:
    std::shared_ptr<Id> val;
    address_t variable_addr;
public:

    explicit Lea(std::shared_ptr<Id> id) : val(std::move(id)), variable_addr(0) {}

    void init(Memory &m) override;

    word_t getWordValue() const override;

    address_t getAddressValue() const override;

};

class Num : public RValue {
private:
    word_t val;
public:
    explicit Num(word_t val) : val(val) {}

    void init(Memory&) override {}

    word_t getWordValue() const override;

    address_t getAddressValue() const override;
};

class Mem : public LValue {
public:

    explicit Mem(std::shared_ptr<RValue> addr) : rVal(std::move(addr)),
                                                 memory_ptr(nullptr) {}

    void init(Memory &m) override;

    word_t getWordValue() const override;

    address_t getAddressValue() const override;

    void setValue(word_t value) override;

private:
    std::shared_ptr<RValue> rVal;
    word_t *memory_ptr;
};

class Instruction {
public:
    virtual void execute(Memory &m, Processor &p) = 0;

    virtual bool hasPriority() const { return false; }

    virtual ~Instruction() = default;
};

class Data : public Instruction {
public:

    Data(std::shared_ptr<Id> id, std::shared_ptr<Num> value)
            : id(std::move(id)), value(std::move(value)) {}


    void execute(Memory &m, Processor &p) override;

    bool hasPriority() const override { return true; }

private:
    std::shared_ptr<Id> id;
    std::shared_ptr<Num> value;
};

class Arithmetic : public Instruction {
public:
    Arithmetic(std::shared_ptr<LValue> dst, std::shared_ptr<RValue> src)
            : lVal(std::move(dst)), rVal(std::move(src)) {}

    ~Arithmetic() override = default;

    void execute(Memory &m, Processor &p) override;

    virtual word_t calculate(word_t arg1, word_t arg2) const = 0;

private:
    std::shared_ptr<LValue> lVal;
    std::shared_ptr<RValue> rVal;

    void setZF(Processor &p, word_t result);

    void setSF(Processor &p, word_t result);
};

class Add : public Arithmetic {
public:
    Add(std::shared_ptr<LValue> dst, std::shared_ptr<RValue> src) :
            Arithmetic(std::move(dst), std::move(src)) {}

    word_t calculate(word_t arg1, word_t arg2) const override;
};

class Sub : public Arithmetic {
public:
    Sub(std::shared_ptr<LValue> dst, std::shared_ptr<RValue> src) :
            Arithmetic(std::move(dst), std::move(src)) {}

    word_t calculate(word_t arg1, word_t arg2) const override;
};

class Inc : public Add {
public:
    explicit Inc(std::shared_ptr<LValue> lVal)
            : Add(std::move(lVal), std::make_shared<Num>(1)) {}
};

class Dec : public Sub {
public:
    explicit Dec(std::shared_ptr<LValue> lVal)
            : Sub(std::move(lVal), std::make_shared<Num>(1)) {}
};

class Mov : public Instruction {
public:
    Mov(std::shared_ptr<LValue> dst, std::shared_ptr<RValue> src)
            : lVal(std::move(dst)), rVal(std::move(src)) {}


    void execute(Memory &m, Processor &p) override;

private:
    std::shared_ptr<LValue> lVal;
    std::shared_ptr<RValue> rVal;
};


class One : public Mov {
public:
    explicit One(std::shared_ptr<LValue> lVal)
                : Mov(std::move(lVal),std::make_shared<Num>(1)) {}
};

class Onez : public One {
public:
    explicit Onez(std::shared_ptr<LValue> lVal) : One(std::move(lVal)) {}

    void execute(Memory &m, Processor &p) override;
};

class Ones : public One {
public:
    explicit Ones(std::shared_ptr<LValue> lVal) : One(std::move(lVal)) {}

    void execute(Memory &m, Processor &p) override;
};

/* Funkcje tworzące obiekty Mem, Num, Lea i instrukcje: */

std::shared_ptr<Num> num(word_t);

std::shared_ptr<Mem> mem(const std::shared_ptr<RValue> &);

std::shared_ptr<Lea> lea(const char *);

std::shared_ptr<Data>
data(const char *, const std::shared_ptr<Num> &);

std::shared_ptr<Mov>
mov(const std::shared_ptr<LValue> &, const std::shared_ptr<RValue> &);

std::shared_ptr<Add>
add(const std::shared_ptr<LValue> &, const std::shared_ptr<RValue> &);

std::shared_ptr<Sub>
sub(const std::shared_ptr<LValue> &, const std::shared_ptr<RValue> &);

std::shared_ptr<Inc> inc(const std::shared_ptr<LValue> &);

std::shared_ptr<Dec> dec(const std::shared_ptr<LValue> &);

std::shared_ptr<One> one(const std::shared_ptr<LValue> &);

std::shared_ptr<Ones> ones(const std::shared_ptr<LValue> &);

std::shared_ptr<Onez> onez(const std::shared_ptr<LValue> &);

#endif //OOASM_H
