
#include "./commands.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace server::cmd {

    std::tuple<SMTPServerCommand, const char*> parse(char* buf) {
        SMTPServerCommand commandResult = SMTPServerCommand::INVALID;
        //  ---
        // Check with command it is
        char* temp = reinterpret_cast<char*>(malloc(16));
        int commandLen = 0;
        if (buf[0] == 'H' || buf[0] == 'E') {
            memcpy(&temp[0], &buf[0], 4);
            temp[4] = '\0';
            if (strcmp(&temp[0], "EHLO") == 0 || strcmp(&temp[0], "HELO") == 0) {
                commandResult = SMTPServerCommand::HELLO;
                commandLen = 4;
            } else if (strcmp(&temp[0], "HELP") == 0) {
                commandResult = SMTPServerCommand::HELP;
                commandLen = 4;
            }

        } else if (buf[0] == 'A') {
            memcpy(&temp[0], &buf[0], 4);
            temp[4] = '\0';
            if (strcmp(&temp[0], "AUTH") == 0) {
                commandResult = SMTPServerCommand::AUTH;
                commandLen = 4;
            }

        } else if (buf[0] == 'D') {
            memcpy(&temp[0], &buf[0], 4);
            temp[4] = '\0';
            if (strcmp(&temp[0], "DATA") == 0) {
                commandResult = SMTPServerCommand::DATA;
                commandLen = 4;
            }

        } else if (buf[0] == 'M') {
            memcpy(&temp[0], &buf[0], 9);
            temp[9] = '\0';
            if (strcmp(&temp[0], "MAIL FROM") == 0) {
                commandResult = SMTPServerCommand::MAIL_FROM;
                commandLen = 9;
            }

        } else if (buf[0] == 'R') {
            memcpy(&temp[0], &buf[0], 7);
            temp[7] = '\0';
            if (strcmp(&temp[0], "RCPT TO") == 0) {
                commandResult = SMTPServerCommand::RCPT_TO;
                commandLen = 7;
            }

        } else if (buf[0] == 'Q') {
            memcpy(&temp[0], &buf[0], 4);
            temp[4] = '\0';
            if (strcmp(&temp[0], "QUIT") == 0) {
                commandResult = SMTPServerCommand::QUIT;
                commandLen = 4;
            }

        } else if (buf[0] == 'S') {
            memcpy(&temp[0], &buf[0], 8);
            temp[8] = '\0';
            if (strcmp(&temp[0], "STARTTLS") == 0) {
                commandResult = SMTPServerCommand::START_TLS;
                commandLen = 8;
            }
        }

        free(temp);

        // check if there nee to be default argument type parsed
        if (buf[commandLen] == ':' ||
            commandResult == SMTPServerCommand::AUTH ||
            commandResult == SMTPServerCommand::HELLO) {
                // check if there is any content at all

                if (strlen(&buf[commandLen]) == 2) {
                    return std::make_tuple(commandResult, nullptr);
                }
                // get the size to copy
                std::size_t size2copy = strlen(&buf[0]) - commandLen;
                // allocate
                char* args = reinterpret_cast<char *>(malloc(size2copy));
                args[0] = '\0';
                strcat(&args[0], &buf[commandLen+1]);
                // remove the <CR><LF>
                args[size2copy - 3] = '\0';

                // resize the buffer
                size2copy -= 2;
                args = reinterpret_cast<char*>(realloc(&args[0], size2copy));
                // if whitespace at begin, remove it
                if (args[0] == ' ') {
                    memmove(&args[0], &args[1], strlen(&args[0]));
                }

                // return the result
                return std::make_tuple(commandResult, reinterpret_cast<const char*>(args));

            } else {
                return std::make_tuple(commandResult, nullptr);
            }
    } // end of the method 'parse'

    char* gen(int code, const char* param, const char* listParams[], char listParamN) {
        char* result = nullptr;
        std::size_t resultSize;
        // prepares the strings
        // create the buffer with 3 chars available for the code, and
        // the one for the dash or space
        result = reinterpret_cast<char*>(malloc(13));
        result[0] = '\0';
        resultSize = 13;
        // appends the code
        // creates the code buffer
        char buffer[12];
        // turns the code into an string
        sprintf(&buffer[0], "%d", code);
        // adds the code
        strcat(&result[0], &buffer[0]);
        // adds '-' or ' '
        if (param != nullptr && listParams == nullptr) {
            strcat(&result[0], " ");
        } else if (listParams != nullptr) {
            strcat(&result[0], "-");
        }
        // adds params if there
        if (param == nullptr) {
            param = "No message specified";
        }
        // resizing the buffer, plus two for <CR><LF>
        resultSize += strlen(&param[0]) + 2;
        result = reinterpret_cast<char*>(realloc(&result[0], resultSize));
        // concats the params
        strcat(&result[0], &param[0]);
        // adds arguments if required
        // checks if any arguments need to be added
        if (listParamN > 0 && listParams != nullptr) {
            // adds the <CR><LF>
            strcat(&result[0], "\r\n");
            // Loops over the arguments
            for (char i = 0; i < listParamN; i++) {
                // resizing the buffer, for the code, one dash and two for <CR><LF>
                resultSize += strlen(listParams[i]) + 6;
                result = reinterpret_cast<char*>(realloc(&result[0], resultSize));
                // appends the code
                strcat(&result[0], &buffer[0]);
                // check if dash or space needs to be appended
                if (i+1 != listParamN) {
                    strcat(&result[0], "-");
                } else {
                    strcat(&result[0], " ");
                }
                // appends the param
                strcat(&result[0], listParams[i]);
                // appends the <CR><LF>
                strcat(&result[0], "\r\n");
            }

        } else {
            strcat(&result[0], " - smtp\r\n");
        }

        return result;
    }

   
    const char* cmd_to_str(const SMTPServerCommand& command) {
        switch (command) {
            case SMTPServerCommand::HELLO: {
                return "Hello";
            }
            case SMTPServerCommand::DATA: {
                return "Data";
            }
            case SMTPServerCommand::MAIL_FROM: {
                return "Mail from";
            }
            case SMTPServerCommand::RCPT_TO: {
                return "Rcpt to";
            }
            case SMTPServerCommand::START_TLS: {
                return "Start TLS";
            }
            case SMTPServerCommand::QUIT: {
                return "Quit";
            }
            default: {
                return "Unknown";
            }
        };
    }
}