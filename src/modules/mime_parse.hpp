#ifndef __MIME_PARSE_H__
#define __MIME_PARSE_H__

#include "./email.hpp"

namespace modules::mime {
    /**
     * Parses an raw mime message, and stores it into an email
     * @param raw
     * @param target
     * @return
     */
    char parseMessage(const char* raw, modules::Email& target);
}

#endif //__MIME_PARSE_H__