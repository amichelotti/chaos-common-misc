//
//  AbstractChannel.h
//  Abstraction of a generic channel
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
     namespace driver {
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

            /**
             reads (synchronous) nb bytes from channel
             @param buffer destination buffer
             @param nb number of bytes to read
             @param timeo milliseconds of timeouts, 0= no timeout
	     @param timeout_arised returns if a timeout has been triggered
             @return number of bytes read, negative on error 
             */
            virtual int read(void *buffer,int nb,int ms_timeo=0,int*timeout_arised=0)=0;
            
            /**
             reads (asynchronous) nb bytes from channel
             @param buffer destination buffer
             @param nb number of bytes to read
             @return number of bytes read, negative on error
             */
            virtual int read_async(void *buffer,int nb)=0;
            
            /**
             in asynchronous mode returns the number of bytes available for read_async
             @return number of bytes available, negative on error (buffer overflow)
             */
            virtual int byte_available_read()=0;

            /**
             writes (synchronous) nb bytes to channel
             @param buffer source buffer
             @param nb number of bytes to write
             @param timeo milliseconds of timeouts, 0= no timeout
	     @param timeout_arised returns if a timeout has been triggered
             @return number of bytes sucessfully written, negative on error
             */
            virtual int write(void *buffer,int nb,int ms_timeo=0,int*timeout_arised=0)=0;
            
            /**
             writes (asynchronous) nb bytes to channel
             @param buffer source buffer
             @param nb number of bytes to write
             @return number of bytes sucessfully written, negative on error or timeout
             */
            virtual int write_async(void *buffer,int nb)=0;

            
            /**
             in asynchronous mode returns the number of bytes to write into the channel
             @return number of bytes available, negative on error (buffer overflow)
             */
            virtual int byte_available_write()=0;
            
            /**
             flush bytes in the write buffer
             */
            virtual void flush_write()=0;
            
            /**
             flush bytes in the read buffer
             */
            virtual void flush_read()=0;



        };

        typedef boost::shared_ptr<AbstractChannel> AbstractChannel_psh;
    };
};
};


#endif