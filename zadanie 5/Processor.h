#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
    Processor() : zero_flag(false), sign_flag(false) {};

    void setZF(bool flag); // setter zero_flag

    void setSF(bool flag); // setter sign_flag

    bool ZF() const; // getter zero_flag

    bool SF() const; // getter sign_flag

private:
    bool zero_flag;
    bool sign_flag;
};

#endif //PROCESSOR_H
