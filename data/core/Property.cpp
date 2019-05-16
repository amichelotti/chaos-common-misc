#include "Property.h"


namespace common  {
namespace misc {
namespace data {
bool Property::hasKey(const std::string& key){
    return props.hasKey(key);
  }
    void Property::createProperty(const std::string&propname,ChaosUniquePtr<chaos::common::data::CDataWrapper> value,const std::string&pubname,conversion_func_t abstractWriter,conversion_func_t privateWriter){
            if(value.get()){
                createProperty(propname,*value.get(),pubname);
            }
    }
    
    chaos::common::data::CDataWrapper Property::getProperty(){return props;}

    void Property::createProperty(const std::string&propname,chaos::common::data::CDataWrapper& value,const std::string&pubname,conversion_func_t abstractWriter,conversion_func_t privateWriter){
            if(!props.hasKey(propname)){
                if(pubname.size()>0){
                    value.append("pubname",pubname);
                    abstract2props[pubname]=propname;
                    convertAbstract2Prop[pubname]=abstractWriter;
                    convertProp2Abstract[propname]=privateWriter;
  
                }
                
                props.append(propname,abstractWriter!=NULL?abstractWriter(value):value);

            } else {
                
                props.setValue(propname,(const chaos::common::data::CDataWrapper*)&value);
            }
            

    }
ChaosUniquePtr<chaos::common::data::CDataWrapper> Property::setProperty(const std::string&propname,ChaosUniquePtr<chaos::common::data::CDataWrapper>& value){
     if(!props.hasKey(propname)){
          std::map<std::string,std::string>::iterator i=abstract2props.find(propname);
          if(i!=abstract2props.end()){
              if(props.hasKey(i->first)){
                 props.setValue(i->first,(const chaos::common::data::CDataWrapper*)value.get());
                return props.getCSDataValue(i->first);  
              }
          }
      } else {
        props.setValue(propname,(const chaos::common::data::CDataWrapper*)value.get());
        return props.getCSDataValue(propname);
      }
      ChaosUniquePtr<chaos::common::data::CDataWrapper> ret;
    return ret;
    }
    
    
}
}
}