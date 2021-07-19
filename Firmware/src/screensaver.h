/*
 * screensaver.h
 *
 *  Created on: Jul 19, 2021
 *      Author: Pavel
 */

#ifndef SCREENSAVER_H_
#define SCREENSAVER_H_

extern uint8_t xdata oledbuff[];
void initSSaver(void);
void fillSSaverBuffer(void);
void fillSSaverOled(void);

#endif /* SCREENSAVER_H_ */
