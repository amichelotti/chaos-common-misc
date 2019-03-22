#include "Property.h"


namespace common  {
namespace misc {
namespace data {

    void Property::createProperty(const std::string&propname,ChaosUniquePtr<chaos::common::data::CDataWrapper> value,const std::string&pubname=""){
            if(value.get()){
                createProperty(propname,*value.get(),pubname);
            }
    }
    
    chaos::common::data::CDataWrapper Property::getProperty(){return props;}

    void Property::createProperty(const std::string&propname,chaos::common::data::CDataWrapper& value,const std::string&pubname=""){
            if(!props.hasKey(propname)){
                if(pubname.size()>0){
                    value.append("pubname",pubname);
                    public2props[pubname]=propname;
                }
                props.append(propname,value);

            } else {
                props.setValue(propname,value);
            }
            

    }
ChaosUniquePtr<chaos::common::data::CDataWrapper> Property::setProperty(const std::string&propname,ChaosUniquePtr<chaos::common::data::CDataWrapper> value){
     if(!props.hasKey(propname)){
          std::map<std::string,std::string>::iterator i=public2props.find(propname);
          if(i!=public2props.end()){
              if(props.hasKey(*i)){
                 props.setValue(*i,value->getValue("value"));
                return props.getCSDataValue(*i);  
              }
          }
      } else {
        props.setValue(propname,value->getValue("value"));
        return props.getCSDataValue(propname);
      }
      
    }
    
    
}
}
}