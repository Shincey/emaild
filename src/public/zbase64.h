#ifndef __ZBASE64_H__
#define __ZBASE64_H__

#include <string>

namespace lib {

    // std::string base64_encode     (std::string const& s, bool url = false);
    // std::string base64_encode_pem (std::string const& s);
    // std::string base64_encode_mime(std::string const& s);

    std::string base64_decode(std::string const &__cipher, bool __remove_linebreaks=false);
    std::string base64_encode(unsigned char const *__plain, std::size_t __len, bool __url=false);
}

#endif // __ZBASE64_H__