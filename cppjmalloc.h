#ifndef _CPPJMALLOC_H_
#define _CPPJMALLOC_H_

#include "cppmoveptralloc/moveptralloc.h"

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>

// LLVM 2.6 is first with clang, it supports __has_builtin
// GCC 3.4 is the first GCC with __has_clz
#undef JMTILLI_JMALLOC_HAS_CLZ

#ifdef __has_builtin
  #if __has_builtin (__builtin_clz)
    #define JMTILLI_JMALLOC_HAS_CLZ
  #endif
#else
  #ifdef __GNUC__
    #if __GNUC__ == 3 && __GNUC_MINOR__ >= 4
      #define JMTILLI_JMALLOC_HAS_CLZ
    #else
      #if __GNUC__ > 3
        #define JMTILLI_JMALLOC_HAS_CLZ
      #endif
    #endif
  #endif
#endif

namespace jmtilli { namespace jmalloc {

struct jmalloc_block {
  struct jmalloc_block *next;
};

#ifndef JMTILLI_JMALLOC_HAS_CLZ
static inline const std::uint8_t *get_lookup(void) 
{
  static const std::uint8_t lookup[129] = {
    0,0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
  };
  return lookup;
}

static inline const std::uint8_t *get_lookup2(void) 
{
  static const std::uint8_t lookup2[256] = {
    0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
  };
  return lookup2;
}
#endif

// Most of the time, this function is expected to be optimized away
static_assert(sizeof(struct jmalloc_block) <= 131072 && alignof(std::max_align_t) <= 131072, "too strict alignment constraints");
static inline std::int8_t normalize_lookupval(std::int8_t lookupval)
{
  if (lookupval == 0 && (sizeof(struct jmalloc_block) > 16 || alignof(std::max_align_t) > 16))
  {
    lookupval = 1;
  }
  if (lookupval == 1 && (sizeof(struct jmalloc_block) > 32 || alignof(std::max_align_t) > 32))
  {
    lookupval = 2;
  }
  if (lookupval == 2 && (sizeof(struct jmalloc_block) > 64 || alignof(std::max_align_t) > 64))
  {
    lookupval = 3;
  }
  if (lookupval == 3 && (sizeof(struct jmalloc_block) > 128 || alignof(std::max_align_t) > 128))
  {
    lookupval = 4;
  }
  if (lookupval == 4 && (sizeof(struct jmalloc_block) > 256 || alignof(std::max_align_t) > 256))
  {
    lookupval = 5;
  }
  if (lookupval == 5 && (sizeof(struct jmalloc_block) > 512 || alignof(std::max_align_t) > 512))
  {
    lookupval = 6;
  }
  if (lookupval == 6 && (sizeof(struct jmalloc_block) > 1024 || alignof(std::max_align_t) > 1024))
  {
    lookupval = 7;
  }
  if (lookupval == 7 && (sizeof(struct jmalloc_block) > 2048 || alignof(std::max_align_t) > 2048))
  {
    lookupval = 8;
  }
  if (lookupval == 8 && (sizeof(struct jmalloc_block) > 4096 || alignof(std::max_align_t) > 4096))
  {
    lookupval = 9;
  }
  if (lookupval == 9 && (sizeof(struct jmalloc_block) > 8192 || alignof(std::max_align_t) > 8192))
  {
    lookupval = 10;
  }
  if (lookupval == 10 && (sizeof(struct jmalloc_block) > 16384 || alignof(std::max_align_t) > 16384))
  {
    lookupval = 11;
  }
  if (lookupval == 11 && (sizeof(struct jmalloc_block) > 32768 || alignof(std::max_align_t) > 32768))
  {
    lookupval = 12;
  }
  if (lookupval == 12 && (sizeof(struct jmalloc_block) > 65536 || alignof(std::max_align_t) > 65536))
  {
    lookupval = 13;
  }
  return lookupval;
}

struct JMallocImpl {
  private:
    JMallocImpl(const JMallocImpl&) = delete;
    JMallocImpl &operator=(const JMallocImpl&) = delete;

    std::shared_ptr<jmtilli::mpa::MovePtrAllocatorImpl> mptr;

    // 16, 32, 64, 128, 256, 512, 1K, 2K, 4K, 8K, 16K, 32K, 64K, 128K
    //struct jmalloc_block *blocks[14];
    std::vector<struct jmalloc_block*> blocks;

  public:
    JMallocImpl(std::size_t sz=512*1024, std::size_t threshold=128*1024, std::size_t list_count=14):
      mptr(std::make_shared<jmtilli::mpa::MovePtrAllocatorImpl>(sz, threshold))
    {
      std::size_t i;
      std::size_t largest_block_size = 16U<<(list_count-1);
      if (list_count < 1 || sizeof(struct jmalloc_block) > largest_block_size || alignof(std::max_align_t) > largest_block_size)
      {
        throw std::invalid_argument("too few lists");
      }
      if (list_count > 17)
      {
        throw std::invalid_argument("max supported list_count is 17");
      }
      for (i = 0; i < list_count; i++)
      {
        blocks.push_back(nullptr);
      }
    }
    JMallocImpl(std::shared_ptr<jmtilli::mpa::MovePtrAllocatorImpl> &mptr, std::size_t list_count=14): mptr(mptr)
    {
      std::size_t i;
      std::size_t largest_block_size = 16U<<(list_count-1);
      if (list_count < 1 || sizeof(struct jmalloc_block) > largest_block_size || alignof(std::max_align_t) > largest_block_size)
      {
        throw std::invalid_argument("too few lists");
      }
      if (list_count > 17)
      {
        throw std::invalid_argument("max supported list_count is 17");
      }
      for (i = 0; i < list_count; i++)
      {
        blocks.push_back(nullptr);
      }
    }

    void *alloc(std::size_t sz)
    {
      struct jmalloc_block **ls = NULL;
      struct jmalloc_block *blk;
      std::int8_t lookupval;
      if (sz > (16U<<(blocks.size()-1)))
      {
        return new char[sz];
      }
#ifdef JMTILLI_JMALLOC_HAS_CLZ
      lookupval = 28-__builtin_clz(sz ? (sz-1) : 0);
      if (lookupval < 0)
      {
        lookupval = 0;
      }
#else
      if (sz > 2048)
      {
        lookupval = get_lookup2()[(sz-1)/4096]+8;
      }
      else
      {
        lookupval = get_lookup()[(sz+15)/16];
      }
#endif
      lookupval = normalize_lookupval(lookupval);
      ls = &blocks[lookupval];
      sz = 1<<(4+lookupval);
      if (!*ls)
      {
        return mptr->alloc(sz, alignof(std::max_align_t));
      }
      blk = *ls;
      *ls = blk->next;
      return blk;
    }

    void dealloc(void *ptr, std::size_t sz)
    {
      struct jmalloc_block **ls = NULL;
      struct jmalloc_block *blk = reinterpret_cast<struct jmalloc_block*>(ptr);
      std::int8_t lookupval;
      if (sz > (16U<<(blocks.size()-1)))
      {
        delete[] static_cast<char*>(ptr);
        return;
      }
#ifdef JMTILLI_JMALLOC_HAS_CLZ
      lookupval = 28-__builtin_clz(sz ? (sz-1) : 0);
      if (lookupval < 0)
      {
        lookupval = 0;
      }
#else
      if (sz > 2048)
      {
        lookupval = get_lookup2()[(sz-1)/4096]+8;
      }
      else
      {
        lookupval = get_lookup()[(sz+15)/16];
      }
#endif
      lookupval = normalize_lookupval(lookupval);
      ls = &blocks[lookupval];
      sz = 1<<(4+lookupval);
      blk->next = *ls;
      *ls = blk;
    }
};

template <typename T>
struct JMallocTypeAllocator {
  private:
    std::shared_ptr<JMallocImpl> impl;
  public:
    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    JMallocTypeAllocator(std::shared_ptr<JMallocImpl> impl) noexcept: impl(impl) {}

    template <typename U>
    constexpr JMallocTypeAllocator(const JMallocTypeAllocator<U> &other) noexcept: impl(other.impl) {}

    T* allocate(std::size_t n) {
        if (n > SIZE_MAX/sizeof(T))
        {
            throw std::bad_alloc();
        }
        void *ptr = impl->alloc(n*sizeof(T));
        return static_cast<T*>(ptr);
    }
    void deallocate(T* p, std::size_t n) noexcept {
        if (n > SIZE_MAX/sizeof(T))
        {
            std::terminate();
        }
        return impl->dealloc(p, n*sizeof(T));
    }

    template <typename U>
    friend bool operator==(const JMallocTypeAllocator<T> &a, const JMallocTypeAllocator<U> &b) noexcept {
        return a.impl.get() == b.impl.get();
    }
    template <typename U>
    friend bool operator!=(const JMallocTypeAllocator<T> &a, const JMallocTypeAllocator<U> &b) noexcept {
        return !(a == b);
    }
};

template <typename T>
struct JMallocTypeAllocatorDangerous {
  private:
    std::shared_ptr<JMallocImpl> &impl;
  public:
    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    JMallocTypeAllocatorDangerous(std::shared_ptr<JMallocImpl> &impl) noexcept: impl(impl) {}

    template <typename U>
    constexpr JMallocTypeAllocatorDangerous(const JMallocTypeAllocatorDangerous<U> &other) noexcept: impl(other.impl) {}

    T* allocate(std::size_t n) {
        if (n > SIZE_MAX/sizeof(T))
        {
            throw std::bad_alloc();
        }
        void *ptr = impl->alloc(n*sizeof(T));
        return static_cast<T*>(ptr);
    }
    void deallocate(T* p, std::size_t n) noexcept {
        if (n > SIZE_MAX/sizeof(T))
        {
            std::terminate();
        }
        return impl->dealloc(p, n*sizeof(T));
    }

    template <typename U>
    friend bool operator==(const JMallocTypeAllocatorDangerous<T> &a, const JMallocTypeAllocatorDangerous<U> &b) noexcept {
        return a.impl.get() == b.impl.get();
    }
    template <typename U>
    friend bool operator!=(const JMallocTypeAllocatorDangerous<T> &a, const JMallocTypeAllocatorDangerous<U> &b) noexcept {
        return !(a == b);
    }
};

static inline std::shared_ptr<JMallocImpl> make_impl(std::size_t arenasz=512*1024, std::size_t thresh=128*1024, std::size_t list_count=14)
{
    return std::make_shared<JMallocImpl>(arenasz, thresh);
}

template <typename T>
using TypeAlloc = JMallocTypeAllocator<T>;

template <typename T>
using TypeAllocDanger = JMallocTypeAllocatorDangerous<T>;

template<typename T, typename... Args>
T *alloc_construct_one(TypeAlloc<T> &alloc, Args&&... args)
{
  using Traits = std::allocator_traits<TypeAlloc<T> >;
  T *res;
  res = Traits::allocate(alloc, 1);
  Traits::construct(alloc, res, std::forward<Args>(args)...);
  return res;
}

template<typename T, typename... Args>
T *alloc_construct_one(std::shared_ptr<JMallocImpl> &impl, Args&&... args)
{
  using Traits = std::allocator_traits<TypeAllocDanger<T> >;
  TypeAllocDanger<T> alloc(impl);
  T *res;
  res = Traits::allocate(alloc, 1);
  Traits::construct(alloc, res, std::forward<Args>(args)...);
  return res;
}

template<typename T, typename... Args>
void destroy_dealloc_one(TypeAlloc<T> &alloc, T *t)
{
  using Traits = std::allocator_traits<TypeAlloc<T> >;
  Traits::destroy(alloc, t);
  Traits::deallocate(alloc, t, 1);
}

template<typename T, typename... Args>
void destroy_dealloc_one(std::shared_ptr<JMallocImpl> &impl, T *t)
{
  using Traits = std::allocator_traits<TypeAllocDanger<T> >;
  TypeAllocDanger<T> alloc(impl);
  Traits::destroy(alloc, t);
  Traits::deallocate(alloc, t, 1);
}

template<typename T, typename... Args>
T *alloc_construct_many(TypeAlloc<T> &alloc, std::size_t n, Args&&... args)
{
  using Traits = std::allocator_traits<TypeAlloc<T> >;
  T *res;
  std::size_t i;
  res = Traits::allocate(alloc, n);
  try
  {
    for (i = 0; i < n; i++)
    {
      Traits::construct(alloc, &res[i], std::forward<Args>(args)...);
    }
    return res;
  }
  catch (...)
  {
    for (; i > 0; i--)
    {
      Traits::destroy(alloc, &res[i-1]);
    }
    Traits::deallocate(alloc, res, n);
    throw;
  }
}

template<typename T, typename... Args>
T *alloc_construct_many(std::shared_ptr<JMallocImpl> &impl, std::size_t n, Args&&... args)
{
  using Traits = std::allocator_traits<TypeAllocDanger<T> >;
  TypeAllocDanger<T> alloc(impl);
  T *res;
  std::size_t i;
  res = Traits::allocate(alloc, n);
  try
  {
    for (i = 0; i < n; i++)
    {
      Traits::construct(alloc, &res[i], std::forward<Args>(args)...);
    }
    return res;
  }
  catch (...)
  {
    for (; i > 0; i--)
    {
      Traits::destroy(alloc, &res[i-1]);
    }
    Traits::deallocate(alloc, res, n);
    throw;
  }
}

template<typename T, typename... Args>
void destroy_dealloc_many(TypeAlloc<T> &alloc, T *t, std::size_t n)
{
  std::size_t i;
  using Traits = std::allocator_traits<TypeAlloc<T> >;
  for (i = n; i > 0; i--)
  {
    Traits::destroy(alloc, &t[i-1]);
  }
  Traits::deallocate(alloc, t, n);
}

template<typename T, typename... Args>
void destroy_dealloc_many(std::shared_ptr<JMallocImpl> &impl, T *t, std::size_t n)
{
  std::size_t i;
  TypeAllocDanger<T> alloc(impl);
  using Traits = std::allocator_traits<TypeAllocDanger<T> >;
  for (i = n; i > 0; i--)
  {
    Traits::destroy(alloc, &t[i-1]);
  }
  Traits::deallocate(alloc, t, n);
}

} }

#endif
