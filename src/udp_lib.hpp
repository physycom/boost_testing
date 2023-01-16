/* Copyright 2016 - Alessandro Fabbri */

/***************************************************************************
This file is part of boost_testing.

boost_testing is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

boost_testing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with boost_testing. If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

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
