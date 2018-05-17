
#include <stdint.h>
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


uint8_t pkt[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ether dst
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ether src
		0xee, 0xee,                         // ether type

		/* data */
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 

		/* data */
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 
};

uint8_t data34[] = {
		/* data */
		0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 
		0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 
		0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 
		0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 

		/* data */
		0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
		0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
		0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
		0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 
};




int main(int argc, char *argv[])
{
	struct rte_mempool *mbuf_pool;
	unsigned nb_ports;

	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

	nb_ports = rte_eth_dev_count();
	if (nb_ports < 2 || (nb_ports & 1))
		rte_exit(EXIT_FAILURE, "Error: number of ports must be even\n");

	mbuf_pool = wmp_alloc("MBUF_POOL", rte_socket_id(), 81920);
	uint16_t portid = 1;
	port_init(portid, mbuf_pool);

	struct rte_mbuf* m = rte_pktmbuf_alloc(mbuf_pool);
	uint8_t* ptr = rte_pktmbuf_mtod(m, uint8_t*);
  memcpy(ptr, pkt, sizeof(pkt));
	m->pkt_len = sizeof(pkt);
	m->data_len = sizeof(pkt);

	struct rte_mbuf* d = rte_pktmbuf_alloc(mbuf_pool);
	uint8_t* dptr = rte_pktmbuf_mtod(d, uint8_t*);
  memcpy(dptr, data34, sizeof(data34));
	d->pkt_len = sizeof(data34);
	d->data_len = sizeof(data34);

	// wrte_pktmbuf_chain(m, d);

	ret = rte_eth_tx_burst(1, 0, &m, 1);
	if (ret != 1) {
		printf("error ocure\n");
		exit(1);
	}
	return 0;
}


