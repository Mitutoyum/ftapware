#pragma once

#include <streambuf>
#include <functional>
#include <string>
#include <utility>

namespace FTAPWare {
    class LogStreamBuf : public std::streambuf {
    public:
        using Callback = std::function<void(const std::string&)>;
    protected:
        std::streambuf::int_type overflow(std::streambuf::int_type ch) override;
        int sync() override;
        std::streamsize xsputn(const char* str, std::streamsize count) override;
    private:
        void Flush();

        std::string buffer;
        Callback callback;
    public:
        LogStreamBuf(Callback callback);
    };
}