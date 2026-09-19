#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

// [LC: Many A+B]
// https://judge.yosupo.jp/problem/many_aplusb
// https://judge.yosupo.jp/submission/364363

// [LC: Many A+B (128bit)]
// https://judge.yosupo.jp/problem/many_aplusb_128bit
// https://judge.yosupo.jp/submission/364365

namespace fastio {
    template<typename T> struct is_vector : std::false_type {};
    template<typename T, typename Alloc> struct is_vector<std::vector<T, Alloc>> : std::true_type {};

    struct Reader {
        char *p, *l, *r;
        bool mmap_success;
        Reader() : p(nullptr), l(nullptr), r(nullptr), mmap_success(false) {
            struct stat st;
            if (fstat(STDIN_FILENO, &st) == 0 && st.st_size > 0) {
                l = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
                if (l != MAP_FAILED) {
                    p = l; r = l + st.st_size;
                    mmap_success = true;
                } else { l = nullptr; }
            }
        }
        ~Reader() { if (l) munmap(l, r - l); }

        inline void skip_space() { while (p < r && *p <= ' ') p++; }
        
        static constexpr bool all_digit(u64 x) {
            return !((x ^ 0x3030303030303030) & 0xf0f0f0f0f0f0f0f0);
        }

        static constexpr u64 parse_8_digits(u64 val) {
            val ^= 0x3030303030303030;
            val = (val * 10 + (val >> 8)) & 0x00ff00ff00ff00ff;
            val = (val * 100 + (val >> 16)) & 0x0000ffff0000ffff;
            return (val * 10000 + (val >> 32)) & 0x00000000ffffffff;
        }
    } reader_internal;

    struct Table {
        uint32_t data[10000];
        constexpr Table() : data{} {
            for(int i = 0; i < 10000; ++i) {
                uint32_t d0 = (i / 1000) + '0';
                uint32_t d1 = (i / 100 % 10) + '0';
                uint32_t d2 = (i / 10 % 10) + '0';
                uint32_t d3 = (i % 10) + '0';
                data[i] = d0 | (d1 << 8) | (d2 << 16) | (d3 << 24);
            }
        }
    };
    inline constexpr Table table{};

    struct Writer {
        static constexpr int BUF_SIZE = 1 << 20;
        char buf[BUF_SIZE];
        char *p = buf;

        ~Writer() { flush(); }
        inline void flush() {
            if (p > buf) {
                [[maybe_unused]] auto res = write(STDOUT_FILENO, buf, p - buf);
                p = buf;
            }
        }
        inline void write_char(char c) {
            if (p > buf + BUF_SIZE - 1) flush();
            *p++ = c;
        }
        inline void write_str(const char* s, size_t len) {
            if (p + len > buf + BUF_SIZE) flush();
            if (len >= BUF_SIZE) { [[maybe_unused]] auto res = write(STDOUT_FILENO, s, len); } 
            else { std::memcpy(p, s, len); p += len; }
        }
        
        template <typename U>
        inline void write_unsigned(U x) {
            if (p > buf + BUF_SIZE - 40) flush();
            if (x == 0) { *p++ = '0'; return; }
            
            char temp[40];
            char* q = temp + 40;
            
            if constexpr (std::is_same_v<U, u128>) {
                while (x > static_cast<u128>(UINT64_MAX)) {
                    q -= 4;
                    u64 rem = static_cast<u64>(x % 10000);
                    std::memcpy(q, &table.data[rem], 4);
                    x /= 10000;
                }
            }
            
            u64 y = static_cast<u64>(x);
            if (y >= 10000000000000000ull) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            } else if (y >= 1000000000000ull) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            } else if (y >= 100000000ull) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            }
            while (y >= 10000) {
                q -= 4; std::memcpy(q, &table.data[y % 10000], 4); y /= 10000;
            }
            
            if (y >= 1000) {
                q -= 4; std::memcpy(q, &table.data[y], 4);
            } else if (y >= 100) {
                *--q = (y % 10) + '0'; y /= 10;
                *--q = (y % 10) + '0'; y /= 10;
                *--q = y + '0';
            } else if (y >= 10) {
                *--q = (y % 10) + '0';
                *--q = (y / 10) + '0';
            } else {
                *--q = y + '0';
            }
            
            int len = (temp + 40) - q;
            std::memcpy(p, q, len);
            p += len;
        }
    } writer_internal;

    template <typename T>
    inline void scan_single(T& x) {
        if (!reader_internal.mmap_success) {
            std::cin >> x;
            return;
        }
        if constexpr (std::is_same_v<T, char>) {
            reader_internal.skip_space();
            if (reader_internal.p < reader_internal.r) x = *reader_internal.p++;
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
            x = 0; reader_internal.skip_space();
            if (reader_internal.p + 16 <= reader_internal.r) {
                u64 v1, v2;
                std::memcpy(&v1, reader_internal.p, 8);
                std::memcpy(&v2, reader_internal.p + 8, 8);
                bool d1 = Reader::all_digit(v1);
                bool d2 = Reader::all_digit(v2);
                if (d1 && d2) {
                    x = Reader::parse_8_digits(v1) * 100000000ull + Reader::parse_8_digits(v2);
                    reader_internal.p += 16;
                } else if (d1) {
                    x = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            } else if (reader_internal.p + 8 <= reader_internal.r) {
                u64 v1; std::memcpy(&v1, reader_internal.p, 8);
                if (Reader::all_digit(v1)) {
                    x = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            }
            while (*reader_internal.p >= '0') { x = x * 10 + (*reader_internal.p++ - '0'); }
        } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            x = 0; reader_internal.skip_space();
            bool neg = false; if (*reader_internal.p == '-') { neg = true; reader_internal.p++; }
            using U = std::conditional_t<std::is_same_v<T, i128>, u128, std::make_unsigned_t<T>>;
            U ux = 0;
            if (reader_internal.p + 16 <= reader_internal.r) {
                u64 v1, v2;
                std::memcpy(&v1, reader_internal.p, 8);
                std::memcpy(&v2, reader_internal.p + 8, 8);
                bool d1 = Reader::all_digit(v1);
                bool d2 = Reader::all_digit(v2);
                if (d1 && d2) {
                    ux = Reader::parse_8_digits(v1) * 100000000ull + Reader::parse_8_digits(v2);
                    reader_internal.p += 16;
                } else if (d1) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            } else if (reader_internal.p + 8 <= reader_internal.r) {
                u64 v1; std::memcpy(&v1, reader_internal.p, 8);
                if (Reader::all_digit(v1)) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            }
            while (*reader_internal.p >= '0') { ux = ux * 10 + (*reader_internal.p++ - '0'); }
            x = neg ? -static_cast<T>(ux) : static_cast<T>(ux);
        } else if constexpr (std::is_same_v<T, u128> || std::is_same_v<T, i128>) {
            x = 0; reader_internal.skip_space();
            bool neg = false; 
            if constexpr (std::is_same_v<T, i128>) {
                if (*reader_internal.p == '-') { neg = true; reader_internal.p++; }
            }
            u128 ux = 0;
            if (reader_internal.p + 16 <= reader_internal.r) {
                u64 v1, v2;
                std::memcpy(&v1, reader_internal.p, 8);
                std::memcpy(&v2, reader_internal.p + 8, 8);
                bool d1 = Reader::all_digit(v1);
                bool d2 = Reader::all_digit(v2);
                if (d1 && d2) {
                    ux = Reader::parse_8_digits(v1) * 100000000ull + Reader::parse_8_digits(v2);
                    reader_internal.p += 16;
                } else if (d1) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            } else if (reader_internal.p + 8 <= reader_internal.r) {
                u64 v1; std::memcpy(&v1, reader_internal.p, 8);
                if (Reader::all_digit(v1)) {
                    ux = Reader::parse_8_digits(v1);
                    reader_internal.p += 8;
                }
            }
            while (*reader_internal.p >= '0') { ux = ux * 10 + (*reader_internal.p++ - '0'); }
            x = neg ? -static_cast<T>(ux) : static_cast<T>(ux);
        } else if constexpr (std::is_same_v<T, std::string>) {
            x.clear(); reader_internal.skip_space();
            while (*reader_internal.p > ' ') { x += *reader_internal.p++; }
        }
    }

    template <typename... Args> inline void scan(Args&... args) { (scan_single(args), ...); }

    template <typename T>
    inline void print_single(const T& x) {
        if constexpr (std::is_same_v<T, char>) {
            writer_internal.write_char(x);
        } else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>) {
            writer_internal.write_unsigned(x);
        } else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            using U = std::make_unsigned_t<T>;
            if (x < 0) { writer_internal.write_char('-'); writer_internal.write_unsigned(static_cast<U>(~static_cast<U>(x) + 1)); } 
            else { writer_internal.write_unsigned(static_cast<U>(x)); }
        } else if constexpr (std::is_same_v<T, u128>) {
            writer_internal.write_unsigned(x);
        } else if constexpr (std::is_same_v<T, i128>) {
            if (x < 0) { writer_internal.write_char('-'); writer_internal.write_unsigned(static_cast<u128>(~static_cast<u128>(x) + 1)); } 
            else { writer_internal.write_unsigned(static_cast<u128>(x)); }
        } else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
            writer_internal.write_str(x, std::strlen(x));
        } else if constexpr (std::is_same_v<T, std::string>) {
            writer_internal.write_str(x.c_str(), x.size());
        } else if constexpr (is_vector<T>::value) {
            for (size_t i = 0; i < x.size(); ++i) {
                if (i > 0) writer_internal.write_char(' ');
                print_single(x[i]);
            }
        }
    }

    inline void print() {}
    template <typename Head, typename... Tail>
    inline void print(const Head& h, const Tail&... t) {
        print_single(h);
        if constexpr (sizeof...(t) > 0) {
            writer_internal.write_char(' ');
            print(t...);
        }
    }
    template <typename... Args>
    inline void println(const Args&... args) {
        print(args...);
        writer_internal.write_char('\n');
    }
    
    inline void flush() { writer_internal.flush(); }
} // namespace fastio