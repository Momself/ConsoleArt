// Tencent is pleased to support the open source community by making RapidJSON available.
// 
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed 
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
// CONDITIONS OF ANY KIND, either express or implied. See the License for the 
// specific language governing permissions and limitations under the License.

#ifndef RAPIDJSON_ALLOCATORS_H_
#define RAPIDJSON_ALLOCATORS_H_

#include "rapidjson.h"

RAPIDJSON_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////////////
// Allocator

/*! \class rapidjson::Allocator
    \brief Concept for allocating, resizing and freeing memory block.
    
    Note that Malloc() and Realloc() are non-static but Free() is static.
    
    So if an allocator need to support Free(), it needs to put its pointer in 
    the header of memory block.

\code
concept Allocator {
    static const bool kNeedFree;    //!< Whether this allocator needs to call Free().

    // Allocate a memory block.
    // \param size of the memory block in bytes.
    // \returns pointer to the memory block.
    void* Malloc(size_t size);

    // Resize a memory block.
    // \param originalPtr The pointer to current memory block. Null pointer is permitted.
    // \param originalSize The current size in bytes. (Design issue: since some allocator may not book-keep this, explicitly pass to it can save memory.)
    // \param newSize the new size in bytes.
    void* Realloc(void* originalPtr, size_t originalSize, size_t newSize);

    // Free a memory block.
    // \param pointer to the memory block. Null pointer is permitted.
    static void Free(void *ptr);
};
\endcode
*/


/*! \def RAPIDJSON_ALLOCATOR_DEFUALT_CHUNK_CAPACITY
    \ingroup RAPIDJSON_CONFIG
    \brief User-defined kDefaultChunkCapacity definition.

    User can define this as any \c size that is a power of 2.
*/

#ifndef RAPIDJSON_ALLOCATOR_DEFAULT_CHUNK_CAPACITY
#define RAPIDJSON_ALLOCATOR_DEFAULT_CHUNK_CAPACITY (64 * 1024)
#endif


///////////////////////////////////////////////////////////////////////////////
// CrtAllocator

//! C-runtime library allocator.
/*! This class is just wrapper for standard C library memory routines.
    \note implements Allocator concept
*/
class CrtAllocator {
public:
    static const bool kNeedFree = true;
    void* Malloc(size_t size) { 
        if (size) //  behavior of malloc(0) is implementation defined.
            return std::malloc(size);
        else
            return NULL; // standardize to returning NULL.
    }
    void* Realloc(void* originalPtr, size_t originalSize, size_t newSize) {
        (void)originalSize;
        if (newSize == 0) {
            std::free(originalPtr);
            return NULL;
        }
        return std::realloc(originalPtr, newSize);
    }
    static void Free(void *ptr) { std::free(ptr); }
};

///////////////////////////////////////////////////////////////////////////////
// MemoryPoolAllocator

//! Default memory allocator used by the parser and DOM.
/*! This allocator allocate memory blocks from pre-allocated memory chunks. 

    It does not free memory blocks. And Realloc() only allocate new memory.

    The memory chunks are allocated by BaseAllocator, which is CrtAllocator by default.

    User may also supply a buffer as the first chunk.

    If the user-buffer is full then additional chunks are allocated by BaseAllocator.

    The user-buffer is not deallocated by this allocator.

    \tparam BaseAllocator the allocator type for allocating memory chunks. Default is CrtAllocator.
    \note implements Allocator concept
*/
template <typename BaseAllocator = CrtAllocator>
class MemoryPoolAllocator {
public:
    static const bool kNeedFree = false;    //!< Tell users that no need to call Free() with this allocator. (concept Allocator)

    //! Constructor with chunkSize.
    /*! \param chunkSize The size of memory chunk. The default is kDefaultChunkSize.
       