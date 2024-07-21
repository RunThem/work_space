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
 * doc:
 *    [](https://zyy.rs/post/ucontext-usage-and-coroutine/)
 *    [](https://www.cnblogs.com/adinosaur/p/5889014.html)
 *    [](https://zhuanlan.zhihu.com/p/82492121)
 *    [](https://juejin.cn/post/7173648788551434247)
 *    [](https://juejin.cn/post/7122978663137542175)
 *    [](https://lib.miaobai.net/blog/context-switch.html)
 *    [](http://jinke.me/2018-09-14-coroutine-context-switch/)
 *    [](https://github.com/dreamsxin/example/blob/master/context/context.md)
 *    [](https://tboox.org/cn/2016/10/28/coroutine-context/)
 *
 * */

#pragma once
#ifndef U_ITASK_H__
#  define U_ITASK_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern any_t    task_new      (any_t);
extern void     task_yield    ();
extern void     task_loop     ();
extern bool     task_delay    (i64_t, i64_t);
extern int      task_socket   (int, int, int);
extern int      task_accept   (int, struct sockaddr*, socklen_t*);
extern int      task_connect  (int, struct sockaddr*, socklen_t);
extern ssize_t  task_read     (int, void*, size_t);
extern ssize_t  task_recv     (int, void*, size_t, int);
extern ssize_t  task_recvfrom (int, void*, size_t, int, struct sockaddr*, socklen_t*);
extern ssize_t  task_write    (int, const void*, size_t);
extern ssize_t  task_send     (int, const void*, size_t, int);
extern ssize_t  task_sendto   (int, const void*, size_t, int, const struct sockaddr*, socklen_t);
/* clang-format on */

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_task_new(fun, ...)                                                                     \
    do {                                                                                           \
      any_t _t = task_new(fun);                                                                    \
                                                                                                   \
      makecontext(_t, any(fun), u_va_cnt(__VA_ARGS__) u_va_list(0, __VA_ARGS__));                  \
    } while (0)

#  define u_task_yield()                                                                           \
    do {                                                                                           \
      task_yield();                                                                                \
    } while (0)

#  define u_task_loop(start, ...)                                                                  \
    do {                                                                                           \
      u_task_new(start __VA_OPT__(, ) __VA_ARGS__);                                                \
                                                                                                   \
      task_loop();                                                                                 \
    } while (0)

#  define u_task_delay(sec, nsec)                                                                  \
    ({                                                                                             \
      ;                                                                                            \
      task_delay(sec, nsec);                                                                       \
    })

#  define u_task_socket(domain, type, protocol)                                                    \
    ({                                                                                             \
      ;                                                                                            \
      task_socket(domain, type, protocol);                                                         \
    })

#  define u_task_accept(fd, addr, len)                                                             \
    ({                                                                                             \
      ;                                                                                            \
      task_accept(fd, addr, len);                                                                  \
    })

#  define u_task_connect(fd, name, namelen)                                                        \
    ({                                                                                             \
      ;                                                                                            \
      task_connect(fd, name, namelen);                                                             \
    })

#  define u_task_read(fd, buf, count)                                                              \
    ({                                                                                             \
      ;                                                                                            \
      task_read(fd, buf, count);                                                                   \
    })

#  define u_task_recv(fd, buf, n, flags)                                                           \
    ({                                                                                             \
      ;                                                                                            \
      task_recv(fd, buf, n, flags);                                                                \
    })

#  define u_task_recvfrom(fd, buf, len, flags, addr, addrlen)                                      \
    ({                                                                                             \
      ;                                                                                            \
      task_recvfrom(fd, buf, len, flags, addr, addrlen);                                           \
    })

#  define u_task_write(fd, buf, count)                                                             \
    ({                                                                                             \
      ;                                                                                            \
      task_write(fd, buf, count);                                                                  \
    })

#  define u_task_send(fd, buf, n, flags)                                                           \
    ({                                                                                             \
      ;                                                                                            \
      task_send(fd, buf, n, flags);                                                                \
    })

#  define u_task_sendto(fd, buf, len, flags, addr, addrlen)                                        \
    ({                                                                                             \
      ;                                                                                            \
      task_sendto(fd, buf, len, flags, addr, addrlen);                                             \
    })

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ITASK_H__ */
