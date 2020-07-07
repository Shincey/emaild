#ifndef __SMTP_RESPONSE_H__
#define __SMTP_RESPONSE_H__

#include "zsystem.h"
#include <string>

namespace smtp {
    typedef enum : u32 {
        SRC_SERVER_INITIAL = 220,
        SRC_QUIT = 221,
        SRC_OPERATOR_SUCCESS = 250,
        SRC_INVALID_ARGS = 501,
        SRC_ERROR_ADDRESS = 553,
        SRC_OPERATOR_FAILURE = 554,
    } SMTPResponseCode;

    typedef enum {
        SRT_CONN_SUCCESS,
        SRT_CONN_FAILURE,
        SRT_HELO_SUCCESS,
        SRT_HELO_FAILURE,
        SRT_MAIL_FROM_SUCCESS,
        SRT_MAIL_FROM_FAILURE,
        SRT_RCPT_TO_SUCCESS,
        SRT_RCPT_TO_FAILURE,
        SRT_DATA_SUCCESS,
        SRT_DATA_FAILURE,
        SRT_QUIT_SUCCESS,
        SRT_RSET_SUCCESS,
        SRT_RSET_FAILURE,
    } SMTPResponseType;

    class SMTPResponse {
    public:
        SMTPResponse(const SMTPResponseCode &__src, const SMTPResponseType &__srt);

        std::string reponse() const { return response_; }

    private:
        std::string response_;
    };
}

#endif // __SMTP_RESPONSE_H__