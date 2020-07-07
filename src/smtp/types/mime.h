#ifndef __MIME_H__
#define __MIME_H__

namespace smtp {
    enum class ContentType {
        TEXT_PLAIN = 0,             // 无格式文本
        TEXT_RICHTEXT,              // 简单格式文本，如粗体、斜体、下划线等
        TEXT_ENRICHTEXT,            // richtext的简化和改进

        MULTIPART_MIXED,            // 多个正文部分，串行处理
        MULTIPART_PARALLEL,         // 多个正文部分，可并行处理
        MULTIPART_DIGEST,           // 一个电子邮件的摘要
        MULTIPART_ALTERNATIVE,      // 多个正文部分，具有相同的语义内容

        MESSAGE_RFC822,             // 内容是另一个RFC 822邮件报文
        MESSAGE_PARTIAL,            // 内容是一个邮件报文的片段
        MESSAGE_EXTERNAL_BODY,      // 内容是指向实际报文的指针

        APPLICATION_OCTET_STREAM,   // 任意二进制数据
        APPLICATION_POSTSCRIPT,     // 一个 PostScript 程序

        IMAGE_JPEG,                 // ISO 10918格式
        IMAGE_GIF,                  // Compuerve 的图形交换格式

        AUDIO_BASIC,                // 用 8 bit ISDN u律格式编码
        
        VIDEO_MPEG,                 // ISO 11172格式
    };

    static const char * content_type[20] = {
        "text/plain",
        "text/richtext",
        "text/enriched",

        "multipart/mixed",
        "multipart/parallel",
        "multipart/digest",
        "multipart/alternative",

        "message/rfc822",
        "message/partial",
        "message/external-body",
        
        "application/octet-stream",
        "application/postscript",

        "image/jpeg",
        "image/gif",

        "audio/basic",

        "video/mpeg"
    };

    enum class ContentTransferEncoding {
        SEVEN_BIT = 0,
        QUOTED_PRINTABLE,
        BASE64,
        EIGHT_BIT,
        BINARY,
    };
}

#endif // __MIME_H__