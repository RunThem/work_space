#pragma once

#include "u.h"

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_MAP_BUCKETS_NUM
#  define U_MAP_BUCKETS_NUM 64
#endif

#ifndef U_MAP_RESIZE_RADIO
#  define U_MAP_RESIZE_RADIO 1.5
#endif

enum u_map_hash_fn {
  MAP_FNV_64_HASH_FN = 1,
  MAP_INT_HASH_FN,

  MAP_HASH_FN_MAX,
};

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef uint64_t hash_t;

typedef fnt(map_hash_fn, hash_t, const uint8_t*, size_t);
typedef fnt(map_eq_fn, bool, const void*, const void*);

#define map(K, V)                                                                                  \
  struct {                                                                                         \
    V* val;                                                                                        \
    K key;                                                                                         \
  }*

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __map_new(size_t ksize, size_t vsize, map_eq_fn eq_fn, enum u_map_hash_fn fn);
#define map_new(K, V, eq_fn, fn) __map_new(sizeof(K), sizeof(V), eq_fn, fn)

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __map_clear(any_t _self);
#define map_clear(map) __map_clear(map)

ret_t __map_cleanup(any_t _self);
#define map_cleanup(map)                                                                           \
  do {                                                                                             \
    __map_cleanup(map);                                                                            \
    map = nullptr;                                                                                 \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __map_ksize(any_t _self);
#define map_ksize(map) __map_ksize(map)

size_t __map_vsize(any_t _self);
#define map_vsize(map) __map_vsize(map)

size_t __map_len(any_t _self);
#define map_len(map) __map_len(map)

bool __map_empty(any_t _self);
#define map_empty(map) __map_empty(map)

any_t __map_at(any_t _self);
#define map_at(map, _key) (*(as(((map)->key = (_key), __map_at(map)), typeof((map)->val))))

void __map_pop(any_t _self);
#define map_pop(map, _key) ((map)->key = (_key), __map_pop(map))

#define __map_val(map) as(any(map) + sizeof((map)->key) + sizeof((map)->val), typeof((map)->val))

ret_t __map_push(any_t _self);
#define map_push(map, _key, _val) ((map)->key = (_key), *__map_val(map) = (_val), __map_push(map))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __map_range(any_t _self);
#define map_for(map) for ((map)->val = nullptr; __map_range(map);)

#ifndef NDEBUG
extern void __map_debug(any_t _self);
#  define map_debug(map) __map_debug(map)
#endif