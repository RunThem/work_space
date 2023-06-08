#include "u.h"

#define __vec_header                                                                               \
  (sizeof(struct {                                                                                 \
    size_t len;                                                                                    \
    size_t cap;                                                                                    \
  }))

size_t __vec__cap(size_t size) {
  if (size < 8)
    return 8;
  if (size < 1024)
    return size * 2;
  return size + 512;
}

#undef _
#define _(v) (*(v))

#define __vec_init(vec, arg...)                                                                    \
  ({                                                                                               \
    size_t _v_init__cap = __vec__cap(va_0th(0, arg));                                              \
    _(vec)              = u_talloc(__vec_header + vec_itsize(vec) * _v_init__cap, typeof(_(vec))); \
    _(vec)->cap         = (_(vec) == nullptr) ? -1 : _v_init__cap;                                 \
  })

#define __vec_init_from(vec, item, arg...)                                                         \
  ({                                                                                               \
    typeof(item) _v_init_from__arr[] = {item va_slice(0, arg)};                                    \
    vec_init(vec, arrlen(_v_init_from__arr));                                                      \
    vec_push_b(vec, item, arg);                                                                    \
                                                                                                   \
    (void)_v_init_from__arr;                                                                       \
    _(vec)->len;                                                                                   \
  })

#define __vec_clone(vec)                                                                           \
  ({                                                                                               \
    size_t _v_clone__size = __vec_header + vec_itsize(vec) * _(vec)->cap;                          \
    auto _v_clone__vec    = u_talloc(_v_clone__size, typeof(_(vec)));                              \
    memcpy(_v_clone__vec, _(vec), _v_clone__size);                                                 \
                                                                                                   \
    _v_clone__vec;                                                                                 \
  })

#define __vec_empty(vec) ({ _(vec)->len == 0; })

#define __vec_resize(vec, size)                                                                    \
  ({                                                                                               \
    ssize_t _v_resize__ret = 0;                                                                    \
    if (_(vec)->cap >= (size)) {                                                                   \
      _(vec)->len    = size;                                                                       \
      _v_resize__ret = _(vec)->cap;                                                                \
    } else {                                                                                       \
      _(vec)         = u_realloc(_(vec), __vec_header + vec_itsize(vec) * (size));                 \
      _v_resize__ret = _(vec)->cap = (_(vec) == nullptr) ? -1 : (size);                            \
    }                                                                                              \
                                                                                                   \
    _v_resize__ret;                                                                                \
  })

#define __vec_clear(vec) ({ _(vec)->len = 0; })

#define __vec_cleanup(vec)                                                                         \
  ({                                                                                               \
    ssize_t _v_cleanup__ret = (_(vec) == nullptr) ? -1 : 0;                                        \
    if (_v_cleanup__ret != -2) {                                                                   \
      u_free(_(vec));                                                                              \
      (vec) = nullptr;                                                                             \
    }                                                                                              \
                                                                                                   \
    _v_cleanup__ret;                                                                               \
  })

#define __vec_at(vec, idx)                                                                         \
  ({                                                                                               \
    ssize_t _v_at__ret        = ((idx) < 0 || (idx) >= _(vec)->len) ? -1 : 0;                      \
    vec_T(vec)* _v_at__item_p = nullptr;                                                           \
                                                                                                   \
    if (_v_at__ret != -1) {                                                                        \
      _v_at__item_p = &_(vec)->data[idx];                                                          \
    }                                                                                              \
                                                                                                   \
    _v_at__item_p;                                                                                 \
  })

#define __vec_pop_b(vec)                                                                           \
  ({                                                                                               \
    vec_T(vec) * _v_pop_b__item_p;                                                                 \
                                                                                                   \
    if (_(vec)->len > 0) {                                                                         \
      _(vec)->len--;                                                                               \
      _v_pop_b__item_p = &_(vec)->data[_(vec)->len]                                                \
    } else {                                                                                       \
      bzero(&_v_pop_b__item_p, sizeof(void*));                                                     \
    }                                                                                              \
                                                                                                   \
    _v_pop_b__item_p;                                                                              \
  })

#define __vec_pop_f(vec)                                                                           \
  ({                                                                                               \
    vec_T(vec) * _v_pop_f__item_p;                                                                 \
                                                                                                   \
    if (_(vec)->len > 0) {                                                                         \
      _(vec)->len--;                                                                               \
      _v_pop_f__item_p = &_(vec)->data[0];                                                         \
                                                                                                   \
      memmove(_(vec)->data, &_(vec)->data[1], _(vec)->len* vec_itsize(vec));                       \
    } else {                                                                                       \
      bzero(&_v_pop_f__item_p, vec_itsize(vec));                                                   \
    }                                                                                              \
                                                                                                   \
    _v_pop_f__item_p;                                                                              \
  })

#define __vec_insert(vec, idx, item, arg...)                                                       \
  ({                                                                                               \
    typeof(item) _v_insert__arr[] = {item va_slice(0, arg)};                                       \
    size_t _v_insert__size        = arrlen(_v_insert__arr);                                        \
                                                                                                   \
    auto _v_insert__len    = _(vec)->len;                                                          \
    auto _v_insert__cap    = _(vec)->cap;                                                          \
    ssize_t _v_insert__ret = (idx < 0 || idx > _(vec)->len) ? -1 : 0;                              \
                                                                                                   \
    if (_v_insert__ret != -1) {                                                                    \
      _v_insert__ret = (_v_insert__cap - _v_insert__len < _v_insert__size) ?                       \
                           vec_resize(vec, __vec__cap(_v_insert__len + _v_insert__size)) :         \
                           _v_insert__cap;                                                         \
    }                                                                                              \
                                                                                                   \
    if (_v_insert__ret != -1) {                                                                    \
      if (idx != _(vec)->len) {                                                                    \
        memmove(&_(vec)->data[idx + _v_insert__size],                                              \
                &_(vec)->data[idx],                                                                \
                (_v_insert__len - idx) * vec_itsize(vec));                                         \
      }                                                                                            \
                                                                                                   \
      for (size_t _v_insert__i = 0; _v_insert__i < _v_insert__size; _v_insert__i++) {              \
        _(vec)->data[idx + _v_insert__i] = (vec_T(vec))_v_insert__arr[_v_insert__i];               \
      }                                                                                            \
                                                                                                   \
      _v_insert__ret = _(vec)->len += _v_insert__size;                                             \
    }                                                                                              \
                                                                                                   \
    _v_insert__ret;                                                                                \
  })
