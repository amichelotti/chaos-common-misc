/* 
 * File:   WaveBase.h
 * Author: michelo
 *
 * Created on November 25, 2015, 8:44 AM
 */

#ifndef WAVEBASE_H
#define	WAVEBASE_H
#include <string>
#include <stdint.h>
namespace common {
    namespace misc {
        namespace wavegenerators {

            class WaveBase {
            public:
                WaveBase();
                WaveBase(const WaveBase& orig);
                virtual ~WaveBase();
                /**
                 * Select the wave generator and give parameters
                 * @param par initialization string for a wave generator
                 * @return 0 on success 
                 */
                virtual int init(const std::string& par) = 0;
                /**
                 * Deinitialize resources if any
                 */
                virtual void deinit() = 0;
                /**
                 * reset the wave generator
                 */
                virtual void reset() = 0;

                /**
                 * Generate a new value from the series
                 * @return the value
                 */
                virtual double generate() = 0;

                /**
                 * Set the time of the simulation
                 * @param t the time of the simulation
                 */
                virtual void setTime(uint64_t t);
                /**
                 * get time 
                 * @return the time of the simulation
                 */
                uint64_t getTime();
                /**
                 * Return the parameters and info for the generator
                 * @return 
                 */
                virtual std::string infoParams() = 0;
            protected:
                uint64_t tim;

            };
        }
    }
}
#endif	/* WAVEBASE_H */

