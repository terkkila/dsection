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

  // Generate seeds for random number generators
  int generateSeed();

  template<typename StartIterator, typename StopIterator>
  inline void print(StartIterator startIt, StopIterator stopIt, const char spacer = ' ') {

    if ( startIt != stopIt ) {
      cout << *startIt;
      ++startIt;
    }

    while ( startIt != stopIt ) {
      cout << spacer << *startIt;
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

  // Reads a list of items from a file
  vector<string> readListFromFile(const string& fileName, const char delimiter);

  // Write to file
  template <typename InputIterator>
  void printToFile(InputIterator begin, InputIterator end, const char delimiter, const string& fileName) {
    
    ofstream toFile(fileName.c_str());

    if ( begin != end ) {
      toFile << *begin;
      ++begin;
    }

    while ( begin != end ) {
      toFile << delimiter << *begin;
      ++begin;
    }

    toFile.close();

  }

  template <typename InputIterator>
  string join(InputIterator begin, InputIterator end, const char delimiter) {

    string ret("");
    
    if ( begin == end ) {
      return(ret);
    }

    stringstream ss;
    ss << *begin;
    ++begin;
    
    while( begin != end ) {
      
      ss << delimiter << *begin;

      ++begin;

    }

    ss >> ret;
    return( ret );

  }

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
