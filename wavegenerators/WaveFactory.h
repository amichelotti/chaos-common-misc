/* 
 * File:   WaveFactory.h
 * Author: michelo
 *
 * Created on November 25, 2015, 12:05 PM
 */

#ifndef WAVEFACTORY_H
#define	WAVEFACTORY_H
#include "WaveBase.h"
#include <boost/shared_ptr.hpp>
namespace common {

    namespace misc {
        namespace wavegenerators {
            
            typedef  boost::shared_ptr<WaveBase> WaveBase_t;
class WaveFactory {
public:
    WaveFactory();
    WaveFactory(const WaveFactory& orig);
    virtual ~WaveFactory();
    
    static WaveBase_t getGenerator(const std::string& gen, const std::string& params); 
private:

};
        
        }
    }
}
#endif	/* WAVEFACTORY_H */

