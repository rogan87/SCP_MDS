REM Config viariables
set PERL_ROOT=C:\perl

REM Install NewOpt.pm
COPY NewOpts.pm %PERL_ROOT%\site\lib

REM Install serial port module.
CD SerialPort-0.19
%PERL_ROOT%\BIN\PERL.EXE Makefile.PL
%PERL_ROOT%\BIN\PERL.EXE install.pl
CD ..

