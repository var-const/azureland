#pragma once

#include <vector>

template <typename T>
void ResizeAtLeast(std::vector<T>& Container, const int NewSize)
{
    typedef std::vector<T> VecT;
    if (Container.size() < static_cast<typename VecT::size_type>(NewSize))
    {
        Container.resize(NewSize);
    }
}
