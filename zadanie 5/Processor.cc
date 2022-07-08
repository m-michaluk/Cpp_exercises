#include "Processor.h"

void Processor::setZF(bool flag) { zero_flag = flag; }

void Processor::setSF(bool flag) { sign_flag = flag; }

bool Processor::ZF() const { return zero_flag; }

bool Processor::SF() const { return sign_flag; }