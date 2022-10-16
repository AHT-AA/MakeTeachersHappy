/*
	Name: main.c
	Copyright: All Rights Free.
	Author: AHT-AA
	Created date: 13.10.2022 20.48
	Modify date: 13.10.2022 20.48
	Description: Make teachers happy!..
*/

#include "main.h"

CHAR buff[64];

CHAR lesson_name[WEEKDAYS_NUMBER][LESSON_NUMBER][16] = {'\0'};
CHAR lesson_time[WEEKDAYS_NUMBER][LESSON_NUMBER][8] = {'\0'};
CHAR lesson_in_blacklist[LESSON_NUMBER*WEEKDAYS_NUMBER][MAX_LESSON_NAME_LENGTH] = {'\0'}; /* Lesson_in_BlackList[40][32] */
int lesson_number_of_in_blacklist = 0;

struct tm *tinfo = NULL;

LPSTR GetErrorMessage(DWORD dwLastError)
{
	if(dwLastError == ERROR_SUCCESS)
		dwLastError = GetLastError();
	LPTSTR lpszErr;
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL))
	{
		return lpszErr;
	}
	return NULL;
}

VOID ExitSys(LPCSTR lpszMsg) /* Exit */
{
	DWORD dwLastError = GetLastError();
	LPTSTR lpszErr;
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpszErr, 0, NULL))
	{
		MessageBox(NULL, lpszErr, lpszMsg, MB_OK | MB_ICONEXCLAMATION);
		LocalFree(lpszErr);
	}
	ExitProcess(dwLastError);
}

VOID GetTime(VOID)
{
	long ti = time(NULL);
	if(tinfo != NULL)
		tinfo = NULL;
	tinfo = localtime(&ti);
	tinfo->tm_year+=1900;
	tinfo->tm_mon++;
	if(tinfo->tm_wday == 0) /* SUNDAY */
		tinfo->tm_wday = 7; /* Make sunday last day of week */
	tinfo->tm_wday--; /* Correct index */
}

VOID debug_lessons(VOID)
{
	for(int k = 0; k < WEEKDAYS_NUMBER; k++)
	{
		for(int i = 0; i < LESSON_NUMBER; i++)
			printf("%d. gun,  %d. ders: %s\n", k+1, i+1, lesson_name[k][i]);
		for(int i = 0; i < LESSON_NUMBER; i++)
			printf("%d. gun,  %d. ders baslangic zamani: %s\n", k+1, i+1, lesson_time[k][i]);
	}
	for(int i = 0; i < lesson_number_of_in_blacklist; i++)
		printf("BlackList lesson %d: %s\n", i+1, lesson_in_blacklist[i]);
	printf("Su anik tarih: %02d.%02d.%04d %02d.%02d.%02d, haftanin %d. gunu\n", tinfo->tm_mday, tinfo->tm_mon, tinfo->tm_year, tinfo->tm_hour, tinfo->tm_min, tinfo->tm_sec, tinfo->tm_wday+1);
	if(tinfo->tm_wday == SAT || tinfo->tm_wday == SUN)
		printf("Sevin, bugun okul yok!..\n");
	else
	{
		for(int i = 0; i < LESSON_NUMBER; i++)
			printf("Bugun %d. ders: %s\n", i+1, lesson_name[tinfo->tm_wday][i]);
		for(int i = 0; i < LESSON_NUMBER; i++)
			printf("Bugun %d. ders baslangic zamani: %s\n", i+1, lesson_time[tinfo->tm_wday ][i]);
		for(int i = 0; i < LESSON_NUMBER; i++)
			if(IsLessonInBlackList(lesson_name[tinfo->tm_wday ][i]) == TRUE)
				printf("Bugunku %d. %s dersi kara listede haberin olsun.\n", i+1, lesson_name[tinfo->tm_wday ][i]);
		if(WhichLessonAreWeIn() != NULL)
			printf("Su anki ders %s\n", WhichLessonAreWeIn());
		if(IsLessonInBlackList(WhichLessonAreWeIn()) == TRUE)
			printf("SU ANDA KARA LISTEDE OLAN BIR DERSTESIN!..");
	}
}

VOID GetSyllabus(VOID)
{
	FILE *fsy = fopen(SYLLABUS_FILE_NAME, "r");
	if(fsy == NULL)
	{
		sprintf(buff, "'%s' files can't open", SYLLABUS_FILE_NAME);
		ExitSys(buff);
	}
	for(int k = 0; k < WEEKDAYS_NUMBER; k++) /* Get lesson name and time */
	{
		for(int i = 0; i < LESSON_NUMBER; i++)
			fscanf(fsy, "%s", lesson_name[k][i]);
		for(int i = 0; i < LESSON_NUMBER; i++)
			fscanf(fsy, "%s", lesson_time[k][i]);

		if(feof(fsy) != 0) /* If file is not correct */
		{
			sprintf(buff, "'%s' file is incorrect format.", SYLLABUS_FILE_NAME);
			MessageBox(NULL, buff, "Error", MB_OK | MB_ICONWARNING);
			exit(1);
		}
	}
	fclose(fsy);
}

VOID GetBlackList(VOID)
{
	FILE *fbl = fopen(BLACKLIST_FILE_NAME, "r");
	if(fbl == NULL)
	{
		sprintf(buff, "'%s' files can't open", BLACKLIST_FILE_NAME);
		ExitSys(buff);
	}
	while(feof(fbl) == 0)
	{
		fscanf(fbl, "%s", lesson_in_blacklist[lesson_number_of_in_blacklist++]);
	}
	if(lesson_in_blacklist[lesson_number_of_in_blacklist-1][0] == '\0') lesson_number_of_in_blacklist--; /* If last line is empty don't include it. */
	fclose(fbl);
}

BOOL IsLessonInBlackList(LPSTR lpszLessonName)
{
	if(lpszLessonName == NULL)
		return FALSE;
	for(int i = 0; i < lesson_number_of_in_blacklist; i++)
	{
		if(strcmp(lpszLessonName, lesson_in_blacklist[i]) == 0)
			return TRUE;
	}
	return FALSE;
}

LPSTR WhichLessonAreWeIn(VOID)
{
	GetTime();
	if(tinfo->tm_wday <= FRI) /* WEEKDAYS */
	{
		CHAR buff[64];
		sprintf(buff, "%d.%02d", tinfo->tm_hour, tinfo->tm_min);
		for(int i = LESSON_NUMBER-1; i >= 0; i--)
			if(atof(buff) > atof(lesson_time[tinfo->tm_wday][i]) && atof(buff) < END_OF_DAY)
				return lesson_name[tinfo->tm_wday][i];
	}
	return NULL;
}

VOID SabotageTheLesson(VOID)
{
	static time_t ti = 0;
	time_t foo;
	if(ti == 0)
		ti = time(NULL);
	foo = time(NULL);
	if(foo >= ti + SPAWN_RATE) /* Make terrorism! */
	{
		ti = time(NULL);
		for(int i = 0; i < SPAWN_NUMBER_OF_ERRORBOX; i++)
		{
			MessageBox(NULL, "Bir Sorun Olustu", "Hata", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		}
	}
}

BOOL IsAddedStartup(VOID)
{
	BOOL bFlag = FALSE;
	LONG lRetVals;
	HKEY hKeyStartup;

	if((lRetVals = RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKeyStartup)) != ERROR_SUCCESS) /* Key can't open */
	{
		MessageBox(NULL, "RegOpenKey(HKEY_CURRENT_USER, \"Software\\Microsoft\\Windows\\CurrentVersion\\Run\", &hKeyStartup)", GetErrorMessage(lRetVals), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	LPSTR lpszExePath;
	LPSTR lpszBuff;
	LPBYTE lpszData;
	DWORD dwBuffSize = MAX_PATH;
	DWORD dwDataSize = MAX_PATH;
	DWORD dwType = REG_SZ;

	lpszExePath = (LPSTR)calloc(sizeof(CHAR), MAX_PATH+1);
	lpszBuff = (LPSTR)calloc(sizeof(CHAR), MAX_PATH+1);
	lpszData = (LPBYTE)calloc(sizeof(CHAR), MAX_PATH+1);

	if(lpszExePath == NULL || lpszBuff == NULL || lpszData == NULL)
	{
		ExitSys("Not enought memory.");
	}
	if(GetModuleFileName(NULL, lpszExePath, MAX_PATH+1) == 0)
	{
		ExitSys("Executable path can't get.");
	}

	for(int i = 0; ; i++)
	{
		if((lRetVals = RegEnumValue(hKeyStartup, i, lpszBuff, &dwBuffSize, NULL, &dwType, lpszData, &dwDataSize)) != ERROR_SUCCESS && (lRetVals != ERROR_NO_MORE_ITEMS))
		{
			MessageBox(NULL, "RegEnumValue(hKeyStartup, i, lpszBuff, &dwBuffSize, NULL, &dwType, lpszData, &dwDataSize)", GetErrorMessage(lRetVals), MB_OK | MB_ICONERROR);
			bFlag = FALSE;
			break;
		}
		else if(lRetVals == ERROR_NO_MORE_ITEMS) /* No more data */
		{
			break;
		}
		else
		{
			if(strcmp(lpszBuff, RegProgramName) == 0) /* If reg found */
			{
				if(strcmp((LPSTR)lpszData, lpszExePath) == 0)/* There are reg but is it correct?.. */
				{
					bFlag = TRUE;
					break;
				}
				else /* Not correct path. Delete it. */
				{
					RegDeleteValue(hKeyStartup, lpszBuff);
					bFlag = FALSE;
					break;
				}
			}
		}
	}
	free(lpszBuff);
	free(lpszData);
	free(lpszExePath);
	RegCloseKey(hKeyStartup);
	return bFlag;
}

VOID AddStartup(VOID)
{
	//MessageBox(NULL, "Hmm demek ki baslangicda yokmusum.", "Bilgi", MB_OK | MB_ICONINFORMATION);
	LONG lRetVals;
	HKEY hKeyStartup;

	if((lRetVals = RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKeyStartup)) != ERROR_SUCCESS) /* Key can't open */
	{
		MessageBox(NULL, "RegOpenKey(HKEY_CURRENT_USER, \"Software\\Microsoft\\Windows\\CurrentVersion\\Run\", &hKeyStartup)", GetErrorMessage(lRetVals), MB_OK | MB_ICONERROR);
		return ;
	}

	LPSTR lpszExePath = NULL;

	if((lpszExePath = (LPSTR)calloc(sizeof(CHAR), MAX_PATH+1)) == NULL)
	{
		ExitSys("Not enought memory.");
	}

	if(GetModuleFileName(NULL, lpszExePath, MAX_PATH+1) == 0)
	{
		ExitSys("Executable path can't get.");
	}

	if((lRetVals = RegSetValueEx(hKeyStartup, RegProgramName, 0, REG_SZ, (CONST BYTE*)lpszExePath, strlen(lpszExePath))) != ERROR_SUCCESS)
	{
		MessageBox(NULL, "RegSetValueEx(hKeyStartup, RegProgramName, 0, REG_SZ, (CONST BYTE*)lpszExePath, strlen(lpszExePath))", GetErrorMessage(lRetVals), MB_OK | MB_ICONERROR);
		return ;
	}
	free(lpszExePath);
	RegCloseKey(hKeyStartup);
}

int main(void)
{
	GetSyllabus();
	GetBlackList();
	GetTime();

	//debug_lessons();

	while(1)
	{
		if(IsAddedStartup() == FALSE)
			AddStartup();
		if(IsLessonInBlackList(WhichLessonAreWeIn()) == TRUE) /* Sabotage the lesson */
		{
			SabotageTheLesson();
		}
		Sleep(10);
	}

	return 0;
}
