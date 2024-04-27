@echo off

refreshenv && ^
vcvars64 && ^
cl /Fe:finder.exe /Foobj/ /Isqlite3 main.c /link /ENTRY:main ./sqlite3/sqlite3.lib vcruntime.lib kernel32.lib ucrt.lib msvcrt.lib