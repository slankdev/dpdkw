
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

inline static bool
wrte_eth_link_is_up(uint16_t portid)
{
	struct rte_eth_link link;
	memset(&link, 0, sizeof(link));
	rte_eth_link_get_nowait(portid, &link);
	return link.link_status == ETH_LINK_UP;
}

inline static void
w_mbuf_set(struct rte_mbuf* m, uint8_t* ptr, size_t len)
{
	uint8_t* mp = rte_pktmbuf_mtod(m, uint8_t*);
	memcpy(mp, ptr, len);
	m->pkt_len = len;
	m->data_len = len;
}

inline static void 
dump_mbuf(const struct rte_mbuf* m)
{
	printf("mbuf@%p\n", m);
	printf("buf_addr: %p\n", m->buf_addr);
	printf("nb_segs : %u\n", m->nb_segs);
	printf("pkt_len : %u\n", m->pkt_len);
	printf("data_len: %u\n", m->data_len);
	printf("buf_len : %u\n", m->buf_len);
	printf("next    : %p\n", m->next);
}

#endif /* _DPDKW_DPDKW_H_ */

