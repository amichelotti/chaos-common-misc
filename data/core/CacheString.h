#ifndef __CACHESTRING__
#define __CACHESTRING__
#include <set>
#include <map>

#include <boost/thread/thread.hpp>


namespace common{
    namespace misc{
        namespace data{
            class CacheString {
                typedef struct cache_ele{
                    std::string data;
                    uint64_t ts;
                    uint32_t ttl; // time to live 0 means no live no cache 
                } cache_ele_t;
                boost::mutex lock;
                std::map<std::string,cache_ele_t> m_cache;
                uint32_t cache_size;
                uint32_t hit,miss,expired,query;
                uint64_t getTimestamp();
                uint64_t oldest;
                void clean();
                public:
                CacheString(const uint32_t size=100);
                /***
                 * Return the cache string corresponding to given key, wait if data not present or if ttlv expired
                 * @return 0, if found, 1 if expired, other if not found
                */
                int fetch(std::string&key,std::string&d);
                /***
                 * Cache key status
                 * @return 0, if found, 1 if expired, other if not present
                */
                int watch(const std::string&src);
                /**
                 * return the number of element in cache
                */
                int write(const std::string&src,const std::string&d,uint32_t ttl=0);
                /**
                 * Set max element size
                */
                void setMaxCache(uint32_t max_cache);
                std::string cacheStats();
                /**
                 * return the number of elements
                */
                uint32_t size();

            };
        }
    }
}
#endif