@echo off

@echo.
@echo  Copyright(c) 2014-2015 MDS Technology Co.,Ltd.
@echo  All rights reserved.
@echo.

set DOS_NEOS178S_BASE=%CD%
set NDK_BASE=%DOS_NEOS178S_BASE%\..\
set DOS_NEOS178S_TEST_BASE=%NDK_BASE%Test

IF "%1" == "-h" (
goto usage
) ELSE (
set NEOS178S_BASE=%DOS_NEOS178S_BASE:\=/%
set NEOS178S_TEST_BASE=%DOS_NEOS178S_TEST_BASE:\=/%
@REM cd %DOS_NEOS178S_BASE%
)

set PATH=%PATH%;%NDK_BASE%\Tools\bin
set ARCH=powerpc
set CPU=ppce200z7
set CPU_VARIANT=mpc5674
set BSP=mpc567x-evb
set PROJECT=myproj
set TOOLS=gnu
set T32CAST=C:\Trace32\demo\t32cast\bin\win-x64\t32cast.exe
set DEMO_PATH = C:\Users\KyungChul.Chul\Documents\git\neos178s_fcc\Test\HLT\TimeManagement

if exist env.log @del env.log
if not exist %DOS_NEOS178S_BASE%\bsp\%BSP% (
@echo Please check BSP!!!
for /R "%CD%\bsp" %%d in (.) do @echo         %%d
goto exit
)

@echo ======================================== >> env.log
@echo ARCH = %ARCH% >> env.log
@echo CPU  = %CPU% >> env.log
@echo BSP  = %BSP% >> env.log
@echo NEOS178S_BASE = %NEOS178S_BASE%  >> env.log
@echo ======================================== >> env.log
@echo.
title NEOS Development Shell %ARCH% %CPU% %BSP%
goto exit

:usage
@echo.
@echo Usage :
@echo     %0 NEOS_BASE
@echo.
@echo     BSP  
for /R "%CD%\bsp" %%d in (.) do @echo         %%d


:exit



