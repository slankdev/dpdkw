
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <dpdkw/dpdkw.h>


static const struct rte_eth_conf port_conf_default = {
	.rxmode = { .max_rx_pkt_len = ETHER_MAX_LEN }
};

uint8_t data[1004] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // ether dst
		0xee, 0xee, 0xee, 0xee, 0xee, 0xee, // ether src
		0xee, 0xee,                         // ether type

		/* data */
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
};

uint8_t data_0x33[2000] = {0x33};
uint8_t data_0x44[2000] = {0x44};
uint8_t data_0x55[2000] = {0x55};
uint8_t data_0x66[2000] = {0x66};
uint8_t data_last[] = {'s','l','a','n','k','d','e','v'};

int main(int argc, char *argv[])
{
	wrte_eal_init(argc, argv);

	uint32_t nb_ports = rte_eth_dev_count();
	if (nb_ports == 0)
		rte_exit(EXIT_FAILURE, "Error: number of ports must be even\n");

	struct rte_mempool *mbuf_pool = wmp_alloc("MBUF_POOL", rte_socket_id(), 81920);

	uint16_t portid = 0;
	port_init(portid, mbuf_pool, &port_conf_default);

	memset(data_0x33, 0x33, sizeof(data_0x33));
	memset(data_0x44, 0x44, sizeof(data_0x44));
	memset(data_0x55, 0x55, sizeof(data_0x55));
	memset(data_0x66, 0x66, sizeof(data_0x66));

	struct rte_mbuf* m = rte_pktmbuf_alloc(mbuf_pool);
	struct rte_mbuf* m_0x33 = rte_pktmbuf_alloc(mbuf_pool);
	struct rte_mbuf* m_0x44 = rte_pktmbuf_alloc(mbuf_pool);
	struct rte_mbuf* m_0x55 = rte_pktmbuf_alloc(mbuf_pool);
	struct rte_mbuf* m_0x66 = rte_pktmbuf_alloc(mbuf_pool);
	struct rte_mbuf* m_last = rte_pktmbuf_alloc(mbuf_pool);

	w_mbuf_set(m, data, sizeof(data));
	w_mbuf_set(m_0x33, data_0x33, sizeof(data_0x33));
	w_mbuf_set(m_0x44, data_0x44, sizeof(data_0x44));
	w_mbuf_set(m_0x55, data_0x55, sizeof(data_0x55));
	w_mbuf_set(m_0x66, data_0x66, sizeof(data_0x66));
	w_mbuf_set(m_last, data_last, sizeof(data_last));

  wrte_pktmbuf_chain(m, m_0x33);
  wrte_pktmbuf_chain(m, m_0x44);
  wrte_pktmbuf_chain(m, m_0x55);
  wrte_pktmbuf_chain(m, m_0x66);
  wrte_pktmbuf_chain(m, m_last);

	while (!wrte_eth_link_is_up(0)) {
		printf(".");
		fflush(stdout);
		usleep(100000);
	}
	printf("\n");
	rte_pktmbuf_dump(stdout, m, 0);

	ret = rte_eth_tx_burst(0, 0, &m, 1);
	if (ret != 1) {
		printf("error ocure\n");
		exit(1);
	}
	return 0;
}


