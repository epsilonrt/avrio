@ECHO off
SETLOCAL ENABLEDELAYEDEXPANSION

SET /A ARGS_COUNT=0
FOR %%a in (%*) DO SET /A ARGS_COUNT+=1
REM ECHO ARGS_COUNT=!ARGS_COUNT!

IF !ARGS_COUNT! LSS 1 (
  ECHO %0: You must provide the name of the file to be created
  EXIT /B -1
)

git.exe --version >NUL 2>&1 && SET GIT_EXISTS=1
diff.exe --version >NUL 2>&1 && SET DIFF_EXISTS=1

IF !GIT_EXISTS! EQU 1 (
  REM ECHO "git found"
  set current_version=
  FOR /f "delims=" %%a in ('git describe') DO @SET "current_version=%%a"
  REM ECHO current_version=!current_version!
  ECHO #define VERSION "!current_version!" > .version
  
  IF !DIFF_EXISTS! EQU 1 (
    diff -q -w .version %1 >NUL 2>&1 && EXIT /B 0
  )
) ELSE (
  ECHO %0: %1 will not be updated because git has been found on this system
)

COPY .version %1 >NUL
TYPE %1
