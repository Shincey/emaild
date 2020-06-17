#ifndef __MIME_HPP__
#define __MIME_HPP__

#include "./types.hpp"

#include <vector>

namespace types {
    enum class MIMEContentType {
            INVALID = 0,
            TEXT_HTML, //HTML文档
            TEXT_PLAIN, //纯文本
            MULTIPART_ALTERNATIVE, //HTML邮件的的HTML形式和纯文本形式，相同内容使用不同形式表示
            MULTIPART_MIXED, //多部分消息
    };

    enum class MIMEContentTransferEncoding {
        MCTE_INVALID = 0,
        MCTE_7_BIT,
        MCTE_8_BIT,
        MCTE_BINARY,
        MCTE_QUOTED_PRINTABLE,
        MCTE_BASE64,
    };

    struct EmailAddress {
        const s8* _name;
        const s8* _addr;
    };

    struct MIMEHeader {
        const s8* _key;
        const s8* _value;
        bool _is_heap = false;
    };

    struct MIMEBody {
        s32 _index;
        const s8* _content;
        std::vector<MIMEHeader> _headers;
        MIMEContentType _content_type;
    };
}

#endif //__MIME_HPP__


