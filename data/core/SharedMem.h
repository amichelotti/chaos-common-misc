
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
  std::auto_ptr<bip::shared_memory_object> shared_obj;
  std::auto_ptr<bip::mapped_region> region;
  std::auto_ptr<bip::named_mutex> mx;
  std::auto_ptr<bip::named_condition> cv;
  bool server;
  size_t msize;
  std::string _name;
 public:
 /***
  * If size >0 the create a new shared mem, otherwise expects created.
 */
  SharedMem(const std::string& name,size_t size=0);
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

  std::string getName(){return _name;}

  int writeMsg(void*ptr,size_t size);
  std::auto_ptr<uint8_t> readMsg();

  int write(const char*src,size_t size);
  int read( char*dst,size_t size);

};
}  // namespace data
}  // namespace misc
}  // namespace common