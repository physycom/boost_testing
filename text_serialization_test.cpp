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

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "boundary_class.hpp"

int main() {
  Geopnt p1(1, 2), p2(3, 4);
  vector<Geopnt> vp{ p1, p2 };
  Boundary b("my_boundary", 10, vp), b2;
  cout << "BOUNDARY (stored)" << endl << b << endl;
  
  ofstream ofs("serialized.txt"); boost::archive::text_oarchive oa(ofs);
  oa << b;
  ofs.close();

  ifstream ifs("serialized.txt", ifstream::binary);
  boost::archive::text_iarchive ia(ifs);
  ia >> b2;
  ifs.close();
  cout << "BOUNDARY (imported)" << endl << b << endl;

  cout << endl << "Hit ENTER to quit..."; cin.get();
  return 0;
}
