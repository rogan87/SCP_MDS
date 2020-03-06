#!c:/perl/bin/perl.exe
##############################################################################
# Copyright (C) 2008, MDS Technology Co., Ltd. All rights reserved.
#
# MDS Technology Co., Ltd.
# 15F., Kolon Digital Tower Bilant, Guro3-dong, Guro-gu, 
# Seoul,  Korea, 152-777
# 
##############################################################################

##############################################################################
# required modules
##############################################################################
# find the location of the script
my @homePath = split /\\|\//, $0;
my $scriptName = pop @homePath;
my $scriptPath = join '/', @homePath;

# look for modules where the script lives
push @INC, $scriptPath;

use strict;
use Win32;    ## not required under all circumstances
require 5.003;
require NewOpts;
use Win32::SerialPort qw( :PARAM :STAT 0.19 );
use Carp;
use Cwd;
use Win32::Process;
use IO::Socket;

my $sock;
my $PORTNO = 9050;
my $MAXLEN = 1024;
my $TIMEOUT = 5;
my $recvMsg;

my $exitCode = 0;
my $cwd = getcwd;

$| = 1; # set autoflush for all file handles

##############################################################################
# define the command line options and their attributes
##############################################################################
my $options = <<OPTS
baudrate  is_number has_default(115200) has_explanation(sets the baudrate of the serial port)
comport   has_default(COM1) has_explanation(name of the normal serial port i.e. COM1) has_type(COM_PORT)
config    has_default(neos.cfg) has_explanation(name of the configuration file) has_type(filespec)
delay     has_default(3) is_number has_explanation(delay after test completion in seconds)
debugport has_default(COM1) has_explanation(name of the debug serial port i.e. COM1) has_type(COM_PORT)
ipaddr    has_explanation(IP address that the server will bind to) has_type(IP_ADDR) has_default(192.168.10.127)
log       is_boolean has_explanation(log incomming data from target)
monitor   has_explanation(monitor type: xMon or CFE) has_default(=>) has_enumeration(=>,xmon)
raw       is_boolean has_explanation(print raw data to the standard out. option log is ignored)
timeout   is_number has_default(20) has_explanation(time in seconds that the server will wait for a request)
arglist   is_filespec must_exist arg_count(>0) has_extension(exe,elf,bin)
arglist   has_description(TP_SRD_subsys_NNNN.elf)
OPTS
;

# parse the command line
my $opts = new NewOpts($options);

##############################################################################
# main control loop
##############################################################################
my $line;
my %testTags =
(
    terminator    => '\[\[TFXExit\]\]',
    coverageStart => '\[\[TFXCoverageDataStart\]\]',
    coverageEnd   => '\[\[TFXCoverageDataEnd\]\]',
    testName      => '\[\[TFXTestName\]\]',
    testVersion   => '\[\[TFXTestVersion\]\]',
    testResult    => '\[\[TFXTestResult\]\]',
);

# create the port object
my $cfgFile = $scriptPath . "/CFG_" . $opts->comport . ".cfg";
my $serialPort = initializeSerialPort($opts->comport, $opts->baudrate, $cfgFile);

# get the test file names
my @argList   = $opts->getArgList;
my $firstFile = $argList[0];
my $tftpFile  = "$cwd/$firstFile";
my $first     = 1;
 
# start the TFTP server
#my $tftpdOBJ = Net::TFTPd->new('FileName' => $firstFile, 'LocalAddr' => $opts->ipaddr)
#      or die "Error creating TFTPd listener: %s", Net::TFTPd->error;


# execute each test
foreach my $testImage (@argList)
{
  $testImage =~ s/\\/\//g;
  print "TEST: $testImage\n" if $opts->verbose;
  my @testResults;
  my $tftpRQ;

  $tftpFile = "$cwd/$testImage";
  
  # give the target reset time to complete
#  sleep 2 if ! $first;
#  $first = 0;

  # tell the target to download and run the test
  if($opts->monitor eq "=>")
  {
     $sock = IO::Socket::INET->new(Proto     => 'udp',
                              PeerPort  => $PORTNO,
                              PeerAddr  => 'localhost')
    or die "Creating socket: $!\n";

    # send absolute file path to server.
    $sock->send($tftpFile) or die "send: $!";
    
    # sleep until sending is completed
    #sleep 1;

    $sock->recv($recvMsg, $MAXLEN) or die "recv: $!";
    print "remote execution done.\n";
    
    $sock->close() or die "close: $!";

  }

  print "FILE: $testImage\n" if $opts->verbose;

  # log the test results
  my $LOG;
  if ($opts->log) 
  {
    my ($testResultsFile, $testCoverageFile) = getOutputFileNames($testImage);
    my $logFile = $testResultsFile;
    $logFile =~ s/\....$/.log/;
    $logFile =~ s/\/Result// if $opts->raw;
    open($LOG, ">$logFile") || die "Couldn't open log file: $logFile";
    print $LOG "$0: Command Line Arguments:\n";
    foreach my $arg (@ARGV)
    {
      print $LOG "$arg ";
    }
    printf $LOG "\nOPTIONS:\n%s\n", $opts->to_s();
    flush $LOG;
  }

  # read the results
  do
  {
    $line = readLine();
    if($opts->log) # log 'em if you got 'em...
    {
      print $LOG "$line\n";
      flush $LOG;
    }
    if ($opts->raw)
    {
      print "$line\n";
    }
    else 
    {
      print "LINE: !$line|\n" if $opts->verbose(2);
      push @testResults, $line;
    }
    if($line =~ /^Fault:/)
    {
      carp "TARGET Reset!! $line";
      $exitCode = -1;
      last;
    }
  } while($line !~ /$testTags{terminator}/i);


  # close the log file
  if ($opts->log) 
  {
    close $LOG;
  }

  if (!$opts->raw)
  {  
    # put the output somewhere useful
    my ($testResultsFile, $testCoverageFile) = getOutputFileNames($testImage);

    # save the coverage information
    my $COV;
    my $results;

    open($COV, ">$testCoverageFile") || die "Couldn't open coverage file: $testCoverageFile";
    my $coverageStarted = 0;
    print $COV "COVERAGE results for $testImage\n";
    foreach $line (@testResults)
    {
      if(!$coverageStarted)
      {
        $coverageStarted = 1 if $line =~ /$testTags{coverageStart}/i;
        next;
      }
      last if $line =~ /$testTags{coverageEnd}/i;
      print $COV "$line\n";
    }
    close $COV;

    # save the test results
    open($results, ">$testResultsFile") || die "Couldn't open results file: $testResultsFile";

    my $testName;
    my $testVersion;
    my $testResult;
    my $testResultCode;
    my $testExitLine;
    my $done = 0;

    foreach $line (@testResults)
    {
      last if $line =~ /$testTags{coverageStart}i/;
      local $\ = "\n";
      for ($line)
      {
        /$testTags{testName}/i    && do { my @parts       = split /\s+/, $line;
                                          $testName       = $parts[2];
                                          $testName       =~ s/.c$//;
                                          $testName       =~ s/Source\///;
                                        };
        /$testTags{testVersion}/i && do { my @parts       = split /\s+/, $line;
                                          $testVersion    = $parts[3];
                                        };
        /$testTags{testResult}/i  && do { my @parts       = split /\s+/, $line;
                                          $testResult     = $parts[1];
                                          $testResultCode = $parts[2];
                                          $testExitLine   = $parts[3];
                                          $done = 1;
                                        };
      }
      if($done)
      {
        my $date = getAsciiDate();
        local $\ = "\n";
        print $results "$testName,$date,$testResult,$testResultCode,$testExitLine";

        # look for the next result
        $testName       = "";
        $testVersion    = "";
        $testResult     = "";
        $testResultCode = "";
        $done = 0;
      }
    }
    close $results;
  } # $opts->raw

  if($opts->delay > 0)
  {
    my $delay = $opts->delay;
    sleep $delay;
  }
}

# cleanup the port object
destroySerialPort($serialPort);

##############################################################################
exit $exitCode; # end of processing ##########################################
##############################################################################

########################################################################
#################### Subroutines begin here ############################
########################################################################

########################################################################
# readLine - read until CR
########################################################################
sub readLine
{
  my $line;
  my $inChr;

  do
  {
    $inChr = getc SP;
    $line .= $inChr if $inChr ne "";
  } while($line !~ /\n/);
  $line =~ s/[\r\n]//g;
  return $line;
}

########################################################################
# destroyComPort - Communications port destruction
########################################################################
sub getOutputFileNames
{
  my ($testImage) = @_;
  my $testResultsFile;
  my $testCoverageFile;

  my @pathParts = split /\\|\//, $testImage;
  my $fileName  = pop @pathParts;
  my $objDir    = pop @pathParts;
  my $basePath;
  if(exists $pathParts[0])
  {
    $basePath = join "/", @pathParts;
  }
  else
  {
    $basePath = ".";
  }

  $testResultsFile   = join "/", $basePath, "Result", $fileName;
  $testCoverageFile  = $testResultsFile;

  $testResultsFile  =~ s/.elf$/.csv/;
  $testCoverageFile =~ s/.elf$/.dat/;

  return ($testResultsFile, $testCoverageFile);
}

########################################################################
# destroyComPort - Communications port destruction
########################################################################
sub destroySerialPort
{
  my ($serialPort) = @_;
  $serialPort->close || warn "couldn't close serial port";
  undef $serialPort;
  untie *SP;
}


########################################################################
# initializeSerialPort - Serial port initialization
########################################################################
sub initializeSerialPort
{
  my ($portName, $baudrate, $cfgFile) = @_;
  genConfigFile($portName, $cfgFile);

  print "INIT: $portName $baudrate $cfgFile\n" if $opts->verbose;
  do 
  {
      #$serialPort = tie (*SP, 'Win32::SerialPort', $cfgFile) || die "Can't tie: $^E\n"; ## TIEHANDLE ##
      $serialPort = tie (*SP, 'Win32::SerialPort', $cfgFile);
      sleep 1 if ! $serialPort;
  } while (!$serialPort);

  # set the configurable parameters
  $serialPort->baudrate($baudrate);
  $serialPort->is_stty_eol(10);

  $serialPort->write_settings || undef $serialPort;
  print "Can't change Device_Control_Block: $^E\n" unless ($serialPort);

  return $serialPort;
}

########################################################################
# getAsciiDate - get an ASCII formatted date string
########################################################################
sub getAsciiDate
{
  my $date;
  my @shortMons = qw( JAN FEB MAR APR MAY JUN JUL AUG SEP OCT NOV DEC );
  my @shortDays = qw( Sun Mon Tue Wed Thu Fri Sat );

     #  0    1    2     3     4    5     6     7     8
  my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
  my $year   = $year + 1900;
  my $month  = $shortMons[$mon];
  my $day    = $mday;
  my $dayStr = $shortDays[$wday];

  $date =  sprintf "%s %02d-%s-%d %02d:%02d:%02d",
          $dayStr, $day, $month, $year, $hour, $min, $sec;
  return $date;
}

########################################################################
# genConfigFile - generate the configuration file j
########################################################################
sub genConfigFile
{
  my ($portName, $cfgFile) = @_;
  my $CFG;
  open($CFG, ">$cfgFile") || die "couldn't write $cfgFile";

  print $CFG <<CONFIGURATION
Win32::SerialPort_Configuration_File -- DO NOT EDIT --
$portName
CFG_1,none
eol,10
clear,-@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@-
RCONST,1000000
istrip,0
CFG_2,none
XOFFCHAR,19
PARITY_EN,0
WCONST,200
intr,3
U_MSG,1
STOP,1
XONLIM,100
erase,8
XONCHAR,17
BINARY,1
RTOT,0
echonl,0
XOFFLIM,200
icrnl,0
inlcr,0
READBUF,32768
igncr,0
EOFCHAR,0
WRITEBUF,4096
RINT,4294967295
ocrnl,0
bsdel, 
opost,0
echoke,0
PARITY,none
HNAME,localhost
echoctl,0
CFG_3,none
EVTCHAR,0
icanon,0
isig,0
HADDR,0
E_MSG,1
DATA,8
DVTYPE,none
echo,0
quit,4
s_eof,26
s_kill,21
ERRCHAR,0
onlcr,0
ALIAS,AltPort
HSHAKE,none
DATYPE,raw
echok,0
echoe,0
BAUD,115200
WTOT,10
CONFIGURATION
;

close $CFG;
}

