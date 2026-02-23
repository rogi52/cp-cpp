#include "template.hpp"
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <type_traits>

namespace fastio {
struct Reader {
    char *p, *l, *r;
    Reader() : l(nullptr), p(nullptr), r(nullptr) {
        struct stat st;
        if (fstat(STDIN_FILENO, &st) == 0 && st.st_size > 0) {
            l = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
            if (l != MAP_FAILED) {
                p = l;
                r = l + st.st_size;
            } else {
                l = nullptr;
            }
        }
    }
    ~Reader() { if (l) munmap(l, r - l); }

    static constexpr bool is_digit(char c) { return '0' <= c && c <= '9'; }
    
    static bool all_digit(u64 x) {
        return !((x ^ 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0);
    }

    u64 u64_read() {
        if (!p) return 0;
        while (p < r && !is_digit(*p)) p++;
        if (p >= r) return 0;
        u64 x = 0;
        if (p + 8 <= r) {
            u64 a;
            std::memcpy(&a, p, 8);
            if (all_digit(a)) {
                a ^= 0x3030303030303030;
                a = (a * 10 + (a >> 8)) & 0x00ff00ff00ff00ff;
                a = (a * 100 + (a >> 16)) & 0x0000ffff0000ffff;
                a = (a * 10000 + (a >> 32)) & 0x00000000ffffffff;
                x = a; p += 8;
            }
        }
        while (p < r && is_digit(*p)) x = x * 10 + (*p++ - '0');
        return x;
    }

    i64 i64_read() {
        if (!p) return 0;
        while (p < r && !is_digit(*p) && *p != '-') p++;
        if (p >= r) return 0;
        bool neg = false;
        if (*p == '-') { neg = true; p++; }
        u64 x = u64_read();
        return neg ? -static_cast<i64>(x) : static_cast<i64>(x);
    }
} reader_internal;

struct Table {
    char data[10000][4];
    constexpr Table() : data{} {
        for(int i=0; i<10000; ++i) {
            data[i][0] = (i / 1000) + '0';
            data[i][1] = (i / 100 % 10) + '0';
            data[i][2] = (i / 10 % 10) + '0';
            data[i][3] = (i % 10) + '0';
        }
    }
};
inline constexpr Table table{};

struct Writer {
    static constexpr int BUF_SIZE = 1 << 20;
    char buf[BUF_SIZE];
    char *p = buf;

    ~Writer() { flush(); }
    void flush() { if (p > buf) { write(STDOUT_FILENO, buf, p - buf); p = buf; } }

    void write_u64(u64 x) {
        if (p > buf + BUF_SIZE - 24) flush();
        if (x == 0) { *p++ = '0'; return; }
        
        char temp[24];
        int i = 0;
        while (x >= 10000) {
            const char* d = table.data[x % 10000];
            temp[i++] = d[3]; temp[i++] = d[2];
            temp[i++] = d[1]; temp[i++] = d[0];
            x /= 10000;
        }
        if (x >= 1000) {
            const char* d = table.data[x];
            temp[i++] = d[3]; temp[i++] = d[2];
            temp[i++] = d[1]; temp[i++] = d[0];
        } else if (x >= 100) {
            temp[i++] = (x % 10) + '0'; x /= 10;
            temp[i++] = (x % 10) + '0'; x /= 10;
            temp[i++] = x + '0';
        } else if (x >= 10) {
            temp[i++] = (x % 10) + '0';
            temp[i++] = (x / 10) + '0';
        } else {
            temp[i++] = x + '0';
        }
        while (i > 0) *p++ = temp[--i];
    }

    void write_char(char c) {
        if (p > buf + BUF_SIZE - 1) flush();
        *p++ = c;
    }
} writer_internal;

inline u64 in_u64() { return reader_internal.u64_read(); }
inline i64 in_i64() { return reader_internal.i64_read(); }
inline void out_u64(u64 x) { writer_internal.write_u64(x); }
inline void out_char(char c) { writer_internal.write_char(c); }
inline void flush() { writer_internal.flush(); }
}