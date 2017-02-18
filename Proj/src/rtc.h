#ifndef RTC_H
#define RTC_H

/** @defgroup RTC RTC
 * @{
 */

/**
 * @brief Checks if the rtc is updating, verifying the UIP in Register A.
 * @return 1 if the rtc is updating, 0 if it's not
 */
int isRTCUpdating();

/**
 * @brief Checks if the values given by the RTC are in BCD, verifying the DM in Register B.
 * @return 1 if the values are in bcd, 0 if they are in binary
 */
int isBCD();

/**
 * @brief Converts values from bcd to binary.
 * @param bcd value in bcd to be converted in binary
 * @return the value in binary
 */
unsigned long BCDtoBinary(unsigned long *bcd);

/**
 * @brief Gets the current date from the RTC in binary.
 * @param day will be updated by the RTC
 * @param month will be updated by the RTC
 * @param year will be updated by the RTC
 */
void getDate(unsigned long *day, unsigned long *month, unsigned long *year);

/**
 * @brief Gets the current hour from the RTC in binary.
 * @param hour will be updated by the RTC
 * @param minutes will be updated by the RTC
 * @param seconds will be updated by the RTC
 */
void getHour(unsigned long *hour, unsigned long *minutes,
		unsigned long *seconds);

/** @} end of RTC */

#endif /* RTC_H */
