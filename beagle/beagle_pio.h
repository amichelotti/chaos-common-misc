#ifndef __BEAGLE_PIO_H__
#define __BEAGLE_PIO_H__
/*
 *	beagle_pio
 *	!CHAOS
 *	Created by Andrea Michelotti.
 *
 *    	Copyright 2013 INFN, National Institute of Nuclear Physics
 *
 *    	Licensed under the Apache License, Version 2.0 (the "License");
 *    	you may not use this file except in compliance with the License.
 *    	You may obtain a copy of the License at
 *
 *    	http://www.apache.org/licenses/LICENSE-2.0
 *
 *    	Unless required by applicable law or agreed to in writing, software
 *    	distributed under the License is distributed on an "AS IS" BASIS,
 *    	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    	See the License for the specific language governing permissions and
 *    	limitations under the License.
 */
#ifdef __cplusplus
extern "C" {
#endif
/**
   \brief configure a pio
   \param pion the pio number
   \param dir 0 is input, 1 is output
   \return 0 if success
*/
int beagle_pio_configure(int pion,int dir);
/**
   \brief set a value of an output pio
   \param pion the pio number
   \param dir 0 is input, 1 is output
   \return 0 if success
*/
int beagle_pio_set(int pion,int val);
/**
   \brief get a value of an input pio
   \param pion the pio number
   \param in val is returned the value 
   \return 0 if success
*/
int beagle_pio_get(int pion,int *val);
#ifdef __cplusplus
}
#endif

#endif
