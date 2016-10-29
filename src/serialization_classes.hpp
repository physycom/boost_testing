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

#include <string>
#include <vector>

#include <boost/serialization/serialization.hpp>                // generic include
#include <boost/serialization/vector.hpp>                       // vector-specialized include

using namespace std;

struct Geopnt {
  int lat, lon;
  Geopnt() {};
  Geopnt(int Lat, int Lon) : lat(Lat), lon(Lon) {};


  friend class boost::serialization::access;
  template<class Archive> void serialize(Archive & ar, const unsigned int /* file_version */) {
    ar & lat & lon;
  }
};

struct Boundary {
  string name;
  int id;
  vector<Geopnt> points;
  Boundary() {};
  Boundary(string Name, int Id, vector<Geopnt> Points) : name(Name), id(Id), points(Points) {};
  friend ostream& operator<<(ostream& os, const Boundary& b) {
    os << b.name << " (" << b.id << ")" << endl;
    os << "{ ";
    for (auto &p : b.points) os << "(" << p.lat << "," << p.lon << ") ";
    os << "}" << endl;
    return os;
  }


  friend class boost::serialization::access;
  template<class Archive> void serialize(Archive & ar, const unsigned int /* file_version */) {
    ar & name & id & points;
  }
};
