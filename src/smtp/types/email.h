#ifndef __EMAIL_H__
#define __EMAIL_H__

#include "zsystem.h"
#include <string>
#include <vector>
#include <tuple>


namespace smtp {
    class EmailAddress {
    public:
        EmailAddress(const std::string &__address, const std::string &__name);
        EmailAddress(const std::string &__raw);

        const std::string get_address() const;
        const std::string get_name() const;

        void set_address(const std::string __address);
        void set_name(const std::string __name);

        std::tuple<const std::string, const std::string> get_domain_name(const std::string &__address);

    private:
        std::string address_;
        std::string name_;
    };


    class EmailHeader {
    private:
        std::string key_;
        std::string value_;
    };

    class FullEmail {
    public:
        FullEmail();

    private:
        u8 email_type_;
        std::string transport_to_;
        std::string subject_;
        std::vector<EmailAddress> addresses_to_;
        std::vector<EmailAddress> addresses_from_;
        std::vector<EmailHeader> headers_;
    };
}


#endif // __EMAIL_H__