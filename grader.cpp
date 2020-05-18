#ifndef GRADER_CPP
#define GRADER_CPP

#include <chrono>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <csignal>
#include <sys/time.h>
#include <sys/resource.h>
#include "swrace.h"

using namespace std;


/*
 * Very simple debugger, enabled at compile time with -DDEBUG.
 * If enabled, it prints on stderr, otherwise it does nothing (it does not
 * even evaluate the expression on its right-hand side).
 *
 * Main ideas taken from:
 *   - C++ enable/disable debug messages of std::couts on the fly
 *     (https://stackoverflow.com/q/3371540/2377454)
 *   - Standard no-op output stream
 *     (https://stackoverflow.com/a/11826787/2377454)
*/
#ifdef DEBUG
#define debug true
#else
#define debug false
#endif


#define debug_logger if (!debug) \
                        {} \
                     else \
                        cerr << "[DEBUG] helpers::"

// conversion factor betwen seconds and nanoseconds
#define NANOS 1000000000

// signal to handle
#define SIGNAL SIGXCPU
#define TIMELIMIT RLIMIT_CPU

namespace helpers {
    long long start_time = -1;
    volatile sig_atomic_t timeout_flag = false;
    unsigned const timelimit = 3;   // soft limit on CPU time (in seconds)

    void setup_signal(void);
    void setup_time_limit(void);

    static void signal_handler(int signum);
}

/*
 * This could be a function factory where and a closure of the signal-handling
 * function so that we could explicitly pass the output ofstream and close it.
 * C++ support closures only for lambdas, alas, at the moment we also need
 * the signal-handling function to be a pointer to a function and lambaa are
 * a different object that can not be converted. See:
 *  - Passing lambda as function pointer
 *    (https://stackoverflow.com/a/28746827/2377454)
*/
void helpers::signal_handler(int signum) {
    helpers::timeout_flag = true;

    if(debug) {
        debug_logger << "signal_handler:\t" << "signal " << signum \
                     << " received" << endl;
        debug_logger << "signal_handler:\t" << "exiting after " \
                     << helpers::get_elapsed_time() << " microseconds" << endl;
    }

    quick_exit(EXIT_SUCCESS);
}


/*
 * Set function signal_handler() as handler for SIGXCPU using sigaction. See
 *   - https://stackoverflow.com/q/4863420/2377454
 *   - https://stackoverflow.com/a/17572787/2377454
*/
void helpers::setup_signal() {
    debug_logger << "set_signal:\t" << "set_signal() called" << endl;

    struct sigaction new_action;

    //Set the handler in the new_action struct
    new_action.sa_handler = signal_handler;

    // Set to empty the sa_mask. It means that no signal is blocked
    // while the handler run.
    sigemptyset(&new_action.sa_mask);

    // Block the SIGXCPU signal, while the handler run, SIGXCPU is ignored.
    sigaddset(&new_action.sa_mask, SIGNAL);

    // Remove any flag from sa_flag
    new_action.sa_flags = 0;

    // Set new action
    sigaction(SIGNAL,&new_action,NULL);

    if(debug) {
        struct sigaction tmp;
        // read the old signal associated to SIGXCPU
        sigaction(SIGNAL, NULL, &tmp);
        debug_logger << "set_signal:\t" << "action.sa_handler: " \
                     << tmp.sa_handler << endl;
    }

    return;
}


/*
 * Set soft CPU time limit.
 * RLIMIT_CPU set teg CPU time limit in seconds..
 * See:
 *   - https://www.go4expert.com/articles/
 *         getrlimit-setrlimit-control-resources-t27477/
 *   - https://gist.github.com/Leporacanthicus/11086960
*/
void helpers::setup_time_limit(void) {
    debug_logger << "set_limit:\t\t" << "set_limit() called" << endl;

    struct rlimit limit;
    if(getrlimit(TIMELIMIT, &limit) != 0) {
        perror("error calling getrlimit()");
        exit(EXIT_FAILURE);
    }

    limit.rlim_cur = helpers::timelimit;

    if(setrlimit(TIMELIMIT, &limit) != 0) {
        perror("error calling setrlimit()");
        exit(EXIT_FAILURE);
    }

    if (debug) {
        struct rlimit tmp;
        getrlimit(TIMELIMIT, &tmp);
        debug_logger << "set_limit:\t\t" << "current limit: " << tmp.rlim_cur \
                     << " seconds" << endl;
    }

    return;
}


void helpers::setup(void) {
    struct timespec start;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start)) {
        exit(EXIT_FAILURE);
    }

    start_time = start.tv_sec*NANOS + start.tv_nsec;

    setup_signal();
    setup_time_limit();

    return;
}


long long helpers::get_elapsed_time(void) {

    struct timespec current;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &current)) {
        exit(EXIT_FAILURE);
    }

    long long current_time = current.tv_sec*NANOS + current.tv_nsec;
    long long elapsed_micro = (current_time - start_time)/1000 + \
                                ((current_time - start_time) % 1000 >= 500);

    return elapsed_micro;
}


bool helpers::has_reached_timeout(void) {
    return helpers::timeout_flag;
}

#endif
