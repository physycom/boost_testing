/* Copyright 2015 - Marco Di Cristina, Alessandro Fabbri */

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

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "serialization_classes.hpp"

int main() {
  Geopnt p1(1, 2), p2(3, 4);
  vector<Geopnt> vp{ p1, p2 };
  Boundary b("my_boundary", 10, vp), b2, b3;
  cout << "BOUNDARY (stored)" << endl << b << endl;

  // With files
  ofstream ofs("serialized.bin", ofstream::binary);
  {
    boost::archive::binary_oarchive oa(ofs);
    oa << b;
  }
  ofs.close();

  ifstream ifs("serialized.bin", ifstream::binary);
  {
    boost::archive::binary_iarchive ia(ifs);
    ia >> b2;
  }
  ifs.close();
  cout << "BOUNDARY (imported)" << endl << b << endl;

  // With streams
  stringstream ss(std::ios::binary | std::ios::out | std::ios::in);
  {
    boost::archive::binary_oarchive oa(ss);
    oa << b;
  }

  {
    boost::archive::binary_iarchive ia(ss);
    ia >> b3;
  }
  cout << "BOUNDARY (from stream)" << endl << b3 << endl;

  cout << endl << "Hit ENTER to quit..."; cin.get();
  return 0;
}