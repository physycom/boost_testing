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

int main(int argc, char** argv) {
  std::string loc_addr, loc_port, rem_addr, rem_port;
  if ( argc == 5 ) {
    loc_addr = argv[3];
    loc_port = argv[4];
    rem_addr = argv[1];
    rem_port = argv[2];
  }
  else if (argc == 2 && std::string(argv[1]) == "def" ) {
    loc_addr = "127.0.0.1";
    loc_port = "22222";
    rem_addr = "127.0.0.1";
    rem_port = "11111";
  }
  else {
    std::cout << "Usage: " << argv[0] << " local_addr local_port remote_addr remote_port" << std::endl;
    std::cout << "       " << " Set up UDP server listening on local_addr:local_port and sending ACK message to remote_addr:remote_port" << std::endl;
    std::cout << "Usage: " << " def" << std::endl;
    std::cout << "       " << " Set up UDP client sending messages on 127.0.0.1:11111 and receiving ACK message to 127.0.0.1:22222" << std::endl;
    exit(1);
  }
  
  std::cout << "**************************" << std::endl;
  std::cout << "**** BOOST UDP CLIENT ****" << std::endl;
  std::cout << "**************************" << std::endl;

  boost::asio::io_service ioService;
  UDPConnection udp_con(ioService, loc_addr, loc_port, rem_addr, rem_port);

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