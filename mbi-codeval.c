#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const bool SHOW_RANGE = false;

static const char
    *prog,
    symbols[] = "0123456789ACDEFGHJKMNPQRTUVWXYZ",
    numbers[] = "0123456789",
    *const letters = symbols + 10,
    *const digits[] = {
        numbers + 1,
        letters, symbols, numbers,
        letters, symbols, numbers,
        letters, letters, numbers, numbers,
    };
static const unsigned
    N = sizeof(digits)/sizeof(*digits),
    NNUMBERS = sizeof(numbers) - 1,
    NSYMBOLS = sizeof(symbols) - 1,
    NLETTERS = sizeof(symbols) - sizeof(numbers),
    BUFFER_SIZE = (10000 / (N+1)) * (N+1),
    sizes[] = {
        NNUMBERS - 1,
        NLETTERS, NSYMBOLS, NNUMBERS,
        NLETTERS, NSYMBOLS, NNUMBERS,
        NLETTERS, NLETTERS, NNUMBERS, NNUMBERS,
    };
static char *buffer, *bufp;

static void usage() {
    fprintf(stderr, "Usage: %s starting-code count\n", prog);
    exit(EINVAL);
}

static unsigned long parse(const char *src) {
    unsigned long x = 0, fac = 1;
    for (int i = 0; i < N; i++) {
        const char *found = strchr(digits[i], src[i]);
        if (!found) {
            fprintf(stderr, "Invalid character '%c' in code string position %d\n", src[i], i);
            usage();
        }
        x += fac*(found - digits[i]);
        fac *= sizes[i];
    }
    return x;
}

/*
// This is slow

static void print_single(const uint8_t code[N]) {
    for (int i = 0; i < N; i++)
        putchar(digits[i][code[i]]);
    putchar('\n');
}
*/

static void print_buffer(unsigned long x) {
    for (int i = N-1; i >= 0; i--) {
        bufp[i] = digits[i][x % sizes[i]];
        x /= sizes[i];
    }
    bufp[N] = '\n';
    bufp += N + 1;
}

static unsigned long range() {
    unsigned long r = 1;
    for (int i = 0; i < N; i++)
        r *= sizes[i];
    return r;
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
    
    unsigned long code = parse(argv[1]), count;
    if (sscanf(argv[2], "%lu", &count) != 1) {
        fprintf(stderr, "Couldn't parse count '%s'\n", argv[2]);
        usage();
    }
    
    if (SHOW_RANGE)
        fprintf(stderr, "%lu codes possible\n", range());
    
    buffer = malloc(BUFFER_SIZE);
    assert(buffer);
    bufp = buffer;
    
    for (int i = 0; i < count; i++, code++) {
        print_buffer(code);
        if (bufp >= buffer + BUFFER_SIZE)
            bflush();
    }
    
    bflush();
    return 0;
}

