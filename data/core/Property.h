#include <chaos/common/data/CDataWrapper.h>

namespace common {
namespace misc {
namespace data {
#define FILLPROPERTYCD(cw, value, min, max, incr) \
  cw->append("value", value);                     \
  cw->append("min", min);                         \
  cw->append("max", max);                         \
  cw->append("incr", incr);
/**
 * Class to store properties/variables that can be abstracted and private
 * typic example is in the use of camera properties that has to be normalized for an abstracted use
*/
class Property {
  chaos::common::data::CDataWrapper props;
  typedef void(*conversion_func_t)(void) ;
  std::map<std::string, std::string> abstract2props;

 public:

  chaos::common::data::CDataWrapper getProperty();
  // between public and private properties 
  void createProperty(const std::string& propname,
                      ChaosUniquePtr<chaos::common::data::CDataWrapper> value,
                      const std::string& abstractname = "",conversion_func_t abstractWriter=NULL,conversion_func_t privateWriter=NULL);
  void createProperty(const std::string& propname,
                      chaos::common::data::CDataWrapper& value,
                      const std::string&  abstractname = "",conversion_func_t abstractWriter=NULL,conversion_func_t privateWriter=NULL);
  ChaosUniquePtr<chaos::common::data::CDataWrapper> setProperty(
      const std::string& propname,
      ChaosUniquePtr<chaos::common::data::CDataWrapper> value);
  template <typename T>
  chaos::common::data::CDataWrapper createProperty(
      const std::string& propname, T value, const std::string& pubname = "") {
    chaos::common::data::CDataWrapper p;
    p.append("value", value);

    if (!props.hasKey(propname)) {
      if (pubname.size() > 0) {
        p.append("pubname", pubname);
        abstract2props[pubname] = propname;
      }
      props.append(propname, p);

    } else {
      props.setValue(propname, p);
    }

    return p;
  }
  template <typename T>
  chaos::common::data::CDataWrapper createProperty(
      const std::string& propname, T value, T min, T max, T incr,
      const std::string& pubname = "") {
    chaos::common::data::CDataWrapper p;
    p.append("value", value);
    p.append("min", min);
    p.append("max", max);
    p.append("incr", incr);

    if (!props.hasKey(propname)) {
      if (pubname.size() > 0) {
        abstract2props[pubname] = propname;

        p.append("pubname", pubname);
      }
      props.append(propname, p);

    } else {
      props.setValue(propname.c_str(), p);
    }

    return p;
  }

  template <typename T>
  ChaosUniquePtr<chaos::common::data::CDataWrapper> setProperty(
      const std::string& propname, const T& value) {
    chaos::common::data::CDataWrapper p;
    p.append("value", value);
    if (!props.hasKey(propname)) {
      std::map<std::string, std::string>::iterator i =
          abstract2props.find(propname);
      if (i != abstract2props.end()) {
        if (props.hasKey(*i)) {
          props.setValue(*i, p);
          return props.getCSDataValue(*i);
        }
      }
    } else {
      props.setValue(propname, p);
      return props.getCSDataValue(propname);
    }

    ChaosUniquePtr<chaos::common::data::CDataWrapper> ret;
    return ret;
  }
  template <typename T>
  int getPropertyValue(const std::string& propname, T& value) {
    if ((props.hasKey(propname)) && (props.isCDataWrapperValue(propname))) {
      value = props.getCSDataValue(propname)->getValue("value");
      return 0;
    }
    std::map<std::string, std::string>::iterator i =
        abstract2props.find(propname);
    if (i != abstract2props.end()) {
              if(props.hasKey(*i)&&(props.isCDataWrapperValue(*i) ){
        value = props.getCSDataValue(*i)->getValue("value");
        return 0;  
              }
    }

    return -1;
  }
};
}  // namespace data
}  // namespace misc
}  // namespace common