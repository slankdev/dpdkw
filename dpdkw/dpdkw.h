
#ifndef _DPDKW_DPDKW_H_
#define _DPDKW_DPDKW_H_

#include <dpdkw/hdr.h>

inline void wrte_eth_dev_get_mtu(uint16_t port_id, uint16_t* mtu)
{
	int ret = rte_eth_dev_get_mtu(port_id, mtu);
	if (ret < 0) {
		switch (ret) {
			default:
				printf("unknown error %s:%d\n", __FILE__, __LINE__);
				exit(1);
		}
	}
}

inline void wrte_eth_dev_set_mtu(uint16_t port_id, uint16_t mtu)
{
	int ret = rte_eth_dev_set_mtu(port_id, mtu);
	if (ret < 0) {
		switch (ret) {
			default:
				printf("unknown error %s:%d\n", __FILE__, __LINE__);
				exit(1);
		}
	}
}


#endif /* _DPDKW_DPDKW_H_ */

