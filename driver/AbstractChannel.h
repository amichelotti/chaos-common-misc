//
//  AbstractChannel.h
//  Abstraction of a generic serial channel
//
//  Created by andrea michelotti on 8/3/17.
//  Copyright (c) 2017 andrea michelotti. All rights reserved.
//

#ifndef __AbstractChannel__
#define __AbstractChannel__

#include <iostream>
#include <boost/shared_ptr.hpp>
namespace common {
  namespace misc {
  namespace driver{
        typedef enum {
            CHANNEL_CANNOT_OPEN =-100,
            CHANNEL_CANNOT_ALLOCATE_RESOURCES,
            CHANNEL_TIMEOUT,
            CHANNEL_READ_ERROR,
            CHANNEL_WRITE_ERROR

        } channel_error_t;
        
        class AbstractChannel {

        	const std::string uid;
        public:
        	AbstractChannel(){};
        	AbstractChannel(const std::string& uid_):uid(uid_){};

        	const std::string& getUid() const {return uid;}
            /**
             initialises resource and channel
             @return 0 on success
             */
            virtual int init()=0;
            
            /**
             deinitalises resources and channel
             @return 0 on success
             */
            virtual int deinit()=0;
        };

        typedef boost::shared_ptr<AbstractChannel> AbstractChannel_psh;
    };

};

};
#endif
