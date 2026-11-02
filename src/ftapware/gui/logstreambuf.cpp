#include "logstreambuf.h"

#include <streambuf>
#include <functional>
#include <string>
#include <utility>
#include <Windows.h>

namespace FTAPWare {
    LogStreamBuf::LogStreamBuf(Callback callback) : callback(std::move(callback)) {}

    std::streambuf::int_type LogStreamBuf::overflow(std::streambuf::int_type ch) {
        if (traits_type::eq_int_type(ch, traits_type::eof()))
            return traits_type::not_eof(ch);

        const char character = traits_type::to_char_type(ch);

        if (character == '\n')
            Flush();
        else
            buffer += character;

        return ch;
    }

    int LogStreamBuf::sync() {
        Flush();
        return 0;
    }

    std::streamsize LogStreamBuf::xsputn(const char* str, std::streamsize count) {
        for (std::streamsize i = 0; i < count; ++i) {
            if (str[i] == '\n')
                Flush();
            else
                buffer += str[i];
        }

        return count;
    }

    void LogStreamBuf::Flush() {
        if (!buffer.empty()) {
            callback(buffer);
            buffer.clear();
        }
    }
};