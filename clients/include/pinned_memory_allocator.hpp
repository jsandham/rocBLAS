//
// Copyright 2018-2019 Advanced Micro Devices, Inc.
//
#pragma once
#include <hip/hip_runtime.h>

//!
//! @brief  Allocator which uses pinned host memory.
//!
template <class T>
struct pinned_memory_allocator
{
    using value_type = T;

    pinned_memory_allocator() = default;

    template <class U>
    pinned_memory_allocator(const pinned_memory_allocator<U>&)
    {
    }

    T* allocate(std::size_t n)
    {
        T*         ptr;
        hipError_t status = hipHostMalloc(&ptr, sizeof(T) * n, hipHostMallocDefault);
        if(status != hipSuccess)
        {
            ptr = nullptr;
            std::cerr << "rocBLAS pinned_memory_allocator failed to allocate memory: "
                      << hipGetErrorString(status) << std::endl;
        }
        return ptr;
    }

    void deallocate(T* ptr, std::size_t n)
    {
        hipError_t status = hipHostFree(ptr);
        if(status != hipSuccess)
        {
            std::cerr << "rocBLAS pinned_memory_allocator failed to free memory: "
                      << hipGetErrorString(status) << std::endl;
        }
    }
};

template <class T, class U>
constexpr bool operator==(const pinned_memory_allocator<T>&, const pinned_memory_allocator<U>&)
{
    return true;
}

template <class T, class U>
constexpr bool operator!=(const pinned_memory_allocator<T>&, const pinned_memory_allocator<U>&)
{
    return false;
}
