
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

inline struct rte_mempool* 
wmp_alloc(const char* name, size_t socket_id, size_t size)
{
  const size_t MBUF_CACHE_SIZE = 0;
	struct rte_mempool* mp = rte_pktmbuf_pool_create(
      name,
      size,
      MBUF_CACHE_SIZE,
      0,
      RTE_MBUF_DEFAULT_BUF_SIZE,
      socket_id);
  if (!mp) {
    printf("rte_pktmbuf_pool_create(name=%s): ", name);
		exit(1);
  }
  return mp;
}

inline static void
wrte_pktmbuf_chain(struct rte_mbuf* first,
									 struct rte_mbuf* second)
{
	int ret = rte_pktmbuf_chain(first, second);
	if (ret < 0) {
		switch (ret) {
			default:
				printf("unknown error %s:%d\n", __FILE__, __LINE__);
				exit(1);
		}
	}
}


#endif /* _DPDKW_DPDKW_H_ */

