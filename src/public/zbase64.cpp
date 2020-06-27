#include "zbase64.h"

namespace lib {

    const char* base64_chars[2] = {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "+/",

        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "-_"};

    static inline std::size_t pos_of_char(const unsigned char __chr) {
        if      (__chr >= 'A' && __chr <= 'Z') return __chr - 'A';
        else if (__chr >= 'a' && __chr <= 'z') return __chr - 'a' + ('Z' - 'A')               + 1;
        else if (__chr >= '0' && __chr <= '9') return __chr - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
        else if (__chr == '+' || __chr == '-') return 62;
        else if (__chr == '/' || __chr == '_') return 63;
        else return 64; // invalid pos
    }

    static inline std::string insert_linebreaks(std::string __str, std::size_t __distance) {
        if (!__str.length()) {
            return "";
        }
        std::size_t pos = __distance;
        while (pos < __str.size()) {
            __str.insert(pos, "\n");
            pos += __distance + 1;
        }
        return __str;
    }

    // template <typename T, std::size_t __length>
    // static std::string encode_with_line_breaks(T __s) {
    //     return insert_linebreaks(base64_encode(__s, false), __length);
    // }

    // template <typename T>
    // static std::string encode_pem(T __s) {
    //     return encode_with_line_breaks<T, 64>(__s);
    // }

    // template <typename T>
    // static std::string encode_mime(T s) {
    //     return encode_with_line_breaks<T, 76>(s);
    // }

    // template <typename T>
    // static std::string encode(T __s, bool __url) {
    //     return base64_encode(reinterpret_cast<const unsigned char*>(__s.data()), __s.length(), __url);
    // }

    /**
     * base64 encode
     * @param __plain : bytes to encode
     * @param __len : length of
     * @param __url : choose the set of base64 characters
     * @return returns based64 encoded string
     */
    std::string base64_encode(unsigned char const *__plain, std::size_t __len, bool __url) {

        std::size_t len_encoded = (__len + 2) / 3 * 4;

        unsigned char trailing_char = __url ? '.' : '=';

        const char *base64_chars_set = base64_chars[__url];

        std::string ret;
        ret.reserve(len_encoded);

        unsigned int pos = 0;

        while (pos < __len) {
            ret.push_back(base64_chars_set[(__plain[pos + 0] & 0xfc) >> 2]);

            if (pos + 1 < __len) {
               ret.push_back(base64_chars_set[((__plain[pos + 0] & 0x03) << 4) + ((__plain[pos + 1] & 0xf0) >> 4)]);
               if (pos+2 < __len) {
                  ret.push_back(base64_chars_set[((__plain[pos + 1] & 0x0f) << 2) + ((__plain[pos + 2] & 0xc0) >> 6)]);
                  ret.push_back(base64_chars_set[__plain[pos + 2] & 0x3f]);
               } else {
                  ret.push_back(base64_chars_set[(__plain[pos + 1] & 0x0f) << 2]);
                  ret.push_back(trailing_char);
               }
            }
            else {
                ret.push_back(base64_chars_set[(__plain[pos + 0] & 0x03) << 4]);
                ret.push_back(trailing_char);
                ret.push_back(trailing_char);
            }

            pos += 3;
        }

        return ret;
    }


    std::string base64_decode(std::string const &__cipher, bool __remove_linebreaks) {
        if (__remove_linebreaks) {
            if (!__cipher.length() ) {
                return "";
            }
            std::string copy(__cipher);
            size_t pos=0;
            while ((pos = copy.find("\n", pos)) != std::string::npos) {
               copy.erase(pos, 1);
            }
            return base64_decode(copy, false);
        }

        size_t len_of_cipher = __cipher.length();
        if (!len_of_cipher) { return std::string(""); }

        size_t in_len = len_of_cipher;
        size_t pos = 0;

        size_t approx_len_of_plain = len_of_cipher / 4 * 3;
        std::string ret;
        ret.reserve(approx_len_of_plain);

        while (pos < in_len) {
           unsigned int chr1_pos = pos_of_char(__cipher[pos + 1]);
           ret.push_back(static_cast<std::string::value_type>(((pos_of_char(__cipher[pos+0])) << 2) + ((chr1_pos & 0x30) >> 4)));
           if (__cipher[pos+2] != '=' && __cipher[pos+2] != '.') {
              unsigned int chr2_pos = pos_of_char(__cipher[pos+2] );
              ret.push_back(static_cast<std::string::value_type>( (( chr1_pos & 0x0f) << 4) + (( chr2_pos & 0x3c) >> 2)));
              if (__cipher[pos+3] != '=' && __cipher[pos+3] != '.') {
                 ret.push_back(static_cast<std::string::value_type>(((chr2_pos & 0x03 ) << 6 ) + pos_of_char(__cipher[pos+3])));
              }
           }
           pos += 4;
        }
        return ret;
    }
    
}