#include "email.h"
#include <stdexcept>
#include "zinterface.h"
#include "core.h"

namespace smtp {
    
    EmailAddress::EmailAddress(const std::string &__address, const std::string &__name) : address_(__address), name_(__name) {}
    
    EmailAddress::EmailAddress(const std::string &__raw) {
        std::size_t at_pos = __raw.find_first_of('@');
        zassert(at_pos != std::string::npos, "wtf");
        if (at_pos == std::string::npos) {
            error(core::Core::instance(), "invalid email address");
            return;
        }

        std::size_t open_bracket_pos = __raw.find_first_of('<');
        zassert(open_bracket_pos != std::string::npos, "wtf");
        std::size_t close_bracket_pos = __raw.find_first_of('>');
        zassert(close_bracket_pos != std::string::npos, "wtf");
        if (open_bracket_pos != std::string::npos && close_bracket_pos != std::string::npos) {
            address_ = __raw.substr(open_bracket_pos + 1, close_bracket_pos - open_bracket_pos - 1);
            name_ = __raw.substr(0, open_bracket_pos);
        } else {
            error(core::Core::instance(), "name<address> shoud be expected");
            return;
        }
    }

    const std::string EmailAddress::get_address() const { return address_; }
    const std::string EmailAddress::get_name() const { return name_; }

    void EmailAddress::set_address(const std::string __address) { address_ = __address; }
    void EmailAddress::set_name(const std::string __name) { name_ = __name; }


    std::tuple<const std::string, const std::string> EmailAddress::get_domain_name(const std::string &__address) {
        std::size_t at_pos = __address.find_first_of('@');
        if (at_pos == std::string::npos) {
            error(core::Core::instance(), "invalid email address");
            return std::tuple<const std::string, const std::string>();
        }
        const std::string domain = __address.substr(at_pos + 1);
        const std::string name = __address.substr(0, at_pos);
        return make_tuple(domain, name);
    }
}