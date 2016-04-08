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

#define _CRT_SECURE_NO_WARNINGS

#include "udp_lib.hpp"
#include "md5.h"

#define CLIENT_ID         0x0101

int main() {
  boost::asio::io_service ioService;
  UDPConnection udp_con(ioService, "localhost", "54321", "localhost", "12345");

  std::string message, text;
  size_t msg_counter = 0;
  std::array<char, 128> buffer;
  boost::system::error_code err;

  for (;;) {
    // wait for user message
    std::cout << std::endl << "Type your message or hit Ctrl-C to quit: ";
    std::getline(std::cin, text);

    // evaluate MD5hash and prepare message 
    std::string md5string = make_md5hash(text);
    message = std::to_string(CLIENT_ID) + ":" + text + ":" + md5string;
    std::cout << "Text : \"" << text << "\"" << std::endl;
    std::cout << "Hash : \"" << md5string << "\"" << std::endl;
    std::cout << "Msg  : \"" << message << "\"" << std::endl;

    // send user message to server
    udp_con.send(message);
    std::cout << std::endl << "Message #" << ++msg_counter << " \"" << message << "\" sent (" << udp_con.len_send << " bytes sent)" << std::endl;

    // receive ack from server
    udp_con.recv(boost::asio::buffer(&buffer[0], buffer.size()), err);
    std::cout << "Server sent ACK #" << msg_counter << " that received ";
    std::cout.write(buffer.data(), udp_con.len_recv);
    std::cout << " bytes." << std::endl;
  }

  return 0;
}