#include "smtp_response.h"
#include "ztools.h"
#include "setting.h"

namespace smtp {
    SMTPResponse::SMTPResponse(const SMTPResponseCode &__src, const SMTPResponseType &__srt) : response_(tool::int_to_str(__srt)) {
        switch (__srt) {
            case SMTPResponseType::SRT_CONN_SUCCESS: {
                response_ << " " << local_domain << " SHINCEY MAKE IT." << "\r\n";
                break;
            }
            case SMTPResponseType::SRT_HELO_SUCCESS: {
                response_ << "-" << local_domain;
                break;
            }
            case SMTPResponseType::SRT_MAIL_FROM_SUCCESS: {
                break;
            }
            case SMTPResponseType::SRT_RCPT_TO_SUCCESS: {
                break;
            }
            case SMTPResponseType::SRT_DATA_SUCCESS: {
                break;
            }
            case SMTPResponseType::SRT_QUIT_SUCCESS: {
                break;
            }
        }
    }
}