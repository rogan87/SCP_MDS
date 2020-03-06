##############################################################################
# Copyright (C) 2008, Dave Cassidy. All rights reserved.
# 
# This module is free software; you can redistribute it and/or modify it
# under the same terms as Perl itself.
#
##############################################################################
package NewOpts;

use strict;

BEGIN
{
  use Exporter   ();
  use vars       qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);
  $VERSION =     '$Revision: 48 $';
  @ISA =         qw(Exporter);
  @EXPORT =      qw();
  @EXPORT_OK =   qw( %optAttributes %optValidations );
  %EXPORT_TAGS = qw();
}

my $defaultArgs = <<ARGS
config    has_default(config.cfg) has_explanation(name of the configuration file) has_type(filespec)
verbose is_boolean has_explanation(print additional debugging information) has_default(0)
help    is_boolean has_explanation(print this information and exit)
ARGS
;

###########################################################################
# Set the attributes of the command line arguments
###########################################################################
my %optAttributes = (
    has_default     => sub { $_[0]->{OPTIONS}{$_[1]}{VALUE}          = $_[2];
                             $_[0]->{OPTIONS}{$_[1]}{DEFAULT}        = $_[2]             },
    has_explanation => sub { $_[0]->{OPTIONS}{$_[1]}{EXPLANATION}    = $_[2]             },
    has_description => sub { $_[0]->{OPTIONS}{$_[1]}{DESCRIPTION}    = $_[2]             },
    is_filespec     => sub { $_[0]->{OPTIONS}{$_[1]}{TYPE}           = "FILE_SPEC"       },
    is_boolean      => sub { $_[0]->{OPTIONS}{$_[1]}{TYPE}           = "BOOLEAN";
                             $_[0]->{OPTIONS}{$_[1]}{VALUE}          = 0;
                             push @{$_[0]->{OPTIONS}{$_[1]}{VALIDATE}}, "is_boolean";    },
    is_number       => sub { $_[0]->{OPTIONS}{$_[1]}{TYPE}           = "NUMBER";
                             push @{$_[0]->{OPTIONS}{$_[1]}{VALIDATE}}, "is_number";     },
    must_exist      => sub { push @{$_[0]->{OPTIONS}{$_[1]}{VALIDATE}}, "must_exist"     },
    must_not_exist  => sub { push @{$_[0]->{OPTIONS}{$_[1]}{VALIDATE}}, "must_not_exist" },
    is_required     => sub { push @{$_[0]->{OPTIONS}{$_[1]}{VALIDATE}}, "is_required";
                             $_[0]->{OPTIONS}{$_[1]}{REQUIRED}       = 1;                },
    arg_count       => sub { $_[0]->{ARG_LIST}{ARG_COUNT}            = $_[2]             },
    has_type        => sub { $_[0]->{OPTIONS}{$_[1]}{TYPE}           = $_[2]             },
    has_enumeration => sub { my @enumList = split ',', $_[2];
                             foreach my $enum (@enumList)
                             {
                                $_[0]->{OPTIONS}{$_[1]}{ENUMERATION}{lc $enum} = 1;
                             }
                             push @{$_[0]->{OPTIONS}{$_[1]}{VALIDATE}}, "has_enumeration";
                             $_[0]->{OPTIONS}{$_[1]}{TYPE}            = "ENUMERATION"     },
    has_extension   => sub { my @extList = split ',', $_[2];
                             foreach my $ext (@extList)
                             {
                                $_[0]->{OPTIONS}{$_[1]}{EXTENSION}{lc $ext} = 1;
                             }
                             push @{$_[0]->{OPTIONS}{$_[1]}{VALIDATE}}, "has_extension"; },
);
$optAttributes{is_only_digits} = $optAttributes{is_number};

###########################################################################
# Validate the command line arguments
###########################################################################
my %optValidations = (
    is_required     => sub { exists($_[0]->{OPTIONS}{$_[1]}{VALUE}) ||
                             !exists($_[0]->{OPTIONS}{$_[1]}{REQUIRED})             },
    is_number       => sub { $_[2] =~ /^[0-9]+$/                                    },
    must_not_exist  => sub { ($_[0]->{OPTIONS}{$_[1]}{TYPE} ne "FILE_SPEC") ||
                             ( ! -e $_[2] )                                         },
    must_exist      => sub { ($_[0]->{OPTIONS}{$_[1]}{TYPE} ne "FILE_SPEC") ||
                            ( -e $_[2] )                                            },
    has_enumeration => sub { return $_[0]->{OPTIONS}{$_[1]}{ENUMERATION}{lc $_[2]}; },
    has_extension   => sub { my @p = split '\.',$_[2]; my $e = pop @p; return
                             $_[0]->{OPTIONS}{$_[1]}{TYPE} ne "FILE_SPEC"   ||
                             $_[0]->{OPTIONS}{$_[1]}{EXTENSION}{lc $e};             },
    is_boolean      => sub { $_[0]->{OPTIONS}{$_[1]}{VALUE} =
                             defined $_[2] ? $_[2] : 1;
                             $_[0]->{OPTIONS}{$_[1]}{VALUE} =~ /^[0-9]+$/;          },
);

###########################################################################
# create the object, and initialize the option/attribute hash
###########################################################################
sub new
{
  my $proto         = shift;
  my $options       = shift || "NO_ARGS";
  my $class         = ref($proto) || $proto;
  my $self          = {};
  $self->{NAME}     = $class;
  $self->{OPTL_LEN} = 0;

  # find the location of the script
  my @homePath = split /\\|\//, $0;
  my $scriptName = pop @homePath;
  my $scriptPath = join '/', @homePath;

  # look for modules (and the config file) where the script lives
  push @INC, $scriptPath;

  bless $self, $class; # for I have sinned ...

  # add default args
  $options = $defaultArgs . $options;

  # read in the option descriptions
  my @opts = split '\n', $options;
  foreach my $opt (@opts)
  {
    $self->getAttributes($opt);
  }

  # process the command line opts
  $self->getOpts();

  return $self;
}

###########################################################################
# convert the option/value pairs to strings
###########################################################################
sub to_s
{
  my $self = shift;
  my $optStr = "";
  my $option;
  my $value;
  my $arg;
  my $optFmtStr = sprintf "%%-%ds=> %%s\n", $self->{OPT_LEN} + 1;

  foreach $option (sort keys %{$self->{OPTIONS}})
  {
    my $optValStr;
    next if $option eq "arglist";
    $value = $self->{OPTIONS}{$option}{VALUE};
    $optValStr = sprintf $optFmtStr, $option, $value;
    $optStr .= $optValStr;
  }

  foreach $arg ( $self->getArgList() )
  {
    $optStr .= "ARG: $arg\n";
  }
  return $optStr;
}

###########################################################################
# return the command line arguments
###########################################################################
sub getArgList
{
   my $self = shift;
   my @arglist = ();
   @arglist = @{$self->{ARG_LIST}{ARGS}} if exists $self->{ARG_LIST}{ARGS};
   return @arglist;
}

###########################################################################
# parse the attributes of each command line option
###########################################################################
sub getAttributes
{
   my $self = shift;
   my $optStr = shift;
   my @validations = split /\s+/, $optStr;

   my $optName      =  shift @validations;
   my $validation;
   my $attribute;
   $self->{OPT_LEN} = length $optName if length $optName > $self->{OPT_LEN};
   while($validation = shift @validations)
   {
      my $attribute;
      if($validation =~ /\(/)
      {
         ($validation, $attribute) = split '\(', $validation;
         while($attribute !~ /\)/)
         {
            $attribute .= " " . shift @validations;
         }
         $attribute =~ s/\)//;
      }

      #printf "VALIDATION: %-12s %-12s(%s)\n", $optName, $validation, $attribute;
      if(exists $optAttributes{$validation})
      {
         $optAttributes{$validation}->($self, $optName, $attribute);
      }
      else
      {
         print "$self->{NAME}: $optName: invalid attribute $validation\n";
      }
   }
}

###########################################################################
# default verbose function. deals with multiple levels of verbosity
###########################################################################
sub verbose
{
    my $self = shift;
    my $verbose = shift || 1;
    return $verbose <= $self->{OPTIONS}{verbose}{VALUE};
}

###########################################################################
# process an arg, value pair
###########################################################################
sub process_arg
{
   my $self = shift;
   my ($key, $value) = @_;
   $key =~ s/^--//;
   if(exists $self->{OPTIONS}{$key})
   {
      printf "PROCESS: $key: $value\n" if $self->verbose(2);
      $self->{OPTIONS}{$key}{COMMANDED} = 1;
      $self->{OPTIONS}{$key}{VALUE} = $value if defined $value;
      foreach my $validation (@{$self->{OPTIONS}{$key}{VALIDATE}})
      {
         printf "VALIDATE: $key: $validation $value\n" if $self->verbose(2);
         if(! $optValidations{$validation}->($self, $key, $value))
         {
             print "$0: invalid argument: $key($value)\n";
             $self->usage;
         }
      }
   }
   else
   {
      print "UNKNOWN ARGUMENT: $key (ignored)\n";
   }
}

###########################################################################
# read the config file, if specified, and if existing
###########################################################################
sub readConfigFile
{
   my $self = shift;
   my $configFile;
   my $path;

   if($self->{OPTIONS}{config})
   {
      my $configFileName = $self->{OPTIONS}{config}{VALUE};
      foreach $path (@INC)
      {
         my $file = $path . "/" . $configFileName;
         if( -e $file)
         {
            $configFile = $file;
            last;
         }
      }
      if( -e $configFile)
      {
         my $inFile;
         my $opt;
         my $value;
         my $line;

         open($inFile, "<$configFile") || die "couldn't open config file $configFile";

         foreach $line (<$inFile>)
         {
            next if $line =~ /^\s*#/; # skip comments
            next if $line =~ /^\s*$/; # skip blanks
            chomp $line;
            ($opt,$value) = split /\s*=\s*/, $line;
            if(exists $self->{OPTIONS}{$opt} and !exists $self->{OPTIONS}{$opt}{COMMANDED})
            {
               $self->process_arg($opt, $value);
            }
         }
         close $inFile;
      }
   }
}

###########################################################################
# parse the command line and validate the options and arguments
###########################################################################
sub getOpts
{
   my $self = shift;
   
   # don't process the arguments twice
   if(!exists $self->{PROCESSED_OPTS})
   {
      $self->{PROCESSED_OPTS} = 1;
   }
   else
   {
      return;
   }

   # process the command line options
   foreach my $arg (@ARGV)
   {
      if ( $arg =~ /^--/ )
      {
         my ($key, $value) = split '=', $arg;
         $key =~ s/^--//;
         $self->process_arg($key, $value);
      }
      else
      {
         # capture any unnamed arguments in the ARGLIST array
         push @{$self->{ARG_LIST}{ARGS}}, $arg;
      }
   }

   # read the config file, if specified, and if existing
   $self->readConfigFile();

   # validate the options
   foreach my $key ( keys %{$self->{OPTIONS}} )
   {
      # make sure we got all of the required arguments
      if(! $optValidations{is_required}->($self, $key))
      {
         print "$0: --$key is a required argument\n";
         $self->usage();
      }
      # make the option name return the option value
      unless($key eq "verbose")
      {
         no strict 'refs';
         *{$key} = sub { $self->{OPTIONS}{$key}{VALUE} };
      }
   }

   # no further validation if user explicitly requested help
   $self->usage if $self->{OPTIONS}{help}{VALUE};

   # validate the argument count if directed to do so
   if(exists $self->{ARG_LIST}{ARG_COUNT})
   {
      my $argCount       = exists $self->{ARG_LIST}{ARGS} ? @{$self->{ARG_LIST}{ARGS}} : 0;
      my $argCountLimits = $self->{ARG_LIST}{ARG_COUNT};
      my ($lowLimit, $highLimit) = getArgLimits($argCountLimits);
      if($argCount < $lowLimit || $argCount > $highLimit)
      {
         print "$0: argument count must be $argCountLimits\n";
         $self->{OPTIONS}{help}{VALUE} = 1;
      }
   }

   #validate each argument
   foreach my $arg (@{$self->{ARG_LIST}{ARGS}})
   {
     foreach my $validation (@{$self->{OPTIONS}{arglist}{VALIDATE}})
     {
        printf "VALIDATE: arglist: $validation $arg\n" if $self->verbose(2);
        if(! $optValidations{$validation}->($self, "arglist", $arg))
        {
            print "$0: ($arg) fails $validation\n";
            $self->usage;
        }
     }
   }

   # babble
   if($self->verbose(2))
   {
      foreach my $arg (@{$self->{ARGLIST}{ARGS}})
      {
         print "ARG:$arg\n";
      }
   }

   # send help if requested
   if($self->{OPTIONS}{help}{VALUE})
   {
      print "$0: --help\n" if $self->verbose(2);
      $self->usage;
   }
}

###########################################################################
# process the argument limits specification: arg_count(>x) e.g.
###########################################################################
sub getArgLimits
{
   my ($argCountLimits) = @_;
   my $lowLimit;
   my $highLimit;

   if($argCountLimits =~ /([0-9]+)\s*-\s*([0-9]+)/)
   {
      $lowLimit  = $1;
      $highLimit = $2;
   }
   elsif($argCountLimits =~ />([0-9]+)/)
   {
      $lowLimit  = $1 + 1;
      $highLimit = 4000000;
   }
   elsif($argCountLimits =~ /<([0-9]+)/)
   {
      $lowLimit  = 1;
      $highLimit = $1 - 1;
   }
   elsif($argCountLimits =~ /([0-9]+)/)
   {
      $lowLimit  = $1;
      $highLimit = $1;
   }
   return($lowLimit, $highLimit);
}

###########################################################################
# print a useful usage message from the option definitions
###########################################################################
sub usage
{
   my $self       = shift;
   my @pathSegs   = split "/", $0;
   my $file       = pop @pathSegs;
   my $keyLength  = 0;
   my $typeLength = 0;

   print "USAGE: $0:\n";
   print "$file ";

   foreach my $key ( sort keys %{$self->{OPTIONS}} )
   {
      next if $key eq "arglist";
      my $type        = $self->{OPTIONS}{$key}{TYPE};
      my $description = $type;
      $keyLength  = length $key  if length $key  > $keyLength;
      $typeLength = length $type if length $type > $typeLength;

      if(defined $self->{OPTIONS}{$key}{DESCRIPTION})
      {
         $description = $self->{OPTIONS}{$key}{DESCRIPTION};
      }
      if( $self->{OPTIONS}{$key}{REQUIRED})
      {
         if($type eq "BOOLEAN")
         {
             print "--$key ";
         }
         elsif($type eq "ENUMERATION")
         {
            my $enumStr = join "|", sort keys %{$self->{OPTIONS}{$key}{ENUMERATION}};
             print "--$key=$enumStr ";
         }
         else
         {
             print "--$key=$description ";
         }
      }
      else
      {
         if($type eq "BOOLEAN")
         {
             print "[--$key] ";
         }
         elsif($type eq "ENUMERATION")
         {
            my $enumStr = join "|", sort keys %{$self->{OPTIONS}{$key}{ENUMERATION}};
             print "[--$key=$enumStr] ";
         }
         else
         {
             print "[--$key=$description] ";
         }
      }
   }
   my $argCountLimits = $self->{ARG_LIST}{ARG_COUNT};
   my ($lowLimit, $highLimit) = getArgLimits($argCountLimits);
   my $continues = "";
   if($highLimit > $lowLimit)
   {
      $continues = "[...]";
   }
   if($lowLimit > 0)
   {
      printf "%s %s\n", $self->{OPTIONS}{arglist}{DESCRIPTION}, $continues;
   }
   elsif(exists $self->{OPTIONS}{arglist}{DESCRIPTION})
   {
      printf "[%s] %s\n", $self->{OPTIONS}{arglist}{DESCRIPTION}, $continues;
   }
   else
   {
      printf "\n";
   }

   my $fmtString = sprintf "    --%%-%ds %%-%ds %%s\n", $keyLength, $typeLength + 1;
   foreach my $key ( sort keys %{$self->{OPTIONS}} )
   {
      next if $key eq "arglist";
      printf $fmtString, $key,
                         $self->{OPTIONS}{$key}{TYPE} . ":",
                         $self->{OPTIONS}{$key}{EXPLANATION};
      if($self->{OPTIONS}{$key}{ENUMERATION})
      {
        printf "%senum( ", " " x ($keyLength + $typeLength + 9);
        foreach my $enumVal (sort keys %{$self->{OPTIONS}{$key}{ENUMERATION}})
        {
            printf "%s ", $enumVal;
        }
        printf ")\n";
      }
      if(defined($self->{OPTIONS}{$key}{DEFAULT}))
      {
        printf "%sdefault(%s)\n", " " x ($keyLength + $typeLength + 9), $self->{OPTIONS}{$key}{DEFAULT};
      }
   }

   if($self->verbose)
   {
     printf "Current Values:\n";
     printf "%s\n", $self->to_s();
   }
   exit;
}

1;
