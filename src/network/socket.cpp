#include "socket.h"

#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>

socket_t::socket_t ()
{
  if ((fd = ::socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    throw std::runtime_error ("Cannot open a new socket.");
}

socket_t::~socket_t ()
{
  ::close (fd);
}

ret_code_t socket_t::connect(sockaddr_in &addr_in) {
  return (::connect(fd, (struct sockaddr *)&addr_in, sizeof(addr_in)) == 0
              ? ret_code_t::OK
              : ret_code_t::ERROR);
}

ret_code_t socket_t::send(const std::string &request) {
  for (size_t size_already_send = 0; size_already_send < request.size();) {

    size_t size_left_to_send = request.size() - size_already_send;
    const char *buf = request.c_str() + size_already_send;
    int size_send = ::send(fd, buf, size_left_to_send, 0 /*default flags*/);
    if (size_send == -1)
      return ret_code_t::ERROR;

    size_already_send += size_send;
  }
  return ret_code_t::OK;
}

template <socket_t::recv_mode_t mode>
socket_t::recv_ret socket_t::recv(char *buf, size_t buf_len)
{
  const int flags = (mode == recv_mode_t::msg_peek ? MSG_PEEK : 0);

  size_t size_already_recv{};
  for (size_already_recv = 0; size_already_recv < buf_len;) {

    size_t size_left_to_recv = buf_len - size_already_recv;
    int size_recv = ::recv (fd, buf + size_already_recv, size_left_to_recv, flags);
    if (size_recv == -1) {
      return {size_already_recv, recv_code_t::ERROR};
    }
    else if (size_recv == 0) { //< nothing to receive more
      return {size_already_recv, recv_code_t::OK};
    }

    size_already_recv += size_recv;
  }

  // If we don't exit the loop, receive package might be unfinished
  return {size_already_recv, recv_code_t::NOT_FINISHED};
}

#define RECV_INSTANTIATIONS(MODE) \
template socket_t::recv_ret socket_t::recv<MODE>(char *buf, size_t buf_len)
RECV_INSTANTIATIONS(socket_t::recv_mode_t::msg_peek);
RECV_INSTANTIATIONS(socket_t::recv_mode_t::not_set);
#undef RECV_INSTANTIATIONS