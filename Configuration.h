#ifndef Configuration_h
#define Configuration_h

#include <algorithm>
//C++17  #include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

class Configuration {
	
  using str  = std::string;	
  using cstr = const str;
  using vstr = std::vector<str>;

  str  file;
//C++17  path  file;
  str  var, val;
  char comment;
  
  std::map<str,std::vector<str>> values;
 
 public:
 
  explicit Configuration( cstr& f, cstr& vr = ":", cstr& vl = ":", const char & endof = '\n' ) : 
   file(f), var(vr), val("("), comment{'#'} {
  
   val += vl + "+)";
   std::ifstream input( file );

   if( input.is_open() ) {
    str line;
    while( getline(input,line,endof) ) { 
	 line.erase( std::remove_if( line.begin(), line.end(), ::isspace ), line.end() );
     if( line.empty() || line[0] == comment ) continue;
     
     auto pos  = line.find(var);
	 auto name = line.substr(0,pos), vals = line.substr(pos+1);
     std::regex re( val ); 
     std::sregex_token_iterator it{ vals.begin(),vals.end(),re,-1 }, reg_end;
     vstr vals_v; for(; it != reg_end; ++it) { vals_v.emplace_back( it->str() ); }
     values[name] = vals_v;
    }
    input.close();
   } else {
    std::cerr << "Couldn't open config file for reading." << std::endl;
   }
  }
  
  float       getF( cstr& key )  const { return stof( values.at(key).at(0) ); }
  str         get( cstr& key )   const { return values.at(key).at(0); }
  vstr        getV( cstr& key )  const { return values.at(key); }
  size_t      getN( cstr& key )  const { return getV(key).size(); }
  
  void        print( cstr& key = "", cstr & option = "", std::ostream & o = std::cout ) const {
   //std::ostream & o = std::cout;
   if( option o != "noname" ) << file << std::endl;
   if( key == "" ) { 
	   for( auto i : values ) print( i.first,"noname" );
   } else { 
	   vstr v = getV(key); o << key << ": "; 
	   for( auto i : v ) o << i << ( (i == v.back()) ? "" : ", " ); 
	   o << std::endl; 
   }
  }
  void        reset() { file = var = val = ""; values.clear(); }
  
};

#endif


