#define uvec_def (bool, int)
#define umap_def ((int, bool), (int, char))
#include <u/u.h>

/* system libs */

#include <stdlib.h>

#if 0
#  include <arpa/inet.h>
#  include <ctype.h>
#  include <dirent.h>
#  include <fcntl.h>
#  include <math.h>
#  include <net/ethernet.h>
#  include <net/if.h>
#  include <netinet/if_ether.h>
#  include <netinet/in.h>
#  include <netinet/ip.h>
#  include <netinet/ip_icmp.h>
#  include <netpacket/packet.h>
#  include <regex.h>
#  include <sys/ioctl.h>
#  include <sys/resource.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <sys/wait.h>
#  include <ucontext.h>
#  include <unistd.h>
#endif

ret_t code = 0;

/*
 * namespace
 *
 * uv: vector
 * um: hash table
 * ut: avl tree
 * ul: list
 * us: string
 * ub: buffer
 * uf: file & directory
 * ug: log
 * un: network
 * */

int main(int argc, const u_cstr_t argv[]) {
  char str[] = "hello";
  int result = typeeq(char[], str);
  infln("result is %d", result);

  infln("%lu", sizeof(enum {T, F}));

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
