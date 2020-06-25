
#include "mime_parse.hpp"

#include <stdlib.h>
#include <string.h>

namespace modules::mime {

    void clean_white_space(const types::s8 *a, std::size_t len, types::s8 **ret) {
        if (*ret != nullptr) free(*ret);
        *ret = reinterpret_cast<types::s8 *>(malloc(len + 1));
        len = 0;

        bool conspace =false; // check the previous is whitespace or not
        while (*a != '\0') {
            if (*a == ' ') {
                if (conspace) {
                    a++;
                    continue;
                }
                conspace =true;

            } else {
                conspace = false;
            }

            (*ret)[len] = *a;
            len++;
            a++;
        }

        (*ret)[len] = '\0';
    }


    types::s8 separate_header_and_body(const types::s8 *raw, types::s8 **header, types::s8 **body) {
        types::s8 *token = nullptr;
        types::s8 *raw_copy = nullptr;
        bool headers_ended;

        std::size_t raw_len;
        std::size_t i, j;
        std::size_t head_size;
        std::size_t body_size;

        bool indention;

        head_size = 1;
        *header = reinterpret_cast<types::s8 *>(malloc(head_size));
        (*header)[0] = '\0';
        
        body_size = 1;
        *body = reinterpret_cast<types::s8 *>(malloc(body_size));
        (*body)[0] = '\0';

        // creates an copy of the raw string
        raw_len = strlen(reinterpret_cast<const char *>(raw));
        raw_copy = reinterpret_cast<types::s8 *>(malloc(raw_len + 1));
        memcpy(raw_copy, raw, raw_len + 1);

        token = reinterpret_cast<types::s8 *>(strtok(reinterpret_cast<char *>(raw_copy), "\r"));
        headers_ended = false;

        while (token != nullptr) {
            // checks if there is a '\n' in the start witch needs to be removed.
            if (token[0] == '\n') {
                memmove(&token[0], &token[1], strlen(reinterpret_cast<const char *>(&token[0])));
            }
            // checks if the string is empty.
            if (token[0] == '\0') {
                headers_ended = true;
            }
            // check if it contains an indention, if so we merge it with the previous line.
            if (token[0] == ' ' || token[0] == '\t') {
                j = 0; // detects where the chars start
                for (types::s8 *p = &token[0]; *p != '\0' && (*p == '\t' || *p == ' '); p++) j++;
                memmove(&token[0], &token[j], strlen(reinterpret_cast<const char *>(&token[0]) - j + 1));
                indention = true;
            }
            
            // append the string to the according result, headers or body

            if (!headers_ended) { // append to headers
                head_size += strlen(reinterpret_cast<const char *>(&token[0]) + 2);
                *header = reinterpret_cast<types::s8 *>(realloc(*header, head_size));
                if (!indention) strcat(reinterpret_cast<char *>(&(*header)[0]), "\r\n");
                strcat(reinterpret_cast<char *>(&(*header)[0]), reinterpret_cast<char *>(&token[0]));
            } else {
                body_size += strlen(reinterpret_cast<char *>(&token[0])) + 2;
                *body = reinterpret_cast<types::s8 *>(realloc(*body, body_size));
                if (!indention) strcat(reinterpret_cast<char *>(&(*body)[0]), "\r\n");
                strcat(reinterpret_cast<char *>(&(*body)[0]), reinterpret_cast<char *>(&token[0]));
            }
            // goes to the next token
            token = reinterpret_cast<types::s8 *>(strtok(nullptr, "\r"));
            i++;
            indention = false;
        }

        // checks if there is an <CR><LF> at the begin of the string

        if ((*body)[0] == '\r' && (*body)[1] == '\n') {
            memmove(&(*body)[0], &(*body)[2], strlen(reinterpret_cast<char *>(&(*body)[0]))-1);
        }
        if ((*header)[0] == '\r' && (*header)[1] == '\n') {
            memmove(&(*header)[0], &(*header)[2], strlen(reinterpret_cast<char*>(&(*header)[0]))-1);
        }

        free(raw_copy);
        return 0;
    }


    types::s8 parse_headers(const char *raw, std::vector<types::MIMEHeader> &headers) {
        
    }

    /**
     * Splits an header base on ':'
     * @param raw
     * @param key
     * @param val
     * @return
     */
    types::s8 split_header(const types::s8 *raw, types::s8 **key, types::s8 **val);

    /**
     * Splits an email address up in the domain and username
     * @param raw
     * @param username
     * @return
     */
    types::s32 split_address(const types::s8 *raw, types::s8 **username, types::s8 **domain);

    /**
     * Gets the content type based value of header
     * @param raw
     * @return
     */
    types::MIMEContentType get_content_type(const types::s8 *raw);

    /**
     * Parses the name and address from an MIME address
     * @param raw
     * @param name
     * @param address
     * @return
     */
    types::s8 parse_address(const types::s8 *raw, types::s8 **name, types::s8 **address);

    /**
     * Parses an header value which may contain both keyed parameters, and non-keyed paramters
     * @param raw
     * @param nkey_params
     * @param key_params
     * @return
     */
    types::s8 parse_header_parameters(const types::s8 *raw, std::vector<const types::s8 *> &nkey_params, std::map<const types::s8 *, const types::s8 *> &key_params);

    /**
     * Parses an list of email address
     * @param raw
     * @param ret
     * @return
     */
    types::s8 parse_address_list(const types::s8 *raw, std::vector<types::EmailAddress> &ret);

    /**
     * Parses an multipart/alternative body
     * @param raw
     * @param boundary
     * @param target
     * @return
     */
    types::s8 parse_multipart_alternative_body(const types::s8 *raw, const types::s8 *boundary, std::vector<types::MIMEBody> &target);

    /**
     * Gets the transfer encoding from string
     * @param raw
     * @return
     */
    types::MIMEContentTransferEncoding get_transfer_encoding(const types::s8 *raw);
}