

#include "utils.h"
void dump_hex(const char *buf, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", (unsigned char)buf[i]);
        if (i % 16 == 15) printf("\n");
    }
    printf("\n");
}

void dump_ppm(const char *buf, size_t len, size_t ancho, size_t alto, const char *filename) {
    FILE *f = fopen(filename, "w");
    fprintf(f, "P3 %zu %zu 255\n", ancho, alto);
    for (size_t i = 0; i < len; i++) {
        fprintf(f, "%d ", (unsigned char)buf[i]);
        if (i % 3 == 2) fprintf(f, "\n");
    }
    fclose(f);
}