#include "cppjmalloc.h"
#include <vector>

namespace jma = jmtilli::jmalloc;

int main(int argc, char **argv)
{
  // same as std::make_shared<jma::JMallocImpl>(..., ...);
  auto impl = jma::make_impl(512*1024, 128*1024);
  jma::TypeAlloc<double> dblalloc(impl);
  using DblTraits = std::allocator_traits<jma::TypeAlloc<double> >;
  std::vector<int, jma::TypeAlloc<int> > vec(impl);
  size_t i;
  double *d;

  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.clear();
  vec.push_back(4);
  vec.push_back(5);
  vec.push_back(6);
  vec.push_back(7);
  vec.push_back(8);
  vec.push_back(9);
  for (i = 0; i < 1000*1000; i++)
  {
    vec.push_back(i);
  }
  d = DblTraits::allocate(dblalloc, 1);
  DblTraits::construct(dblalloc, d, 1.5);
  DblTraits::destroy(dblalloc, d);
  DblTraits::deallocate(dblalloc, d, 1);

  d = jma::alloc_construct_one<double>(impl, 1.5);
  jma::destroy_dealloc_one<double>(impl, d);

  d = jma::alloc_construct_one(dblalloc, 1.5);
  jma::destroy_dealloc_one(dblalloc, d);

  d = DblTraits::allocate(dblalloc, 3);
  DblTraits::construct(dblalloc, &d[0], 2.5);
  DblTraits::construct(dblalloc, &d[1], 3.5);
  DblTraits::construct(dblalloc, &d[2], 4.5);
  DblTraits::destroy(dblalloc, &d[2]);
  DblTraits::destroy(dblalloc, &d[1]);
  DblTraits::destroy(dblalloc, &d[0]);
  DblTraits::deallocate(dblalloc, d, 3);

  d = jma::alloc_construct_many(dblalloc, 3, 2.75);
  jma::destroy_dealloc_many(dblalloc, d, 3);

  d = jma::alloc_construct_many<double>(impl, 3, 2.75);
  jma::destroy_dealloc_many<double>(impl, d, 3);

  if (dblalloc != dblalloc) // test comparison works
    return 1;

  return 0;
}
