#ifndef _BRYNET_NET_ANY_H
#define _BRYNET_NET_ANY_H

#include <brynet/utils/CPP_VERSION.h>

#ifdef HAVE_LANG_CXX17
#include <any>
#else
#include <cstdint>
#endif

namespace brynet
{
    namespace net
    {
#ifdef HAVE_LANG_CXX17
        typedef std::any BrynetAny;

        template<typename T>
        auto cast(const BrynetAny& ud)
        {
            return std::any_cast<T>(&ud);
        }
#else
        typedef int64_t BrynetAny;
        template<typename T>
        auto cast(const BrynetAny& ud)
        {
            return static_cast<const T*>(&ud);
        }
#endif
    }
}

#endif
