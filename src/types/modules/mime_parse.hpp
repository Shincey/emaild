#ifndef __MIME_PARSE_H__
#define __MIME_PARSE_H__

#include <map>
#include <vector>

#include "../types/mime.hpp"
#include "../types/types.hpp"

namespace modules::mime {

    /**
     * Remove unwated whitespace
     * @param a
     * @param len
     */
    void clean_white_space(const types::s8 *a, std::size_t len, types::s8 **ret);

    /**
     * Separates the headers from the body in MIME message
     * @param raw
     * @param header
     * @param body
     * @return
     */
    types::s8 separate_header_and_body(const types::s8 *raw, types::s8 **header, types::s8 **body);

    /**
     * Parses the headers into an vector
     * @param raw
     * @param headers
     * @return
     */
    types::s8 parse_headers(const char *raw, std::vector<types::MIMEHeader> &headers);

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

#endif //__MIME_PARSE_H__