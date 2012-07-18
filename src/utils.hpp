#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>


using namespace std;

namespace utils {

  template<typename StartIterator, typename StopIterator>
  inline void write(ostream& os, StartIterator startIt, StopIterator stopIt, const char delimiter = ' ') {

    if ( startIt != stopIt ) {
      os << *startIt;
      ++startIt;
    }

    while ( startIt != stopIt ) {
      os << delimiter << *startIt;
      ++startIt;
    }
  }
  
  // Chomps a string, i.e. removes all the trailing end-of-line characters
  string chomp(const string& str, const string& eof = "\r\n");

  // Removes leading and trailing whitespace characters
  string trim(const string& str, const string& wh = " ");
  
  map<string,string> parse(istream& streamObj,
			   const char delimiter,
			   const char separator,
			   const char comment);
  
  // Splits a delimited stream
  vector<string> split(istream& streamObj, const char delimiter, const string& wh = " ");

  template <typename T>
  T str2(const string& str) {

    stringstream ss( chomp(str) );
    T ret;
    ss >> ret;
    
    if ( ss.fail() || ss.bad() || !ss.eof() ) {
      cerr << "utils::convert::str2<T>() -- input '" << str
	   << "' incorrectly formatted for conversion to type T" << endl;
      exit(1);
    }
    
    return( ret );
  }

  vector<size_t> range(const size_t n);

  vector<vector<size_t> > splitRange(const size_t nElements, const size_t nSplits);
  
}

#endif
