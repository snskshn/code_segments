/*
    userver -- (pronounced you-server or micro-server).
    This file is part of the userver, a high-performance web server designed for
    performance experiments.

    This file is Copyright (C) 2006  David Pariag

    This file is part of userver, a simple web server designed for
    performance experiments.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA
*/

#ifndef LOGGING_H
#define LOGGING_H

#include "info.h"

/* Maximum length of a logfile's name (not path) */
#define MAX_LOGFILE_LEN 32


/* Initialize this logging module. This function must be called
   before any requests are logged */
int logging_init( char * logfile );

/* Log the request represented by the given info ptr */
void log_request( struct info * ip );

/* Cleanup and exit the logging code */
void logging_cleanup();

#endif /* LOGGING_H */
