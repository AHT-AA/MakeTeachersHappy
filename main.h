/*
	Name: main.h
	Copyright: All Rights Free.
	Author: AHT-AA
	Created date: 13.10.2022 20.49
	Modify date: 13.10.2022 20.49
	Description: Make teachers heppy!..
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#define RegProgramName "MakeTeachersHappy"

#define SYLLABUS_FILE_NAME "syllabus.txt"
#define BLACKLIST_FILE_NAME "blacklist.txt" /* Hocam bak seni kara listeye aliriz ona gore ayagini denk al(!) */

#define WEEKDAYS_NUMBER 5 /* 5 days of week is weekday :(( */
#define LESSON_NUMBER 8 /* 8 lesson per day :((((( */
#define LESSON_TIME 40 /* 40 minute per lesson :| */
#define MAX_LESSON_NAME_LENGTH 32

#define END_OF_DAY 16.00

#define SPAWN_NUMBER_OF_ERRORBOX 3
#define SPAWN_RATE (60*3) /* 3 minute */

enum DayofWeek
{
	MON,
	TUE,
	WEN,
	THU,
	FRI,
	SAT,
	SUN
};

BOOL IsLessonInBlackList(LPSTR lpszLessonName);
LPSTR WhichLessonAreWeIn(VOID);

#endif /* _MAIN_H_ */
