#include "SharedMem.h"


namespace common  {
namespace misc {
namespace data {


int SharedMem::resize(size_t size){
    shared_obj.truncate(size);
    region.reset(new bip::mapped_region(shared_obj, bip::read_write));
    msize=region->get_size();
    return msize;
}
void SharedMem::notify_all(){
    bip::scoped_lock<bip::named_mutex> lk(mx);
    cv.notify_all();
}
void SharedMem::wait(){
    bip::scoped_lock<bip::named_mutex> lk(mx);
    cv.wait(lk);
}
int SharedMem::write(const char*src,size_t size){
    bip::scoped_lock<bip::named_mutex> lk(mx);
    size_t siz=0;
    if(region->get_address()){
        siz=std::min(region->get_size(),size);
        memcpy(region->get_address(),src,siz);
    }
    return siz;
}
int SharedMem::read( char*dst,size_t size){
    bip::scoped_lock<bip::named_mutex> lk(mx);
    size_t siz=0;
    if(region->get_address()&&dst){
        siz=std::min(region->get_size(),size);
        memcpy(dst,region->get_address(),siz);
    }
    return siz;
    
}
void *SharedMem::getMem(){
    return region->get_address();
}
size_t SharedMem::getSize(){
    return region->get_size();
}
int SharedMem::writeMsg(void*ptr,size_t size){
    bip::scoped_lock<bip::named_mutex> lk(mx);
    uint32_t *p=(uint32_t*)region->get_address();
    if(p && (region->get_size()<size)){
        *p++=size;
        memcpy(p,ptr,size);
        return size;
    }
    return 0;

}
  std::auto_ptr<uint8_t> SharedMem::readMsg(){
        std::auto_ptr<uint8_t> ret;
        bip::scoped_lock<bip::named_mutex> lk(mx);
        uint32_t *p=(uint32_t*)region->get_address();

        if(p && (p[0]<region->get_size())){
            ret.reset(new uint8_t[p[0]]);
            memcpy(ret.get(),&p[1],p[0]);
            
        }
        return ret;
  }

    
}
}
}