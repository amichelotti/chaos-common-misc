#include "CacheString.h"
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace common
{
namespace misc
{
namespace data
{
CacheString::CacheString(const uint32_t size) : cache_size(size),hit(0),miss(0),query(0){
    oldest=0;
}
uint64_t CacheString::getTimestamp(){
    boost::posix_time::ptime current_date_microseconds = boost::posix_time::microsec_clock::local_time();

    return current_date_microseconds.time_of_day().total_microseconds();

}
int CacheString::fetch(std::string &key){
    uint64_t now=getTimestamp();
    boost::uuids::string_generator str_gen;
    boost::uuids::uuid uid = str_gen(key);
    boost::mutex::scoped_lock l(lock);

    std::map<std::string,cache_ele_t>::iterator i=m_cache.find(boost::uuids::to_string(uid));
    query++;
    if(i!=m_cache.end()){
        
        if((now-i->second.ts)>now-i->second.ttl){
            
            m_cache.erase(i);
            return 1;
        } else {
            key=i->second.data;
            hit++;
            return 0;
        }
    }
    miss++;
    return -1;
}

int CacheString::watch(const std::string &key){
    uint64_t now=getTimestamp();

    boost::uuids::string_generator str_gen;
    boost::uuids::uuid uid = str_gen(key);
    std::map<std::string,cache_ele_t>::iterator i=m_cache.find(boost::uuids::to_string(uid));

     if(i!=m_cache.end()){
        if(now-i->second.ts>now-i->second.ttl){
            return 1;
        } else {
            return 0;
        }
    }
    return -1;
}
void CacheString::clean(){
    uint64_t now=getTimestamp();
    uint64_t limit=(now-oldest)/2;
    oldest=limit;
    std::map<std::string,cache_ele_t>::iterator i;
    boost::mutex::scoped_lock l(lock);

    for(i=m_cache.begin();i!=m_cache.end();){
        if(i->second.ts<limit){
            m_cache.erase(i++);
        } else {
            ++i;
        }
    }


}

int CacheString::write(const std::string &key, uint32_t ttl ){
    boost::uuids::string_generator str_gen;
    boost::uuids::uuid uid = str_gen(key);
    
    if((m_cache.size()>=cache_size)){
      clean();
    }
 
    uint64_t now=getTimestamp();
    if(oldest==0){
        oldest=now;
    }
    cache_ele_t ele;
    ele.data=key;
    ele.ts=now;
    ele.ttl=ttl;
    boost::mutex::scoped_lock l(lock);
    m_cache[boost::uuids::to_string(uid)]=ele;
    return m_cache.size();
}
void CacheString::setMaxCache(uint32_t max_cache){
    cache_size=max_cache;

}
/**
                 * return the number of elements
                */
uint32_t CacheString::size(){
    return m_cache.size();
}
}; // namespace data
} // namespace misc
} // namespace common
