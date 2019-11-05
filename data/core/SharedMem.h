
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
namespace common {
namespace misc {
namespace data {
namespace bip   = boost::interprocess;


class SharedMem {
  bip::shared_memory_object shared_obj;
  std::auto_ptr<bip::mapped_region> region;
  bip::named_mutex mx;
  bip::named_condition cv;
  size_t msize;
 public:
  SharedMem(const std::string& name):shared_obj(bip::open_or_create,(name+"_mem").c_str(), bip::read_write),mx(bip::open_or_create,(name+"_mtx").c_str()),cv(bip::open_or_create,(name+"_mtx").c_str()),msize(0){}
  /*
    resize shared memory
  */
  int resize(size_t size);
  /**
   * notify all waiters
  */
  void notify_all();
  /**
   * wait on notify
   * 
  */
  void wait();
  /**
   * get mapped address
  */
  void *getMem();
  /***
   * get memory size
   * 
  */
  size_t getSize();

  int writeMsg(void*ptr,size_t size);
  std::auto_ptr<uint8_t> readMsg();

  int write(const char*src,size_t size);
  int read( char*dst,size_t size);

};
}  // namespace data
}  // namespace misc
}  // namespace common