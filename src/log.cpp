#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>


static FILE *f_log = 0;


void log(const char *fmt_spec, ...)
{
    if(!f_log)
	f_log = fopen("blockip.log", "a");



    if(!fmt_spec || !fmt_spec[0])
    {
	fprintf(f_log, "\n");
	fflush(f_log);
	return;
    };

    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    time_t tu = tv.tv_sec;
    tm t;
    localtime_r(&tu, &t);

    fprintf(f_log, "%02d.%02d.%04d-%02d:%02d:%02d.%06d: ", t.tm_mday,
	t.tm_mon+1, t.tm_year+1900, t.tm_hour, t.tm_min, t.tm_sec, (int)tv.tv_usec);

    va_list arglist;
    va_start (arglist, fmt_spec);
    vfprintf(f_log, fmt_spec, arglist);
    va_end(arglist);
    fprintf(f_log, "\n");

    fflush(f_log);
};

