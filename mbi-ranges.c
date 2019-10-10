#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 11

static const char
    *prog,
    symbols[] = "0123456789ACDEFGHJKMNPQRTUVWXYZ",
    numbers[] = "0123456789",
    *const letters = symbols + 10,
    *const digits[N] = {
        numbers + 1,
        letters, symbols, numbers,
        letters, symbols, numbers,
        letters, letters, numbers, numbers,
    };
static const unsigned
    BUFFER_SIZE = (10000 / (N+1)) * (N+1);
static unsigned sizes[N];
static unsigned long facs[N];
static char *buffer, *bufp;

static void init() {
    unsigned long fac = 1;
    for (int i = N-1; i > 0; i--) {
        sizes[i] = strlen(digits[i]);
        facs[i] = fac;
        fac *= sizes[i];
    }
    
    buffer = malloc(BUFFER_SIZE);
    assert(buffer);
    bufp = buffer;
}

static void usage() {
    fprintf(stderr, "Usage: %s starting-code count\n", prog);
    exit(EINVAL);
}

static void parse(const char *src, unsigned code[N], unsigned long within_digit[N]) {
    unsigned long sum = 0;
    for (int i = N-1; i >= 0; i--) {
        const char *found = strchr(digits[i], src[i]);
        if (!found) {
            fprintf(stderr, "Invalid character '%c' in code string position %d\n", src[i], i);
            usage();
        }
        
        unsigned d = found - digits[i];
        code[i] = d;
        within_digit[i] = sum;
        sum += d*facs[i];
    }
}

static void print_and_incr(unsigned code[N], unsigned long within_digit[N]) {
    for (int i = N-1; i >= 0; i--) {
        bufp[i] = digits[i][code[i]];
        if (++within_digit[i] >= facs[i]) {
            within_digit[i] = 0;
            if (++code[i] >= sizes[i])
                code[i] = 0;
        }
    }
    bufp += N;
    *bufp = '\n';
    bufp++;
}

static void bflush() {
    unsigned long n = bufp - buffer;
    assert(write(STDOUT_FILENO, buffer, n) == n);
    bufp = buffer;
}

int main(int argc, const char **argv) {
    prog = argv[0];
    if (argc != 3)
        usage();
    
    init();
    
    unsigned code[N];
    unsigned long within_digit[N];
    parse(argv[1], code, within_digit);
    
    unsigned long count;
    if (sscanf(argv[2], "%lu", &count) != 1) {
        fprintf(stderr, "Couldn't parse count '%s'\n", argv[2]);
        usage();
    }
    
    for (unsigned long i = 0; i < count; i++) {
        print_and_incr(code, within_digit);
        if (bufp >= buffer + BUFFER_SIZE)
            bflush();
    }
    
    bflush();
    return 0;
}

