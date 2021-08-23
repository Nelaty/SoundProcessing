/* 
 * Created by Matthias "Nelaty" Gensheimer on 26.06.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include <cstdint>
#include <string.h>
#include <stdlib.h>

namespace sp::util
{
    class RawData
    {
    public:
        template<class T>
        struct FwdIt
        {

        };

        template<class T>
        FwdIt<T> begin(){
            return {};
        }
        template<class T>
        FwdIt<T> end() {
            return {};
        }

        void allocate(void* data, size_t size)
        {
            free();
            mSize = size;
            mData = malloc(mSize * sizeof(unsigned char));
            memcpy(mData, data, mSize);
        }

        void free()
        {
            if(mData) ::free(mData);
        }

        inline void* data() const
        {
            return mData;
        }

        inline size_t size() const
        {
            return mSize;
        }

        inline bool empty() const
        {
            return mSize == 0;
        }

    private:
        size_t mSize;
        void* mData{nullptr};
    };
}