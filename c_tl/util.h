#include <sys/ioctl.h> // ioctl, TIOCGWINSZ
#include <unistd.h>
#ifdef WIN32
#include <stdio.h>
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>
#endif

#define max(a, b) ({\
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; \
    })

#define min(a, b) ({ \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; \
    })

// sleep for `n` milliseconds
// stolen from:
// https://stackoverflow.com/a/28827188
void sleep_ms(int milliseconds) { // cross-platform sleep function
    #ifdef WIN32
        Sleep(milliseconds);
    #elif _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        nanosleep(&ts, NULL);
    #else
        if (milliseconds >= 1000) {
            sleep(milliseconds / 1000);
        }
        usleep((milliseconds % 1000) * 1000);
    #endif
}

struct winsize;

// gets the terminal size in (rows == ws_row, columns == ws_col)
struct winsize get_term_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w;
}