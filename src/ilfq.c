/* MIT License
 *
 * Copyright (c) 2023 RunThem <iccy.fun@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * */

#include <u/u.h>

typedef struct node_t node_t, *node_ref_t;
struct node_t {
  any_t item;
  _Atomic(node_ref_t) next;
};

typedef struct {
  _Atomic(node_ref_t) head;
  _Atomic(node_ref_t) tail;

  _Atomic(size_t) len;
} lfq_t, *lfq_ref_t;

u_lfq_ref_t lfq_new() {
  lfq_ref_t self  = nullptr;
  node_ref_t node = nullptr;

  self = u_talloc(lfq_t);
  u_nil_if(self);

  node = u_talloc(node_t);
  u_nil_if(node);

  u_atomic_init(&self->len, 0);
  u_atomic_init(&self->head, node);
  u_atomic_init(&self->tail, node);

  return (u_lfq_ref_t)self;

err:
  return nullptr;
}

void lfq_cleanup(u_lfq_ref_t _self) {
  lfq_ref_t self = (lfq_ref_t)_self;

  u_nchk_if(self == nullptr);
  u_nchk_if(u_atomic_pop(&self->len) == 0);

  while (lfq_pop(_self))
    ;

  u_free(self);
}

size_t lfq_len(u_lfq_ref_t _self) {
  lfq_ref_t self = (lfq_ref_t)_self;

  u_chk_if(self == nullptr, 0);

  return u_atomic_pop(&self->len);
}

bool lfq_put(u_lfq_ref_t _self, any_t obj) {
  lfq_ref_t self  = (lfq_ref_t)_self;
  node_ref_t tail = nullptr;
  node_ref_t next = nullptr;
  node_ref_t node = nullptr;

  u_chk_if(self == nullptr, false);
  u_chk_if(obj == nullptr, false);

  node = u_talloc(node_t);
  u_nil_if(node);

  node->item = obj;
  node->next = nullptr;

  while (true) {
    tail = u_atomic_pop(&self->tail);
    next = u_atomic_pop(&tail->next);

    if (u_atomic_pop(&self->tail) != tail) {
      continue;
    }

    if (next != nullptr) {
      u_atomic_cswap(&self->tail, tail, next);
      continue;
    }

    if (u_atomic_cswap(&tail->next, next, node)) {
      break;
    }
  }

  u_atomic_cswap(&self->tail, tail, node);
  u_atomic_add(&self->len, 1);

  return true;

err:
  return false;
}

any_t lfq_pop(u_lfq_ref_t _self) {
  lfq_ref_t self  = (lfq_ref_t)_self;
  node_ref_t head = nullptr;
  node_ref_t tail = nullptr;
  node_ref_t next = nullptr;

  u_chk_if(self == nullptr, nullptr);

  while (true) {
    head = u_atomic_pop(&self->head);
    tail = u_atomic_pop(&self->tail);
    next = u_atomic_pop(&head->next);

    if (head != u_atomic_pop(&self->head)) {
      continue;
    }

    if (head == tail) {
      u_nil_if(next);

      u_atomic_cswap(&self->tail, tail, next);
      continue;
    }

    if (u_atomic_cswap(&self->head, head, next)) {
      break;
    }
  }

  u_free(head);

  u_atomic_sub(&self->len, 1);

  return next->item;

err:
  return nullptr;
}
