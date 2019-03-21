#include <chaos/common/data/CDataWrapper.h>

namespace common  {
namespace misc {
namespace data {
    class Property {
        chaos::common::data::CDataWrapper props;
        std::map<std::string,std::string> public2props;

        public:
        chaos::common::data::CDataWrapper getProperty(){return props;}
        void createProperty(const std::string&propname,chaos::common::data::CDataWrapper& value,const std::string&pubname=""){
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
        template< typename T>
        chaos::common::data::CDataWrapper createProperty(const std::string&propname,T value,const std::string&pubname=""){
            chaos::common::data::CDataWrapper p;
            p.append("value",value);
            
            if(!props.hasKey(propname)){
                if(pubname.size()>0){
                    p.append("pubname",pubname);
                    public2props[pubname]=propname;
                }
                props.append(propname,p);

            } else {
                props.setValue(propname,p);
            }
            

            return p;
        }
     template< typename T>
        chaos::common::data::CDataWrapper createProperty(const std::string&propname,T value,T min,T max,T incr,const std::string&pubname=""){
            chaos::common::data::CDataWrapper p;
            p.append("value",value);
            p.append("min",min);
            p.append("max",max);
            p.append("incr",incr);
            
            if(!props.hasKey(propname)){
                if(pubname.size()>0){
                 public2props[pubname]=propname;

                    p.append("pubname",pubname);
                }
                props.append(propname,p);

            } else {
                props.setValue(propname,p);
            }
            

            return p;
        }
    template< typename T>
    ChaosUniquePtr<chaos::common::data::CDataWrapper> setProperty(const std::string&propname,T value){
      chaos::common::data::CDataWrapper p;
      p.append("value",value);
      if(!props.hasKey(propname)){
          std::map<std::string,std::string>::iterator i=public2props.find(propname);
          if(i!=public2props.end()){
              if(props.hasKey(*i)){
                 props.setValue(*i,p);
                return props.getCSDataValue(*i);  
              }
          }
      } else {
        props.setValue(propname,p);
        return props.getCSDataValue(propname);
      }
      
      ChaosUniquePtr<chaos::common::data::CDataWrapper> ret;  
      return ret;
    }
template< typename T>
    int getPropertyValue(const std::string&propname,T& value){
      
    if((props.hasKey(propname))&&(props.isCDataWrapperValue(propname))){
        value=props.getCSDataValue(propname)->getValue("value");
        return 0;
     }
     std::map<std::string,std::string>::iterator i=public2props.find(propname);
     if(i!=public2props.end()){
              if(props.hasKey(*i)&&(props.isCDataWrapperValue(*i) ){
                    value=props.getCSDataValue(*i)->getValue("value");
                return 0;  
              }
          }
     
      return -1;
    

    }
};
}
}
}