#pragma once
#include <cstdint>

namespace sp
{
    enum class InitFlags
    {
        invalid = 0x0000,
        gui = 0x0001,
        everything = 0xffff
    };

    inline InitFlags operator|(InitFlags lhs, InitFlags rhs)
    {
        return static_cast<InitFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    inline InitFlags operator&(InitFlags lhs, InitFlags rhs)
    {
        return static_cast<InitFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }

    class SoundProcessor
    {
    public:
        static void init(InitFlags flags)
        {
            if((flags & InitFlags::gui) != InitFlags::invalid){
                // init gui
            }
        }
    private:

    };
}
