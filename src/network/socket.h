/*
 Wrapper for standard C libraries networking facilities
*/

#pragma once

#include "utils/common.h"

#include <string>

struct sockaddr_in;

class socket_t {
public:
  enum class recv_mode_t {
    msg_peek,
    not_set
  };

  enum class recv_code_t {
    OK,
    ERROR,
    NOT_FINISHED
  };

  struct recv_ret {
    size_t total_recv_size{};
    recv_code_t code{};
  };

private:
  using fd_t = int;
  fd_t fd; // socket descriptor

public:
  explicit socket_t();
  ~socket_t();
  DISALLOW_COPY_AND_MOVE (socket_t);

  template <recv_mode_t mode>
  recv_ret recv(char *buf, size_t buf_len);
  ret_code_t send(const std::string &request);
  ret_code_t connect(sockaddr_in &addr_in);
};