#ifndef SIMPLERANDOM_H
#define SIMPLERANDOM_H

#include <QtNumeric>

// from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
inline quint32 simpleRand(quint32 x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
};

inline quint32 inverseRand(quint32 x) {
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = ((x >> 16) ^ x) * 0x119de1f3;
    x = (x >> 16) ^ x;
    return x;
};


#endif // SIMPLERANDOM_H
