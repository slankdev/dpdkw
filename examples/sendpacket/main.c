
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <dpdkw/dpdkw.h>

#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

static const struct rte_eth_conf port_conf_default = {
	.rxmode = { .max_rx_pkt_len = ETHER_MAX_LEN }
};

static inline void
port_init(uint16_t port, struct rte_mempool *mbuf_pool)
{
	struct rte_eth_conf port_conf = port_conf_default;
	uint16_t nb_rxd = 128;
	uint16_t nb_txd = 512;

	if (port >= rte_eth_dev_count()) {
		printf("port number is invalid\n");
		exit(1);
	}

	/* Configure the Ethernet device. */
	const uint16_t rx_rings = 1;
	const uint16_t tx_rings = 1;
	int retval = rte_eth_dev_configure(port, rx_rings, tx_rings, &port_conf);
	if (retval != 0) {
		printf("rte_eth_dev_configure\n");
		exit(1);
	}

	retval = rte_eth_dev_adjust_nb_rx_tx_desc(port, &nb_rxd, &nb_txd);
	if (retval != 0) {
		printf("rte_eth_dev_adjust_nb_rx_tx_desc\n");
		exit(1);
	}

	for (uint16_t q = 0; q < rx_rings; q++) {
		retval = rte_eth_rx_queue_setup(port, q, nb_rxd,
				rte_eth_dev_socket_id(port), NULL, mbuf_pool);
		if (retval != 0) {
			printf("rte_eth_rx_queue_setup\n");
			exit(1);
		}
	}

	for (uint16_t q = 0; q < tx_rings; q++) {
		retval = rte_eth_tx_queue_setup(port, q, nb_txd,
				rte_eth_dev_socket_id(port), NULL);
		if (retval != 0) {
			printf("rte_eth_tx_queue_setup\n");
			exit(1);
		}
	}

	/* Start the Ethernet port. */
	retval = rte_eth_dev_start(port);
	if (retval != 0) {
		printf("rte_eth_dev_start\n");
		exit(1);
	}

	/* Display the port MAC address. */
	struct ether_addr addr;
	rte_eth_macaddr_get(port, &addr);
	printf("Port %u MAC: %02" PRIx8 " %02" PRIx8 " %02" PRIx8
			   " %02" PRIx8 " %02" PRIx8 " %02" PRIx8 "\n",
			port,
			addr.addr_bytes[0], addr.addr_bytes[1],
			addr.addr_bytes[2], addr.addr_bytes[3],
			addr.addr_bytes[4], addr.addr_bytes[5]);

	/* Enable RX in promiscuous mode for the Ethernet device. */
	rte_eth_promiscuous_enable(port);
}


uint8_t data[] = {
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
uint8_t data_0x66[2500] = {0x66};
uint8_t data_last[] = {
	0xff, 0xff, 0xff,
};

int main(int argc, char *argv[])
{
	// rte_log_set_global_level(RTE_LOG_DEBUG);
	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

	unsigned nb_ports = rte_eth_dev_count();
	if (nb_ports == 0)
		rte_exit(EXIT_FAILURE, "Error: number of ports must be even\n");

	struct rte_mempool *mbuf_pool;
#if 0
	mbuf_pool = wmp_alloc("MBUF_POOL", rte_socket_id(), 81920);
#else
	const size_t NB_MBUF = 8192;
	const size_t MBUF_SIZE = RTE_MBUF_DEFAULT_DATAROOM + 
			sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM;
	printf("mbufsize: %zd \n", MBUF_SIZE);
	mbuf_pool = rte_mempool_create("MBUF_POOL", NB_MBUF, MBUF_SIZE, 0,
		sizeof(struct rte_pktmbuf_pool_private), 
		rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL,
		rte_socket_id(), MEMPOOL_F_SP_PUT|MEMPOOL_F_SC_GET);
	if (!mbuf_pool) {
		printf("rte_mempool_create\n");
		exit(1);
	}
#endif
	uint16_t portid = 0;
	port_init(portid, mbuf_pool);

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

	printf("before send\n");
	ret = rte_eth_tx_burst(0, 0, &m, 1);
	if (ret != 1) {
		printf("error ocure\n");
		exit(1);
	}
	printf("after send ret:%d\n", ret);
	return 0;
}


