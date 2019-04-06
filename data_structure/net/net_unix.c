#include "net_unix.h"

void net_unix_addr_set(net_t *net, char *path)
{
	struct sockaddr_un addr;

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, path, strlen(path));

	net_address_set(net, (struct sockaddr*)&addr, sizeof(addr));

	return;
}

