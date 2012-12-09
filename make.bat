rem WinGCC
@echo off
gcc -c -g -Wall -pedantic browser.c
gcc -c -g -Wall -pedantic server.c
gcc -c -g -Wall -pedantic serverInfo.c
windres browser_res.rc -o browser_res.o
gcc -mwindows browser.o browser_res.o server.o serverInfo  -lws2_32 -lcomctl32 -o AMLocalBrowser
del browser.o
del server.o
del serverInfo.o
del browser_res.o
pause