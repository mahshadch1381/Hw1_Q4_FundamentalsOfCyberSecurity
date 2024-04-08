 #include <pcap.h>
#include <stdio.h>

void handle_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    if (header->len >= 34 && packet[12] == 0x08 && packet[23] == 0x01) {
        const u_char *icmp_payload = packet + 34;  
        printf("ICMP payload: ");
        int i;
        for (i = 23; i < 40 ; i++) {
            printf("%c ", icmp_payload[i]);
        }
        printf("\n");
    }
}

int main() {
    char *dev = "Wi-Fi";  
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live(dev, 65536, 1, 1000, errbuf);

    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 1;
    }

    struct bpf_program filter;
    
    if (pcap_compile(handle, &filter, "icmp", 0, 0) == -1 ||
            pcap_setfilter(handle, &filter) == -1) {
        fprintf(stderr, "Couldn't set filter: %s\n", pcap_geterr(handle));
        return 1;
    }

    pcap_loop(handle, -1, handle_packet, NULL);

    pcap_close(handle);
    return 0;
}
