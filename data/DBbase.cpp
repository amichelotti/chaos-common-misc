/*
 * DBbase.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#include "DBbase.h"

namespace common {
namespace misc {
namespace data {


std::ostream& operator<<(std::ostream& out,blobRecord_t& ret){

	 out<<"{keys:[";
	 for(int cnt=0;cnt<ret.size();cnt++){
		out<<"\""<<ret[cnt].key<<"\"";
		if((cnt+1)<ret.size())
			  out<<",";
	 }
  out<<"]}";
  return out;
}

} /* namespace data */
} /* namespace misc */
} /* namespace common */



