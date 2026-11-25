#include <stdio.h>

enum {
    KEY_BYTES = 10,
    MAX_DATA_BYTES = 256,
    WARMUP_CLOCKS = 1152,
    VECTOR_BYTES = 16
};

int main(int argc, char *argv[])
{
    /* eSTREAM, first 16 bytes of stream[0..63]. */
    const char *vectors[][3] = {
        {"80000000000000000000", "00000000000000000000",
         "38EB86FF730D7A9CAF8DF13A4420540D"},
        {"00000000000000000000", "00000000000000000000",
         "FBE0BF265859051B517A2E4E239FC97F"},
        {"0053A6F94C9FF24598EB", "0D74DB42A91077DE45AC",
         "F4CD954A717F26A7D6930830C4E7CF08"}
    };
    const char *vector_names[] = {"Set 1 #0", "Set 2 #0", "Set 6 #0"};
    const char zero_input[] = "00000000000000000000000000000000";
    const char hex[] = "0123456789ABCDEF";
    const int vector_count = (int)(sizeof vectors / sizeof vectors[0]);
    int test_mode = argc == 1;
    int failure_count = 0;

    if (!test_mode && argc != 4) {
        fprintf(stderr, "Usage: %s [KEY_HEX IV_HEX DATA_HEX]\n", argv[0]);
        fprintf(stderr, "KEY and IV: 20 hex digits each. DATA: 1 to 256 bytes in hex.\n");
        return 2;
    }

    for (int test = 0; test < (test_mode ? vector_count : 1); ++test) {
        const char *inputs[3] = {
            test_mode ? vectors[test][0] : argv[1],
            test_mode ? vectors[test][1] : argv[2],
            test_mode ? zero_input : argv[3]
        };
        unsigned char key[KEY_BYTES] = {0};
        unsigned char iv[KEY_BYTES] = {0};
        unsigned char data[MAX_DATA_BYTES] = {0};
        unsigned char *buffers[] = {key, iv, data};
        const int capacities[] = {KEY_BYTES, KEY_BYTES, MAX_DATA_BYTES};
        const char *field_names[] = {"KEY", "IV", "DATA"};
        int data_bytes = 0;

        /* Decode hexadecimal without string or conversion libraries. */
        for (int field = 0; field < 3; ++field) {
            int digit = 0;
            while (inputs[field][digit] != '\0') {
                char ch = inputs[field][digit];
                int value = 0;
                while (value < 16 && ch != hex[value]
                       && !(value >= 10 && ch == "abcdef"[value - 10])) {
                    ++value;
                }
                if (digit >= 2 * capacities[field] || value == 16) {
                    fprintf(stderr, "Invalid %s: bad hex digit or field too long.\n",
                            field_names[field]);
                    return 2;
                }
                buffers[field][digit / 2] |= (unsigned char)(value << (digit % 2 ? 0 : 4));
                ++digit;
            }
            if (digit == 0 || digit % 2 != 0 || (field < 2 && digit != 2 * KEY_BYTES)) {
                fprintf(stderr, "Invalid %s: wrong number of hex digits.\n", field_names[field]);
                return 2;
            }
            if (field == 2) {
                data_bytes = digit / 2;
            }
        }

        /* Unused s[0] keeps subscripts identical to the specification. */
        unsigned char s[289] = {0};
        for (int bit = 0; bit < 80; ++bit) {
            int source = 79 - bit;
            s[1 + bit] = (unsigned char)((key[source / 8] >> (source % 8)) & 1);
            s[94 + bit] = (unsigned char)((iv[source / 8] >> (source % 8)) & 1);
        }
        s[286] = s[287] = s[288] = 1;

        for (int clock = 0; clock < WARMUP_CLOCKS + 8 * data_bytes; ++clock) {
            int t1 = s[66] ^ s[93];
            int t2 = s[162] ^ s[177];
            int t3 = s[243] ^ s[288];
            if (clock >= WARMUP_CLOCKS) {
                int output_bit = clock - WARMUP_CLOCKS;
                data[output_bit / 8] ^= (unsigned char)((t1 ^ t2 ^ t3) << (output_bit % 8));
            }

            /* Read every tap before updating any part of the state. */
            t1 ^= (s[91] & s[92]) ^ s[171];
            t2 ^= (s[175] & s[176]) ^ s[264];
            t3 ^= (s[286] & s[287]) ^ s[69];

            for (int i = 288; i > 1; --i) {
                s[i] = s[i - 1];
            }
            s[1] = (unsigned char)t3;
            s[94] = (unsigned char)t1;
            s[178] = (unsigned char)t2;
        }

        if (!test_mode) {
            for (int i = 0; i < data_bytes; ++i) {
                printf("%02X", (unsigned int)data[i]);
            }
            putchar('\n');
            continue;
        }

        int match = data_bytes == VECTOR_BYTES;
        printf("%s\nKEY     %s\nIV      %s\nINPUT   %s\nEXPECT  %s\nOUTPUT  ",
               vector_names[test], inputs[0], inputs[1], inputs[2], vectors[test][2]);
        for (int i = 0; i < data_bytes; ++i) {
            unsigned int byte = data[i];
            printf("%02X", byte);
            if (i >= VECTOR_BYTES || hex[byte >> 4] != vectors[test][2][2 * i]
                || hex[byte & 15] != vectors[test][2][2 * i + 1]) {
                match = 0;
            }
        }
        printf("\n%s\n\n", match ? "PASS" : "FAIL");
        failure_count += !match;
    }

    if (test_mode) {
        printf("Result: %d/%d vectors passed.\n", vector_count - failure_count, vector_count);
    }
    return failure_count != 0;
}
