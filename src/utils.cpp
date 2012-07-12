#include "utils.hpp"

#include <algorithm>
#include <ctime>
#include <ios>
#include <cassert>

int utils::generateSeed() {
  
  time_t now;
  time(&now);
  return( clock() + now );
  
}

// Removes all newline and any trailing characters
string utils::chomp(const string& str, const string& nl) {
  
  size_t endStr = str.find_first_of(nl);
  return( str.substr(0, endStr) );

}

// Remove all leading and trailing whitespaces
string utils::trim(const string& str, const string& wh) {
  
  size_t beginStr = str.find_first_not_of(wh);
  if ( beginStr == string::npos ) {
    // no content
    return("");
  }

  size_t endStr = str.find_last_not_of(wh);
  size_t range = endStr - beginStr + 1;
  
  return( str.substr(beginStr, range) );
}

map<string,string> utils::parse(istream& streamObj,
				const char delimiter,
				const char separator,
				const char comment) {

  map<string,string> ret;

  string key;

  while ( !streamObj.eof() ) {

    // Parse the key
    getline(streamObj,key,separator);
    assert( streamObj.good() );
    assert( !streamObj.eof() );
    
    // Peek the next characeter and check if it's a comment 
    if ( streamObj.peek() == comment ) {

      // ignore the comment character...
      streamObj.ignore();

      // ... and get the value for the key
      getline(streamObj,ret[key],comment);

      assert( ret.find(key) != ret.end() );

      // If the next character is a delimiter, ignore it
      if ( streamObj.peek() == delimiter ) {
	streamObj.ignore();
      } 

    } else {
      
      // Without the comment character we just read until the next delimiter
      getline(streamObj,ret[key],delimiter);
      
    }
 
  }
  
  // The possible carriage return and end-of-line characters need to be removed
  ret[key] = utils::chomp(ret[key]);
    
  return( ret );

}

vector<string> utils::split(istream& streamObj, const char delimiter, const string& wh) {

  string newItem("");
  vector<string> items;

  while ( getline(streamObj,newItem,delimiter) ) {
    newItem = utils::trim(newItem,wh);
    items.push_back(newItem);
  }

  return( items );

}

vector<string> utils::readListFromFile(const string& fileName, const char delimiter) {
  
  ifstream streamObj( fileName.c_str() );
  assert(streamObj.good());
  
  return( utils::split(streamObj,delimiter) );
}

vector<size_t> utils::range(const size_t n) {
  
  vector<size_t> ics(n);
  
  for(size_t i = 0; i < n; ++i) {
    ics[i] = i;
  }

  return( ics );

}

vector<vector<size_t> > utils::splitRange(const size_t nElements, const size_t nSplits) {

  vector<size_t> ics = utils::range(nElements);

  assert( nSplits >= 1 );

  vector<vector<size_t> > splits(nSplits);

  size_t splitSize = ics.size() / nSplits ;

  size_t remainder = ics.size() % nSplits;

  size_t startIdx = 0;

  for ( size_t splitIdx = 0; splitIdx < nSplits; ++splitIdx ) {

    size_t stopIdx = startIdx + splitSize;

    if ( splitIdx < remainder ) stopIdx++;

    splits[splitIdx].resize(stopIdx-startIdx);

    copy(ics.begin()+startIdx,ics.begin()+stopIdx,splits[splitIdx].begin());

    startIdx = stopIdx;

  }

  return( splits );

}




