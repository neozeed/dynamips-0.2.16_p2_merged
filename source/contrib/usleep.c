#include <windows.h>
#include <stdio.h>


//This implimentation is from facebook.
//https://github.com/facebook/watchman/blob/master/winbuild/time.c
/* Copyright 2014-present Facebook, Inc.
 * Licensed under the Apache License, Version 2.0 */
/*
void usleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}*/


//Not sure where this usleep comes from
//http://eudaq.hepforge.org/svn/trunk/tlu/src/win_uSleep.cpp ??
//Note MinGW has a usleep, but it doesn't actually sleep so
//I'm renaming this to Musleep ...
void Musleep(int waitTime) {
    __int64 time1 = 0, time2 = 0, freq = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);
		Sleep(5);
    } while((time2-time1) < waitTime);
}
