#include "SharedMem.h"
#include <boost/algorithm/string/replace.hpp>
#include <boost/thread/thread_time.hpp>


namespace common  {
namespace misc {
namespace data {

SharedMem::SharedMem(const std::string& name,size_t siz):msize(0){
    _name=name+"_mem";
    boost::replace_all(_name,"/","_");
    std::string mtxname=_name+"_mtx";
    std::string cvname=_name+"_cv";
    server=(siz>0);
   // bip::shared_memory_object::remove(shname.c_str());
   
   if(siz>0){
       try{
       bip::shared_memory_object::remove(_name.c_str());

   } catch(...){

   }
        shared_obj.reset(new bip::shared_memory_object(bip::create_only,_name.c_str(),bip::read_write));
        shared_obj->truncate(siz);
        msize=siz;
   } else {
        shared_obj.reset(new bip::shared_memory_object(bip::open_only,_name.c_str(),bip::read_write));

   }
    mx.reset(new bip::named_mutex(bip::open_or_create,mtxname.c_str()));
    cv.reset(new bip::named_condition(bip::open_or_create,cvname.c_str()));
    region.reset(new bip::mapped_region(*shared_obj.get(), bip::read_write));

}
int SharedMem::resize(size_t size){
    if(server){
        shared_obj->truncate(size);
        region.reset(new bip::mapped_region(*shared_obj.get(), bip::read_write));
        msize=region->get_size();
    }
    return msize;
}
void SharedMem::notify_all(){
    bip::scoped_lock<bip::named_mutex> lk(*mx.get());
    cv->notify_all();
}
int SharedMem::wait(uint32_t timeoms){
    bip::scoped_lock<bip::named_mutex> lk(*mx.get());
    
    if(timeoms>0){
        boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(timeoms);

        if(cv->timed_wait(lk,timeout)==false){
            //timeout
            return -100;
        }
    } else {
        cv->wait(lk);
    }
    return 0;
        
}
int SharedMem::write(const char*src,size_t size){
    bip::scoped_lock<bip::named_mutex> lk(*mx.get());
    size_t siz=0;
    if(region->get_address()){
        siz=std::min(region->get_size(),size);
        memcpy(region->get_address(),src,siz);
    }
    return siz;
}
int SharedMem::read( char*dst,size_t size){
    bip::scoped_lock<bip::named_mutex> lk(*mx.get());
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
    bip::scoped_lock<bip::named_mutex> lk(*mx.get());
    uint32_t *p=(uint32_t*)region->get_address();
    if(p && (size<(region->get_size()-sizeof(uint32_t)))){
        *p++=size;
        memcpy(p,ptr,size);
        return size;
    }
    return 0;

}
  std::auto_ptr<uint8_t> SharedMem::readMsg(){
        std::auto_ptr<uint8_t> ret;
        bip::scoped_lock<bip::named_mutex> lk(*mx.get());
        uint32_t *p=(uint32_t*)region->get_address();

        if(p && (p[0]<region->get_size())){
            ret.reset(new uint8_t[p[0]]);
            memcpy(ret.get(),&p[1],p[0]);
            
        }
        return ret;
  }
  size_t SharedMem::readMsg(uint8_t**ptr){
      size_t ret=0;
    if(ptr){
        *ptr=NULL;
    }
    bip::scoped_lock<bip::named_mutex> lk(*mx.get());
        uint32_t *p=(uint32_t*)region->get_address();
        if(p && (p[0]<region->get_size())){
            ret=p[0];
            if(ptr){
                *ptr=(uint8_t*)&p[1];
            }
        }
    return ret;
  }

    
}
}
}