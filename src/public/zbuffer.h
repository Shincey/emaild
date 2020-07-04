#ifndef __ZBUFFER_H__
#define __ZBUFFER_H__

#include "zsystem.h"
#include "ztools.h"

namespace lib {
    class zbuffer {
    public:
        explicit zbuffer(const u32 __size) : size_(__size), in_(0), out_(0) {
            //zassert(__size >= 0, "parameter passed shoud be bigger than zero"); // not nesscary
            data_ = new char[__size];
        }

        ~zbuffer() {
            delete[] data_;
        }

        inline bool in(const void *__data, const u32 __size) {
            zassert(in_ >= out_ && in_ <= size_, "something wrong here");

            if (size_ - length() < __size) { return false; }

            if (size_ - in_ < __size) {
                if (in_ == out_) {
                    in_ = 0;
                    out_ = 0;
                } else {
                    tool::mem::s_memcpy(data_, size_, data_ + out_, in_ - out_);
                    in_ -= out_;
                    out_ = 0;
                }
            }

            zassert(in_ < size_ && out_ < size_, "something wrong here");
            tool::mem::s_memcpy(data_ + in_, size_ - length(), __data, __size);
            in_ += __size;
            zassert(in_ <= size_, "something wrong here");

            return true;
        }

        inline bool out(const u32 __size) {
            zassert(out_ + __size <= in_ && in_ <= size_, "something wrong here");

            if (out_ + __size > in_) { return false; }
            
            out_ += __size;

            return true;
        }

        inline const void *data() const {
            return data_ + out_;
        }

        inline const s32 length() const {
            zassert(in_ >= out_ && in_ <= size_, "something wrong here");
            return in_ - out_;
        }

    private:
        const u32 size_;
        u32 in_;
        u32 out_;
        char *data_;
    };
}

#endif // __ZBUFFER_H__