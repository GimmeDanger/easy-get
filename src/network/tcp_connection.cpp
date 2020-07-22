#include "tcp_connection.h"

// glibc networking libs
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <cstring>
#include <sstream>

std::string tcp_connection::to_string(tcp_connection::reset_code_t code) {
  switch (code) {
  case reset_code_t::OK:
    return "success";
  case reset_code_t::SOCK_CONSTR_ERR:
    return "couldn't open new socket";
  case reset_code_t::SOCKET_CONN_ERR:
    return "couldn't start the connection";
  case reset_code_t::HOSTNAME_TO_IP_ERR:
    return "couldn't convert host name to ip";
  case reset_code_t::IP_TO_BIN_ERR:
    return "couldn't convert ip to binary";
  default:
    throw std::invalid_argument("unknown reset code");
  }
}

tcp_connection::tcp_connection(const std::string &_host_name,
                               unsigned short _host_port) {
  auto ret = reset(_host_name, _host_port);
  if (ret != reset_code_t::OK) {
    std::stringstream ss;
    ss << "Cannot create new tcp connection for ";
    ss << "(" << _host_name << ", " << _host_port << "): ";
    ss << to_string(ret);
    throw std::runtime_error(ss.str());
  }
}

void tcp_connection::clear ()
{
  socket.reset();
  host_name.clear();
  host_ip.clear();
  host_port = default_port;
}

tcp_connection::~tcp_connection()
{
  close ();
}

void tcp_connection::close () noexcept
{
  clear ();
}

ret_code_t tcp_connection::send(const std::string &request)
{
  return socket ? socket->send(request) : ret_code_t::ERROR;
}

tcp_connection::reset_code_t tcp_connection::reset(const std::string &_host_name, const unsigned short _host_port)
{
  close ();

  auto opt_host_ip = get_ip_by_hostname (_host_name);
  if (!opt_host_ip.has_value())
    return reset_code_t::HOSTNAME_TO_IP_ERR;

  host_ip = opt_host_ip.value ();
  host_name = _host_name;
  host_port = _host_port;

  // Try open new socket
  try {
    socket = std::make_unique<socket_t>();
  } catch (...) {
    return reset_code_t::SOCK_CONSTR_ERR;
  }

  // Construct the server address structure
  sockaddr_in server_addr {};
  memset (&server_addr, 0, sizeof (server_addr));
  server_addr.sin_port = htons (host_port);
  server_addr.sin_family = AF_INET; // internet address family
  if (inet_pton (AF_INET, host_ip.c_str (), &server_addr.sin_addr) <= 0)
    return reset_code_t::IP_TO_BIN_ERR;

  if (socket->connect (server_addr) != ret_code_t::OK)
    return reset_code_t::SOCKET_CONN_ERR;

  return reset_code_t::OK;
}

std::optional<std::string> tcp_connection::get_ip_by_hostname (const std::string &_host_name)
{
  addrinfo hints{};
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo *servinfo = nullptr;
  if (getaddrinfo (_host_name.c_str(), "http", &hints, &servinfo))
    return {};

  char buf[64];
  for (auto *p = servinfo; p != nullptr; p = p->ai_next)
  {
    auto *h = (struct sockaddr_in*) (p->ai_addr);
    strcpy (buf, inet_ntoa (h->sin_addr));
  }
  freeaddrinfo (servinfo);
  return {std::string(buf)};
}

template <socket_t::recv_mode_t mode>
socket_t::recv_ret tcp_connection::recv(char *buf, size_t buf_len) {
  return (socket ? socket->recv<mode>(buf, buf_len)
                 : socket_t::recv_ret{0, socket_t::recv_code_t::ERROR});
}

#define RECV_INSTANTIATIONS(MODE) \
template socket_t::recv_ret tcp_connection::recv<MODE>(char *buf, size_t buf_len)
RECV_INSTANTIATIONS(socket_t::recv_mode_t::msg_peek);
RECV_INSTANTIATIONS(socket_t::recv_mode_t::not_set);
#undef RECV_INSTANTIATIONS