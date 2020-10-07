#ifndef SWRACE_H
#define SWRACE_H

#ifdef __cplusplus
// C++ code
using namespace std;

namespace helpers {
    long long get_elapsed_time(void);
    bool has_reached_timeout(void);

    void setup(void);
}

#ifndef GRADER_CPP

__attribute__((weak))
int __real_main();

int main() {
    helpers::setup();
    return __real_main();
}

#define main __real_main

#endif

#else
// C code
#include <stdbool.h>

long long get_elapsed_time(void);
bool has_reached_timeout(void);

void setup(void);

#error "Only C++ is supported"
#endif

#endif
#pragma once
