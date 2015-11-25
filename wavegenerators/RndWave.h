/* 
 * File:   RndWave.h
 * Author: michelo
 *
 * Created on November 25, 2015, 9:21 AM
 */

#ifndef RNDWAVE_H
#define	RNDWAVE_H
#include "WaveBase.h"
namespace common {

    namespace misc {
        namespace wavegenerators {

            class RndWave :public WaveBase {
            public:
                RndWave();
                RndWave(const RndWave& orig);
                virtual ~RndWave();


                int init(const std::string& par);

                void deinit();

                void reset();


                double generate();

                std::string infoParams();

            private:
                double max, min;
            };
        }
    }
}
#endif	/* RNDWAVE_H */

