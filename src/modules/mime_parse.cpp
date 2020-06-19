
#include "mime_parse.hpp"
#include "timer.hpp"

namespace modules {

    char parseMessage(const char* raw, modules::Email& target) {
        char rc = 0;
        Timer t("MIME Parser");

        char *headers = nullptr;
        char *body = nullptr;

        // separates
    } // end of method 'parseMessage'

}