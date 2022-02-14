#include "core/gen/gen_dynamic.h"

#include <string>
#include <vector>

#include "base/config.h"

std::string FilteringStatement(const struct config& cfg) {
  const struct filter filter = cfg.if_filter;
  std::vector<std::string> filter_elements;
  bool use_udph = false;

  // If config has filtering attributes, convert them into string.
  // udp_src
  if (filter.udp_src.has_value()) {
    use_udph = true;
    filter_elements.push_back("udph->source==bpf_htons(" +
                              std::to_string(filter.udp_src.value()) + ")");
  }

  // udp_dest
  if (filter.udp_dest.has_value()) {
    use_udph = true;
    filter_elements.push_back("udph->dest==bpf_htons(" +
                              std::to_string(filter.udp_dest.value()) + ")");
  }

  // udp_len
  if (filter.udp_len.has_value()) {
    use_udph = true;
    filter_elements.push_back("udph->len==bpf_htons(" +
                              std::to_string(filter.udp_len.value()) + ")");
  }

  // udp_check
  if (filter.udp_check.has_value()) {
    use_udph = true;
    filter_elements.push_back("udph->check==bpf_htons(" +
                              std::to_string(filter.udp_check.value()) + ")");
  }

  std::string s;
  if (cfg.use_udp) {
    s += "udph&&";
  } else if (cfg.use_tcp) {
    s += "tcph&&";
  } else if (cfg.use_icmp) {
    s += "icmph&&";
  }

  for (const auto& elements : filter_elements) {
    s += elements;
    s += "&&";
  }
  s = s.substr(0, s.length() - 2);

  std::string ret = "if(" + s + ")";
  return ret;
}

std::string RewriteStatement(const struct config& cfg) {
  std::string s = "{";
  const struct filter filter = cfg.then_filter;

  // tcp_src
  if (filter.tcp_src.has_value()) {
    s += "tcph->src=bpf_htons(";
    s += std::to_string(filter.tcp_src.value());
    s += ");";
  }

  // tcp_dest
  if (filter.tcp_dest.has_value()) {
    s += "tcph->dest=bpf_htons(";
    s += std::to_string(filter.tcp_dest.value());
    s += ");";
  }

  // tcp_seq
  if (filter.tcp_seq.has_value()) {
    s += "tcph->seq=bpf_htons(";
    s += std::to_string(filter.tcp_seq.value());
    s += ");";
  }

  // tcp_ack_seq
  if (filter.tcp_ack_seq.has_value()) {
    s += "tcph->ack_seq=bpf_htons(";
    s += std::to_string(filter.tcp_ack_seq.value());
    s += ");";
  }

  // tcp_urg
  if (filter.tcp_urg.has_value()) {
    if (filter.tcp_urg.value()) {
      s += "tcph->urg=1;";
    } else {
      s += "tcph->urg=0;";
    }
  }

  // tcp_ack
  if (filter.tcp_ack.has_value()) {
    if (filter.tcp_ack.value()) {
      s += "tcph->ack=1;";
    } else {
      s += "tcph->ack=0;";
    }
  }

  // tcp_psh
  if (filter.tcp_psh.has_value()) {
    if (filter.tcp_psh.value()) {
      s += "tcph->psh=1;";
    } else {
      s += "tcph->psh=0;";
    }
  }

  // tcp_rst
  if (filter.tcp_rst.has_value()) {
    if (filter.tcp_rst.value()) {
      s += "tcph->rst=1;";
    } else {
      s += "tcph->rst=0;";
    }
  }

  // tcp_syn
  if (filter.tcp_syn.has_value()) {
    if (filter.tcp_syn.value()) {
      s += "tcph->syn=1;";
    } else {
      s += "tcph->syn=0;";
    }
  }

  // tcp_fin
  if (filter.tcp_fin.has_value()) {
    if (filter.tcp_fin.value()) {
      s += "tcph->fin=1;";
    } else {
      s += "tcph->fin=0;";
    }
  }

  // udp_dest
  if (filter.udp_dest.has_value()) {
    s += "udph->dest=bpf_htons(";
    s += std::to_string(filter.udp_dest.value());
    s += ");";
  }

  // udp_len
  if (filter.udp_len.has_value()) {
    s += "udph->len=bpf_htons(";
    s += std::to_string(filter.udp_len.value());
    s += ");";
  }

  // udp_check
  if (filter.udp_check.has_value()) {
    s += "udph->check=bpf_htons(";
    s += std::to_string(filter.udp_check.value());
    s += ");";
  }

  s += "}else {return XDP_PASS;}\n";
  return s;
}
