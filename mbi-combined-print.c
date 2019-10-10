#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

static void parse(const char *src, unsigned dest[N]) {
    for (int i = 0; i < N; i++) {
        const char *found = strchr(digits[i], src[i]);
        if (!found) {
            fprintf(stderr, "Invalid character '%c' in code string position %d\n", src[i], i);
            usage();
        }
        dest[i] = found - digits[i];
    }
}

static void print_and_inc(unsigned code[N]) {
    int incr = 1;
    for (int p = N-1; p >= 0; p--) {
        bufp[p] = digits[p][code[p]];
        if (incr) {
            incr &= ++code[p] >= sizes[p];
            if (incr) {
                if (p == 0) {
                    fprintf(stderr, "Won't increment past last valid code\n");
                    exit(E2BIG);
                }
                code[p] = 0;
            }
        }
    }
    bufp += N;
    *bufp++ = '\n';
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
    
    unsigned code[N];
    parse(argv[1], code);
    
    unsigned long count;
    if (sscanf(argv[2], "%lu", &count) != 1) {
        fprintf(stderr, "Couldn't parse count '%s'\n", argv[2]);
        usage();
    }
    
    buffer = malloc(BUFFER_SIZE);
    assert(buffer);
    bufp = buffer;
    
    for (unsigned long i = 0; i < count; i++) {
        print_and_inc(code);
        if (bufp >= buffer + BUFFER_SIZE)
            bflush();
    }
    
    bflush();
    return 0;
}

