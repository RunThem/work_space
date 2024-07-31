#include <u/u.h>
#include <avlhash.h>
#include <avlmini.h>

/*
 * namespace
 *
 * ua: arr
 * uv: vec
 * um: map
 * ut: avl
 * ul: lst
 * us: str
 * ub: buf
 * uf: file
 * ug: log
 * un: net
 * */

/* 全新版本的字符串原始实现 */
typedef char* u_string_t[2]; /* {raw string pointer, string data pointer} */

size_t node_hash(const void* key) {
  return (size_t)key;
}

int node_compare(const void* key1, const void* key2) {
  size_t x = (size_t)key1;
  size_t y = (size_t)key2;
  return (int)(x - y);
}

typedef struct MyNode {
  struct avl_node node;
  int key;
  int val;
} MyNode;

static inline int avl_node_compare(const void* n1, const void* n2) {
  MyNode* x = (MyNode*)n1;
  MyNode* y = (MyNode*)n2;
  return x->key - y->key;
}

int main(int argc, const u_cstr_t argv[]) {

#define N 1000'0000

#if 1
  /* avlmini */
  struct avl_hash_map map = {};
  struct avl_root tree    = {};
  struct avl_node** nodes = nullptr;
  MyNode* node            = nullptr;
  struct avl_node* dup    = nullptr;

  avl_map_init(&map, node_hash, node_compare);

  nodes = (struct avl_node**)malloc(sizeof(void*) * N);
  u_each (i, N) {
    node      = u_talloc(MyNode);
    node->key = i;
    node->val = i;
    nodes[i]  = (struct avl_node*)node;
  }

  // u_bm_block("avl hash map", N) {
  //   u_each (i, N) {
  //     avl_map_add(&map, (void*)(intptr_t)i, (void*)(intptr_t)i, nullptr);
  //   }
  // }

  u_bm_block("avl tree", N) {
    u_each (i, N) {
      node      = u_talloc(MyNode);
      node->key = i;
      node->val = i;
      avl_node_add(&tree, (struct avl_node*)node, avl_node_compare, dup);
    }
  }
#endif

  // #undef N
  // #define N 100'0000

#if 1
  /* libu */
  /* #[[map<int, int>]] */
  auto m = u_map_new(int, int);
  /* #[[tree<int, int>]] */
  auto t = u_tree_new(int, int, fn_cmp(int));

#  define u_bench(msg, cnt, ...)                                                                   \
    do {                                                                                           \
      struct timespec __st = {};                                                                   \
      struct timespec __et = {};                                                                   \
      u64_t __take         = 10'0000'0000L;                                                        \
      u64_t __s            = 0; /* second */                                                       \
      u64_t __ms           = 0; /* millisecond */                                                  \
      u64_t __us           = 0; /* microsecond */                                                  \
      u64_t __ns           = 0; /* nanosecond */                                                   \
      u64_t __ave          = 0;                                                                    \
                                                                                                   \
      clock_gettime(CLOCK_MONOTONIC, &__st);                                                       \
      for (i64_t i = 0; i < cnt; i++) {                                                            \
        __VA_ARGS__                                                                                \
      }                                                                                            \
      clock_gettime(CLOCK_MONOTONIC, &__et);                                                       \
                                                                                                   \
      __take = 10'0000'0000L * (__et.tv_sec - __st.tv_sec) + (__et.tv_nsec - __st.tv_nsec);        \
                                                                                                   \
      __ave  = __take / cnt;                                                                       \
      __ns   = __take % 1000;                                                                      \
      __take = __take / 1000;                                                                      \
      __us   = __take % 1000;                                                                      \
      __take = __take / 1000;                                                                      \
      __ms   = __take % 1000;                                                                      \
      __take = __take / 1000;                                                                      \
      __s    = __take % 1000;                                                                      \
      __take = __take / 1000;                                                                      \
                                                                                                   \
      fprintf(stderr, "Total time: %zus, %zums, %zuus, %zuns. ", __s, __ms, __us, __ns);           \
                                                                                                   \
      if (cnt != 1) {                                                                              \
        fprintf(stderr, "Average time: %zuns/%zu\n", __ave, (size_t)cnt);                          \
      }                                                                                            \
    } while (0)

  // u_bench("map put", N, {
  //   ;
  //   u_tree_put(t, i, i);
  // });

  // u_bm_block("map", N) {
  //   u_each (i, N) {
  //     u_map_put(m, i, i);
  //   }
  // }
  //
  // u_each (i, N) {
  //   auto v = u_map_pop(m, i);
  //   u_dbg("[%d] = %d", i, v);
  //   // u_die_if(i != v);
  // }

// u_bm_block("tree", N) {
//   u_each (i, N) {
//     u_tree_put(t, i, i);
//   }
// }
#endif

  return EXIT_SUCCESS;
}
