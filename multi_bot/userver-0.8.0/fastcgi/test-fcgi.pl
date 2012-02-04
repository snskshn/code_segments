#!/usr/bin/perl

#
#    userver -- (pronounced you-server or micro-server).
#    This file is part of the userver, a high-performance web server designed for
#    performance experiments.
#          
#    This file is Copyright (C) 2004  Craig Barkhouse
#
#    Authors: Craig Barkhouse <cabarkho@uwaterloo.ca>
#    See AUTHORS file for list of contributors to the project.
#  
#    This program is free software; you can redistribute it and/or
#    modify it under the terms of the GNU General Public License as
#    published by the Free Software Foundation; either version 2 of the
#    License, or (at your option) any later version.
#  
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    General Public License for more details.
#  
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
#    02111-1307 USA
#

use FCGI;

my %ORIGENV = %ENV;

my $sockpath = ":8000";
if (defined $ARGV[0]) { $sockpath = $ARGV[0]; }
my $socket = FCGI::OpenSocket($sockpath, 5);
my $fcgi = FCGI::Request(\*STDIN, \*STDOUT, \*STDERR, \%ENV, $socket);

use CGI;

my $count = 0;

while ($fcgi->Accept() >= 0) {

  # initialize the CGI library
  CGI::initialize_globals();

  # override any CGI globals here
  #$CGI::POST_MAX = 1024 * 10;
  #$CGI::DISABLE_UPLOADS = 1;

  # initialize a CGI object
  $cgi = new CGI;

  ############################## BEGIN CGI ##############################

  print "Content-type: text/plain\r\n\r\n";

  $count++;
  print "Generating reply #$count at ", `date "+%a %b %d %T %Y"`;
  print "\n";

  print "environment variables:\n";
  foreach $var (sort keys(%ENV)) {
    print "$var=$ENV{$var}\n";
  }
  print "\n";

  print "CGI parameters:\n";
  foreach $name (sort $cgi->param()) {
    $value = $cgi->param($name);
    print "$name=$value\n";
  }
  print "\n";

  ############################### END CGI ###############################

  # destroy the CGI object
  $cgi->delete();

  # restore original environment
  %ENV = %ORIGENV;

}

FCGI::CloseSocket($socket);
