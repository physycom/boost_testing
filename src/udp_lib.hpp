#define _WIN32_WINNT      0x0501

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UDPConnection {
private:
  boost::asio::io_service& IoService;
  udp::socket Socket;
  udp::endpoint EP_recv, EP_send;

public:
  size_t len_recv, len_send;

  UDPConnection(boost::asio::io_service& io_service, const std::string& host_recv, const std::string& port_recv, const std::string& host_send, const std::string& port_send)
    : IoService(io_service), Socket(io_service, udp::endpoint(udp::v4(), 0)) {
    udp::resolver resolver(IoService);
    udp::resolver::query query_recv(udp::v4(), host_recv, port_recv);
    udp::resolver::query query_send(udp::v4(), host_send, port_send);
    EP_recv = *resolver.resolve(query_recv);
    EP_send = *resolver.resolve(query_send);
    Socket = udp::socket(io_service, EP_recv);
  }

  ~UDPConnection() {
    Socket.close();
  }

  void send(const std::string& msg) {
    len_send = Socket.send_to(boost::asio::buffer(msg, msg.size()), EP_send);
  }

  void recv(const boost::asio::mutable_buffers_1 & buffer, boost::system::error_code & err) {
    len_recv = Socket.receive(buffer, 0, err);
  }

};
