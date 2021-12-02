#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <cstdint>
#include <string>

struct datarec {
  uint64_t rx_packets;
  uint64_t rx_bytes;
};

enum class Action {
	Pass = 0,
	Drop = 1,
};

struct Policy {
	Action action = Action::Pass;
  int priority = -1;
  int port = -1;
  std::string ip_address;
  std::string protocol;
};

struct config {
  uint32_t xdp_flags;
  unsigned int ifindex;
	std::string ifname;
	std::string bpf_filepath;
  std::string progsec;
	bool unload;
	std::string yaml_filepath;
};

#define EXIT_OK 0
#define EXIT_FAIL 1
#define EXIT_FAIL_XDP 30
#define EXIT_FAIL_BPF 40
#define EXIT_FAIL_MAP 50
#define EXIT_SIGNAL 100

#endif /* _DEFINE_H_ */
