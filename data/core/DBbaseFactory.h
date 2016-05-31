/*
 * DBbaseFactory.h
 *
 *  Created on: Feb 23, 2016
 *      Author: michelo
 */

#ifndef DATA_CORE_DBBASEFACTORY_H_
#define DATA_CORE_DBBASEFACTORY_H_
#include <vector>
#include <string>
#include <map>
#include <common/misc/data/core/DBbase.h>
#include <boost/thread/mutex.hpp>

namespace common {
namespace misc {
namespace data {
class DBbaseFactory {

private:

	static std::map< std::string,DBbase* > instances;

	static boost::mutex mutex;

	public:
	static DBbase* getInstance(const std::string&type,const std::string& name );
};


} /* namespace data */
} /* namespace misc */
} /* namespace common */

#endif /* DATA_CORE_DBBASE_H_ */
