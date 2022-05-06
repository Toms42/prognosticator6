#ifndef MUSICUTIL_H
#define MUSICUTIL_H

#include <cmath>

inline double st_to_freq(double st) {
    return pow(2, (st-69)/12)*440;
}

inline double freq_to_st(double freq) {
    return 12*log2(freq/440)+69;
}

#endif // MUSICUTIL_H
