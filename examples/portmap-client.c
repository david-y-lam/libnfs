/* 
   Copyright (C) by Ronnie Sahlberg <ronniesahlberg@gmail.com> 2014
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

/* Example program using the lowlevel raw interface.
 * This allow accurate control of the exact commands that are being used.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef WIN32
#include "win32_compat.h"
#endif

#ifdef HAVE_POLL_H
#include <poll.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libnfs-zdr.h"
#include "libnfs.h"
#include "libnfs-raw.h"
#include "libnfs-raw-mount.h"
#include "libnfs-raw-nfs.h"
#include "libnfs-raw-portmap.h"
#include "libnfs-raw-rquota.h"

struct client {
       int is_finished;
};

void pmap2_dump_cb(struct rpc_context *rpc, int status, void *data, void *private_data)
{
	struct client *client = private_data;
	struct pmap2_dump_result *dr = data;
	struct pmap2_mapping_list *list = dr->list;

	if (status == RPC_STATUS_ERROR) {
		printf("PORTMAP2/DUMP call failed with \"%s\"\n", (char *)data);
		exit(10);
	}
	if (status != RPC_STATUS_SUCCESS) {
		printf("PORTMAP2/DUMP call failed, status:%d\n", status);
		exit(10);
	}

	printf("PORTMAP2/DUMP:\n");
	while (list) {
		printf("	Prog:%d Vers:%d Protocol:%d Port:%d\n",
			list->map.prog,
			list->map.vers,
			list->map.prot,
			list->map.port);
		list = list->next;
	}
	client->is_finished = 1;
}

void pmap3_dump_cb(struct rpc_context *rpc, int status, void *data, void *private_data)
{
	struct client *client = private_data;
	struct pmap3_dump_result *dr = data;
	struct pmap3_mapping_list *list = dr->list;

	if (status == RPC_STATUS_ERROR) {
		printf("PORTMAP3/DUMP call failed with \"%s\"\n", (char *)data);
		exit(10);
	}
	if (status != RPC_STATUS_SUCCESS) {
		printf("PORTMAP3/DUMP call failed, status:%d\n", status);
		exit(10);
	}

	printf("PORTMAP3/DUMP:\n");
	while (list) {
		printf("	Prog:%d Vers:%d Netid:%s Addr:%s Owner:%s\n",
			list->map.prog,
			list->map.vers,
			list->map.netid,
			list->map.addr,
			list->map.owner);
		list = list->next;
	}
	client->is_finished = 1;
}

void pmap2_null_cb(struct rpc_context *rpc, int status, void *data, void *private_data)
{
	struct client *client = private_data;

	if (status == RPC_STATUS_ERROR) {
		printf("PORTMAP2/NULL call failed with \"%s\"\n", (char *)data);
		exit(10);
	}
	if (status != RPC_STATUS_SUCCESS) {
		printf("PORTMAP2/NULL call failed, status:%d\n", status);
		exit(10);
	}

	printf("PORTMAP2/NULL responded and server is alive\n");
	client->is_finished = 1;
}

void pmap3_null_cb(struct rpc_context *rpc, int status, void *data, void *private_data)
{
	struct client *client = private_data;

	if (status == RPC_STATUS_ERROR) {
		printf("PORTMAP3/NULL call failed with \"%s\"\n", (char *)data);
		exit(10);
	}
	if (status != RPC_STATUS_SUCCESS) {
		printf("PORTMAP3/NULL call failed, status:%d\n", status);
		exit(10);
	}

	printf("PORTMAP3/NULL responded and server is alive\n");
	client->is_finished = 1;
}

void pmap_null_cb(struct rpc_context *rpc, int status, void *data, void *private_data)
{
	struct client *client = private_data;

	if (status == RPC_STATUS_ERROR) {
		printf("PORTMAP/NULL call failed with \"%s\"\n", (char *)data);
		exit(10);
	}
	if (status != RPC_STATUS_SUCCESS) {
		printf("PORTMAP/NULL call failed, status:%d\n", status);
		exit(10);
	}

	client->is_finished = 1;
}

void pmap_connect_cb(struct rpc_context *rpc, int status, void *data _U_, void *private_data)
{
	struct client *client = private_data;

	if (status != RPC_STATUS_SUCCESS) {
		printf("connection to portmapper failed\n");
		exit(10);
	}

	if (rpc_pmap2_null_async(rpc, pmap_null_cb, client) != 0) {
		printf("Failed to send null request\n");
		exit(10);
	}
}


static void wait_until_finished(struct rpc_context *rpc, struct client *client)
{
	struct pollfd pfd;

	client->is_finished = 0;
	for (;;) {
		pfd.fd = rpc_get_fd(rpc);
		pfd.events = rpc_which_events(rpc);

		if (poll(&pfd, 1, -1) < 0) {
			printf("Poll failed");
			exit(10);
		}
		if (rpc_service(rpc, pfd.revents) < 0) {
			printf("rpc_service failed\n");
			break;
		}
		if (client->is_finished) {
			break;
		}
	}
}

int main(int argc _U_, char *argv[] _U_)
{
	struct rpc_context *rpc;
	struct client client;
	char *server = NULL;
	int i;
	int null2 = 0;
	int dump2 = 0;
	int null3 = 0;
	int dump3 = 0;
	int command_found = 0;

	rpc = rpc_init_context();
	if (rpc == NULL) {
		printf("failed to init context\n");
		exit(10);
	}

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "dump2")) {
			dump2 = 1;
			command_found++;
		} else if (!strcmp(argv[i], "null2")) {
			null2 = 1;
			command_found++;
		} else if (!strcmp(argv[i], "dump3")) {
			dump3 = 1;
			command_found++;
		} else if (!strcmp(argv[i], "null3")) {
			null3 = 1;
			command_found++;
		} else {
			server = argv[i];
		}
	}
	if (command_found == 0 || server == NULL) {
		fprintf(stderr, "Usage: portmap-client <command*> <server>\n");
		exit(10);
	}

	if (rpc_connect_async(rpc, server, 111, pmap_connect_cb, &client) != 0) {
		printf("Failed to start connection\n");
		exit(10);
	}
	wait_until_finished(rpc, &client);

	if (null2) {
		if (rpc_pmap2_null_async(rpc, pmap2_null_cb, &client) != 0) {
			printf("Failed to send NULL2 request\n");
			exit(10);
		}
		wait_until_finished(rpc, &client);
	}
	if (dump2) {
		if (rpc_pmap2_dump_async(rpc, pmap2_dump_cb, &client) != 0) {
			printf("Failed to send DUMP2 request\n");
			exit(10);
		}
		wait_until_finished(rpc, &client);
	}
	if (null3) {
		if (rpc_pmap3_null_async(rpc, pmap3_null_cb, &client) != 0) {
			printf("Failed to send NULL3 request\n");
			exit(10);
		}
		wait_until_finished(rpc, &client);
	}
	if (dump3) {
		if (rpc_pmap3_dump_async(rpc, pmap3_dump_cb, &client) != 0) {
			printf("Failed to send DUMP3 request\n");
			exit(10);
		}
		wait_until_finished(rpc, &client);
	}

	
	rpc_destroy_context(rpc);
	rpc=NULL;
	return 0;
}
