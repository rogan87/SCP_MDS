REM T32 script file to boot MPC567X image on RAM

SET t32path=C:\T32\bin\windows64

start %t32path%\t32mppc.exe -s %NEOS178S_TEST_BASE%\Tools\mpc5674_ram.cmm

SLEEP 20 

TASKKILL /IM t32mppc.exe

SLEEP 1

goto:eof

