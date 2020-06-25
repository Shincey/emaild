#ifndef __EMAIL_HPP__
#define __EMAIL_HPP__

#include "../types/mime.hpp"
#include "../types/types.hpp"

#include <vector>
#include <string>

namespace modules {
    
    enum class EmailContentType {
        EMIAL_CT_MULTIPART_ALTERNATIVE,
        EMIAL_CT_TEXT_PLAIN,
        EMAIL_CT_TEXT_HTML,
    };

    enum class EmailContentSectionType {
        EMAIL_CS_HTML,
        EMAIL_CS_TEXT_PLAIN,
    };

    struct EmailContentSection {
        std::string _content;
        EmailContentSectionType _type;
        std::vector<types::MIMEHeader> _headers;
        types::s32 _index;
    };

    class Email {
    public:
        ~Email() {
            delete this->_subject;
            delete this->_boundary;
            delete this->_message_id;
        }

    public:
        types::MIMEContentType _content_type;
        types::EmailAddress _transport_to;
        types::EmailAddress _transprot_from;
        const types::s8 * _subject = nullptr;
        const types::s8 * _message_id = nullptr;
        const types::s8 * _boundary = nullptr;
        types::s64 _date;
        types::s64 _time_stamp;
        types::s64 _receive_time_stamp;
        types::s64 _bucket;

        // user data
        types::s64 _uuid;
        types::s64 _usr_uuid;

        std::vector<types::EmailAddress> _froms;
        std::vector<types::EmailAddress> _tos;
        std::vector<types::MIMEHeader> _headers;
        std::vector<types::MIMEBody> _content;

    };

}

#endif // __EMAIL_HPP__