#include <float.h>
#include <time.h>
#include <winbase.h>
#include <sys/timeb.h>


long _timezone = 8 * 3600L; /* Pacific Time Zone */
int _daylight = 1;          /* Daylight Saving Time (DST) in timezone */
long _dstbias = -3600L;     /* DST offset in seconds */

static char tzstd[64] = { "PST" };
static char tzdst[64] = { "PDT" };
char *_tzname[2] = { tzstd, tzdst };

#define ChkAdd(dest, src1, src2)   ( ((src1 >= 0L) && (src2 >= 0L) \
    && (dest < 0L)) || ((src1 < 0L) && (src2 < 0L) && (dest >= 0L)) )

#define ChkMul(dest, src1, src2)   ( src1 ? (dest/src1 != src2) : 0 )

#define _DAY_SEC           (24L * 60L * 60L)    /* secs in a day */
#define _YEAR_SEC          (365L * _DAY_SEC)    /* secs in a year */
#define _FOUR_YEAR_SEC     (1461L * _DAY_SEC)   /* secs in a 4 year interval */
#define _DEC_SEC           315532800L           /* secs in 1970-1979 */
#define _BASE_YEAR         70L                  /* 1970 is the base year */
#define _BASE_DOW          4                    /* 01-01-70 was a Thursday */
#define _LEAP_YEAR_ADJUST  17L                  /* Leap years 1900 - 1970 */
#define _MAX_YEAR          138L                 /* 2038 is the max year */

int _days[] = {
  -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
};

int _lpdays[] = {
  -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};


/*
 * Number of milliseconds in one day
 */
#define DAY_MILLISEC    (24L * 60L * 60L * 1000L)

/*
 * The macro below is valid for years between 1901 and 2099, which easily
 * includes all years representable by the current implementation of time_t.
 */
#define IS_LEAP_YEAR(year)  ( (year & 3) == 0 )

/*
 * Pointer to a saved copy of the TZ value obtained in the previous call
 * to tzset() set (if any).
 */
static char * lastTZ = NULL;

/*
 * Flag indicating that time zone information came from GetTimeZoneInformation
 * API call.
 */
static int tzapiused;

static TIME_ZONE_INFORMATION tzinfo;

/*
 * Structure used to represent DST transition date/times.
 */
typedef struct {
        int  yr;        /* year of interest */
        int  yd;        /* day of year */
        long ms;        /* milli-seconds in the day */
        } transitiondate;

/*
 * DST start and end structs.
 */
static transitiondate dststart = { -1, 0, 0L };
static transitiondate dstend   = { -1, 0, 0L };

static int __cdecl _isindst_lk(struct tm *);

static void __tzset(void);

static struct tm tb = { 0 };

struct tm * xcegmtime(const time_t *timp)
{
  long caltim = *timp;  
  int islpyr = 0;       
  int tmptim;
  int *mdays;           

  struct tm *ptb = &tb;

  if ( caltim < 0L )
    return(NULL);

  tmptim = (int)(caltim / _FOUR_YEAR_SEC);
  caltim -= ((long)tmptim * _FOUR_YEAR_SEC);

  tmptim = (tmptim * 4) + 70; 

  if ( caltim >= _YEAR_SEC ) 
    {
      tmptim++;                 
      caltim -= _YEAR_SEC;
      
      if ( caltim >= _YEAR_SEC ) 
	{
	  tmptim++;               
	  caltim -= _YEAR_SEC;

	  if ( caltim >= (_YEAR_SEC + _DAY_SEC) ) 
	    {
	      tmptim++;
	      caltim -= (_YEAR_SEC + _DAY_SEC);
	    }
	  else 
	    {
	      islpyr++;
	    }
	}
    }

  ptb->tm_year = tmptim;

  ptb->tm_yday = (int)(caltim / _DAY_SEC);
  caltim -= (long)(ptb->tm_yday) * _DAY_SEC;

  if ( islpyr )
    mdays = _lpdays;
  else
    mdays = _days;

  for ( tmptim = 1 ; mdays[tmptim] < ptb->tm_yday ; tmptim++ ) ;

  ptb->tm_mon = --tmptim;

  ptb->tm_mday = ptb->tm_yday - mdays[tmptim];

  ptb->tm_wday = ((int)(*timp / _DAY_SEC) + _BASE_DOW) % 7;

  ptb->tm_hour = (int)(caltim / 3600);
  caltim -= (long)ptb->tm_hour * 3600L;

  ptb->tm_min = (int)(caltim / 60);
  ptb->tm_sec = (int)(caltim - (ptb->tm_min) * 60);

  ptb->tm_isdst = 0;
  return( (struct tm *)ptb );
}

static void __cdecl cvtdate (
        int trantype,
        int datetype,
        int year,
        int month,
        int week,
        int dayofweek,
        int date,
        int hour,
        int min,
        int sec,
        int msec
        )
{
        int yearday;
        int monthdow;

        if ( datetype == 1 ) {

            /*
             * Transition day specified in day-in-month format.
             */

            /*
             * Figure the year-day of the start of the month.
             */
            yearday = 1 + (IS_LEAP_YEAR(year) ? _lpdays[month - 1] :
                      _days[month - 1]);

            /*
             * Figure the day of the week of the start of the month.
             */
            monthdow = (yearday + ((year - 70) * 365) + ((year - 1) >> 2) -
                        _LEAP_YEAR_ADJUST + _BASE_DOW) % 7;

            /*
             * Figure the year-day of the transition date
             */
            if ( monthdow < dayofweek )
                yearday += (dayofweek - monthdow) + (week - 1) * 7;
            else
                yearday += (dayofweek - monthdow) + week * 7;

            /*
             * May have to adjust the calculation above if week == 5 (meaning
             * the last instance of the day in the month). Check if year falls
             * beyond after month and adjust accordingly.
             */
            if ( (week == 5) &&
                 (yearday > (IS_LEAP_YEAR(year) ? _lpdays[month] :
                             _days[month])) )
            {
                yearday -= 7;
            }
        }
        else {
            /*
             * Transition day specified as an absolute day
             */
            yearday = IS_LEAP_YEAR(year) ? _lpdays[month - 1] :
                      _days[month - 1];

            yearday += date;
        }

        if ( trantype == 1 ) {
            /*
             * Converted date was for the start of DST
             */
            dststart.yd = yearday;
            dststart.ms = (long)msec +
                          (1000L * (sec + 60L * (min + 60L * hour)));
            /*
             * Set year field of dststart so that unnecessary calls to
             * cvtdate() may be avoided.
             */
            dststart.yr = year;
        }
        else {
            /*
             * Converted date was for the end of DST
             */
            dstend.yd = yearday;
            dstend.ms = (long)msec +
                              (1000L * (sec + 60L * (min + 60L * hour)));
            /*
             * The converted date is still a DST date. Must convert to a
             * standard (local) date while being careful the millisecond field
             * does not overflow or underflow.
             */
            if ( (dstend.ms += (_dstbias * 1000L)) < 0 ) {
                dstend.ms += DAY_MILLISEC;
                dstend.yd--;
            }
            else if ( dstend.ms >= DAY_MILLISEC ) {
                dstend.ms -= DAY_MILLISEC;
                dstend.yd++;
            }

            /*
             * Set year field of dstend so that unnecessary calls to cvtdate()
             * may be avoided.
             */
            dstend.yr = year;
        }

        return;
}

int _isindst (struct tm *tb)
{
  long ms;

  if ( _daylight == 0 )
    return 0;

  /*
   * Compute (recompute) the transition dates for daylight saving time
   * if necessary.The yr (year) fields of dststart and dstend is
   * compared to the year of interest to determine necessity.
   */
  if ( (tb->tm_year != dststart.yr) || (tb->tm_year != dstend.yr) ) {
    if ( tzapiused ) {
      /*
       * Convert the start of daylight saving time to dststart.
       */
      if ( tzinfo.DaylightDate.wYear == 0 )
	cvtdate( 1,
		 1,             /* day-in-month format */
		 tb->tm_year,
		 tzinfo.DaylightDate.wMonth,
		 tzinfo.DaylightDate.wDay,
		 tzinfo.DaylightDate.wDayOfWeek,
		 0,
		 tzinfo.DaylightDate.wHour,
		 tzinfo.DaylightDate.wMinute,
		 tzinfo.DaylightDate.wSecond,
		 tzinfo.DaylightDate.wMilliseconds );
      else
	cvtdate( 1,
		 0,             /* absolute date */
		 tb->tm_year,
		 tzinfo.DaylightDate.wMonth,
		 0,
		 0,
		 tzinfo.DaylightDate.wDay,
		 tzinfo.DaylightDate.wHour,
		 tzinfo.DaylightDate.wMinute,
		 tzinfo.DaylightDate.wSecond,
		 tzinfo.DaylightDate.wMilliseconds );
      /*
       * Convert start of standard time to dstend.
       */
      if ( tzinfo.StandardDate.wYear == 0 )
	cvtdate( 0,
		 1,             /* day-in-month format */
		 tb->tm_year,
		 tzinfo.StandardDate.wMonth,
		 tzinfo.StandardDate.wDay,
		 tzinfo.StandardDate.wDayOfWeek,
		 0,
		 tzinfo.StandardDate.wHour,
		 tzinfo.StandardDate.wMinute,
		 tzinfo.StandardDate.wSecond,
		 tzinfo.StandardDate.wMilliseconds );
      else
	cvtdate( 0,
		 0,             /* absolute date */
		 tb->tm_year,
		 tzinfo.StandardDate.wMonth,
		 0,
		 0,
		 tzinfo.StandardDate.wDay,
		 tzinfo.StandardDate.wHour,
		 tzinfo.StandardDate.wMinute,
		 tzinfo.StandardDate.wSecond,
		 tzinfo.StandardDate.wMilliseconds );

    }
    else {
      /*
       * GetTimeZoneInformation API was NOT used, or failed. USA
       * daylight saving time rules are assumed.
       */
      cvtdate( 1,
	       1,
	       tb->tm_year,
	       4,                 /* April */
	       1,                 /* first... */
	       0,                 /* ...Sunday */
	       0,
	       2,                 /* 02:00 (2 AM) */
	       0,
	       0,
	       0 );

      cvtdate( 0,
	       1,
	       tb->tm_year,
	       10,                /* October */
	       5,                 /* last... */
	       0,                 /* ...Sunday */
	       0,
	       2,                 /* 02:00 (2 AM) */
	       0,
	       0,
	       0 );
    }
  }

  /*
   * Handle simple cases first.
   */
  if ( dststart.yd < dstend.yd ) {
    /*
     * Northern hemisphere ordering
     */
    if ( (tb->tm_yday < dststart.yd) || (tb->tm_yday > dstend.yd) )
      return 0;
    if ( (tb->tm_yday > dststart.yd) && (tb->tm_yday < dstend.yd) )
      return 1;
  }
  else {
    /*
     * Southern hemisphere ordering
     */
    if ( (tb->tm_yday < dstend.yd) || (tb->tm_yday > dststart.yd) )
      return 1;
    if ( (tb->tm_yday > dstend.yd) && (tb->tm_yday < dststart.yd) )
      return 0;
  }

  ms = 1000L * (tb->tm_sec + 60L * tb->tm_min + 3600L * tb->tm_hour);

  if ( tb->tm_yday == dststart.yd ) {
    if ( ms >= dststart.ms )
      return 1;
    else
      return 0;
  }
  else {
    /*
     * tb->tm_yday == dstend.yd
     */
    if ( ms < dstend.ms )
      return 1;
    else
      return 0;
  }
}

struct tm * xcelocaltime (const time_t *ptime)
{
  struct tm *ptm;
  long ltime;

  if ( (long)*ptime < 0L )
    return( NULL );

  __tzset();

  if ( (*ptime > 3 * _DAY_SEC) && (*ptime < LONG_MAX - 3 * _DAY_SEC) ) 
    {
      ltime = (long)*ptime - _timezone;
      ptm = xcegmtime( (time_t *)&ltime );

      if ( _daylight && _isindst( ptm ) ) 
	{
	  ltime -= _dstbias;
	  ptm = xcegmtime( (time_t *)&ltime );
	  ptm->tm_isdst = 1;
	}
    }
  else 
    {
      ptm = xcegmtime( ptime );

      if ( _isindst(ptm) )
	ltime = (long)ptm->tm_sec - (_timezone + _dstbias);
      else
	ltime = (long)ptm->tm_sec - _timezone;
      ptm->tm_sec = (int)(ltime % 60);
      if ( ptm->tm_sec < 0 ) {
	ptm->tm_sec += 60;
	ltime -= 60;
      }

      ltime = (long)ptm->tm_min + ltime/60;
      ptm->tm_min = (int)(ltime % 60);
      if ( ptm->tm_min < 0 ) {
	ptm->tm_min += 60;
	ltime -= 60;
      }

      ltime = (long)ptm->tm_hour + ltime/60;
      ptm->tm_hour = (int)(ltime % 24);
      if ( ptm->tm_hour < 0 ) 
	{
	  ptm->tm_hour += 24;
	  ltime -=24;
	}

      ltime /= 24;

      if ( ltime > 0L ) 
	{
	  ptm->tm_wday = (ptm->tm_wday + ltime) % 7;
	  ptm->tm_mday += ltime;
	  ptm->tm_yday += ltime;
	}
      else if ( ltime < 0L ) 
	{
	  ptm->tm_wday = (ptm->tm_wday + 7 + ltime) % 7;
	  if ( (ptm->tm_mday += ltime) <= 0 ) {
	    ptm->tm_mday += 31;
	    ptm->tm_yday = 364;
	    ptm->tm_mon = 11;
	    ptm->tm_year--;
	  }
	  else 
	    {
	      ptm->tm_yday += ltime;
	    }
	}
    }


  return(ptm);
}

void 
_xcetzset (void)
{
  int defused;
  int negdiff = 0;

  /*
   * Clear the flag indicated whether GetTimeZoneInformation was used.
   */
  tzapiused = 0;

  /*
   * Set year fields of dststart and dstend structures to -1 to ensure
   * they are recomputed as after this
   */
  dststart.yr = dstend.yr = -1;

  /*
   * Fetch the value of the TZ environment variable.
   */
    /*
     * There is no TZ environment variable, try to use the time zone
     * information from the system.
     */

    if ( GetTimeZoneInformation( &tzinfo ) != 0xFFFFFFFF ) {
      /*
       * Note that the API was used.
       */
      tzapiused = 1;

      /*
       * Derive _timezone value from Bias and StandardBias fields.
       */
      _timezone = tzinfo.Bias * 60L;

      if ( tzinfo.StandardDate.wMonth != 0 )
	_timezone += (tzinfo.StandardBias * 60L);

      /*
       * Check to see if there is a daylight time bias. Since the
       * StandardBias has been added into _timezone, it must be
       * compensated for in the value computed for _dstbias.
       */
      if ( (tzinfo.DaylightDate.wMonth != 0) &&
	   (tzinfo.DaylightBias != 0) )
	{
	  _daylight = 1;
	  _dstbias = (tzinfo.DaylightBias - tzinfo.StandardBias) *
	    60L;
	}
      else {
	_daylight = 0;

	/*
	 * Set daylight bias to 0 because GetTimeZoneInformation
	 * may return TIME_ZONE_ID_DAYLIGHT even though there is
	 * no DST (in NT 3.51, just turn off the automatic DST
	 * adjust in the control panel)!
	 */
	_dstbias = 0;
      }

      /*
       * Try to grab the name strings for both the time zone and the
       * daylight zone. Note the wide character strings in tzinfo
       * must be converted to multibyte characters strings. The
       * locale codepage, __lc_codepage, is used for this. Note that
       * if setlocale() with LC_ALL or LC_CTYPE has not been called,
       * then __lc_codepage will be 0 (_CLOCALECP), which is CP_ACP
       * (which means use the host's default ANSI codepage).
       */
      if ( (WideCharToMultiByte( CP_ACP,
				 WC_COMPOSITECHECK |
				 WC_SEPCHARS,
				 tzinfo.StandardName,
				 -1,
				 _tzname[0],
				 63,
				 NULL,
				 &defused ) != 0) &&
	   (!defused) )
	_tzname[0][63] = '\0';
      else
	_tzname[0][0] = '\0';

      if ( (WideCharToMultiByte( CP_ACP,
				 WC_COMPOSITECHECK |
				 WC_SEPCHARS,
				 tzinfo.DaylightName,
				 -1,
				 _tzname[1],
				 63,
				 NULL,
				 &defused ) != 0) &&
	   (!defused) )
	_tzname[1][63] = '\0';
      else
	_tzname[1][0] = '\0';

    }
}




void __tzset(void)
{
  static int first_time = 0;

  if ( !first_time ) {

    if ( !first_time ) {
      _xcetzset();
      first_time++;
    }
  }
}


static time_t 
_make_time_t (struct tm *tb, int ultflag)
{
  long tmptm1, tmptm2, tmptm3;
  struct tm *tbtemp;

  if ( ((tmptm1 = tb->tm_year) < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1) )
    goto err_mktime;

  if ( (tb->tm_mon < 0) || (tb->tm_mon > 11) ) 
    {
      tmptm1 += (tb->tm_mon / 12);

      if ( (tb->tm_mon %= 12) < 0 ) 
	{
	  tb->tm_mon += 12;
	  tmptm1--;
	}

      if ( (tmptm1 < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1) )
	goto err_mktime;
    }

  tmptm2 = _days[tb->tm_mon];

  if ( !(tmptm1 & 3) && (tb->tm_mon > 1) )
    tmptm2++;

  tmptm3 = (tmptm1 - _BASE_YEAR) * 365L + ((tmptm1 - 1L) >> 2)
    - _LEAP_YEAR_ADJUST;

  tmptm3 += tmptm2;

  tmptm1 = tmptm3 + (tmptm2 = (long)(tb->tm_mday));
  if ( ChkAdd(tmptm1, tmptm3, tmptm2) )
    goto err_mktime;

  tmptm2 = tmptm1 * 24L;
  if ( ChkMul(tmptm2, tmptm1, 24L) )
    goto err_mktime;

  tmptm1 = tmptm2 + (tmptm3 = (long)tb->tm_hour);
  if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
    goto err_mktime;

  tmptm2 = tmptm1 * 60L;
  if ( ChkMul(tmptm2, tmptm1, 60L) )
    goto err_mktime;

  tmptm1 = tmptm2 + (tmptm3 = (long)tb->tm_min);
  if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
    goto err_mktime;

  tmptm2 = tmptm1 * 60L;
  if ( ChkMul(tmptm2, tmptm1, 60L) )
    goto err_mktime;

  tmptm1 = tmptm2 + (tmptm3 = (long)tb->tm_sec);
  if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
    goto err_mktime;

  if  ( ultflag ) 
    {
    __tzset();

    tmptm1 += _timezone;

    if ( (tbtemp = xcelocaltime(&tmptm1)) == NULL )
      goto err_mktime;

    if ( (tb->tm_isdst > 0) || ((tb->tm_isdst < 0) &&
				(tbtemp->tm_isdst > 0)) ) 
      {
	tmptm1 += _dstbias;
	tbtemp = xcelocaltime(&tmptm1);
      }
    }
  else 
    {
      if ( (tbtemp = xcegmtime(&tmptm1)) == NULL )
	goto err_mktime;
    }

  *tb = *tbtemp;
  return (time_t)tmptm1;

 err_mktime:
  return (time_t)(-1);
}


time_t 
mktime (struct tm *tb)
{
  return( _make_time_t(tb, 1) );
}


struct tm * localtime (const time_t *ptime)
{
  struct tm *ptm;
  long ltime;

  if ( (long)*ptime < 0L )
    return( NULL );

  __tzset();

  if ( (*ptime > 3 * _DAY_SEC) && (*ptime < LONG_MAX - 3 * _DAY_SEC) ) 
    {
      ltime = (long)*ptime - _timezone;
      ptm = xcegmtime( (time_t *)&ltime );

      if ( _daylight && _isindst( ptm ) ) 
	{
	  ltime -= _dstbias;
	  ptm = xcegmtime( (time_t *)&ltime );
	  ptm->tm_isdst = 1;
	}
    }
  else 
    {
      ptm = xcegmtime( ptime );

      if ( _isindst(ptm) )
	ltime = (long)ptm->tm_sec - (_timezone + _dstbias);
      else
	ltime = (long)ptm->tm_sec - _timezone;
      ptm->tm_sec = (int)(ltime % 60);
      if ( ptm->tm_sec < 0 ) {
	ptm->tm_sec += 60;
	ltime -= 60;
      }

      ltime = (long)ptm->tm_min + ltime/60;
      ptm->tm_min = (int)(ltime % 60);
      if ( ptm->tm_min < 0 ) {
	ptm->tm_min += 60;
	ltime -= 60;
      }

      ltime = (long)ptm->tm_hour + ltime/60;
      ptm->tm_hour = (int)(ltime % 24);
      if ( ptm->tm_hour < 0 ) 
	{
	  ptm->tm_hour += 24;
	  ltime -=24;
	}

      ltime /= 24;

      if ( ltime > 0L ) 
	{
	  ptm->tm_wday = (ptm->tm_wday + ltime) % 7;
	  ptm->tm_mday += ltime;
	  ptm->tm_yday += ltime;
	}
      else if ( ltime < 0L ) 
	{
	  ptm->tm_wday = (ptm->tm_wday + 7 + ltime) % 7;
	  if ( (ptm->tm_mday += ltime) <= 0 ) {
	    ptm->tm_mday += 31;
	    ptm->tm_yday = 364;
	    ptm->tm_mon = 11;
	    ptm->tm_year--;
	  }
	  else 
	    {
	      ptm->tm_yday += ltime;
	    }
	}
    }


  return(ptm);
}


static int dname_len[7] =
{6, 6, 7, 9, 8, 6, 8};

static char * dname[7] =
{"Sunday", "Monday", "Tuesday", "Wednesday",
 "Thursday", "Friday", "Saturday"};

static int mname_len[12] =
{7, 8, 5, 5, 3, 4, 4, 6, 9, 7, 8, 8};

static char * mname[12] =
{"January", "February", "March", "April",
 "May", "June", "July", "August", "September", "October", "November",
 "December"};

size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *tim_p)
{
  size_t count = 0;
  int i;

  for (;;)
    {
      while (*format && *format != '%')
	{
	  if (count < maxsize - 1)
	    s[count++] = *format++;
	  else
	    return 0;
	}

      if (*format == '\0')
	break;

      format++;
      switch (*format)
	{
	case 'a':
	  for (i = 0; i < 3; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      else
		return 0;
	    }
	  break;
	case 'A':
	  for (i = 0; i < dname_len[tim_p->tm_wday]; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      else
		return 0;
	    }
	  break;
	case 'b':
	  for (i = 0; i < 3; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  mname[tim_p->tm_mon][i];
	      else
		return 0;
	    }
	  break;
	case 'B':
	  for (i = 0; i < mname_len[tim_p->tm_mon]; i++)
	    {
	      if (count < maxsize - 1)
		s[count++] =
		  mname[tim_p->tm_mon][i];
	      else
		return 0;
	    }
	  break;
	case 'c':
	  if (count < maxsize - 24)
	    {
	      for (i = 0; i < 3; i++)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      s[count++] = ' ';
	      for (i = 0; i < 3; i++)
		s[count++] =
		  mname[tim_p->tm_mon][i];

	      sprintf (&s[count],
		       " %.2d %2.2d:%2.2d:%2.2d %.4d",
		       tim_p->tm_mday, tim_p->tm_hour,
		       tim_p->tm_min,
		       tim_p->tm_sec, 1900 +
		       tim_p->tm_year);
	      count += 17;
	    }
	  else
	    return 0;
	  break;
	case 'd':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%.2d",
		       tim_p->tm_mday);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'H':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       tim_p->tm_hour);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'I':
	  if (count < maxsize - 2)
	    {
	      if (tim_p->tm_hour == 0 ||
		  tim_p->tm_hour == 12)
		{
		  s[count++] = '1';
		  s[count++] = '2';
		}
	      else
		{
		  sprintf (&s[count], "%.2d",
			   tim_p->tm_hour % 12);
		  count += 2;
		}
	    }
	  else
	    return 0;
	  break;
	case 'j':
	  if (count < maxsize - 3)
	    {
	      sprintf (&s[count], "%.3d",
		       tim_p->tm_yday + 1);
	      count += 3;
	    }
	  else
	    return 0;
	  break;
	case 'm':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%.2d",
		       tim_p->tm_mon + 1);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'M':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       tim_p->tm_min);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'p':
	  if (count < maxsize - 2)
	    {
	      if (tim_p->tm_hour < 12)
		s[count++] = 'A';
	      else
		s[count++] = 'P';

	      s[count++] = 'M';
	    }
	  else
	    return 0;
	  break;
	case 'S':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       tim_p->tm_sec);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'U':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       (tim_p->tm_yday + 7 -
			tim_p->tm_wday) / 7);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'w':
	  if (count < maxsize - 1)
	    {
	      sprintf (&s[count], "%1.1d",
		       tim_p->tm_wday);
	      count++;
	    }
	  else
	    return 0;
	  break;
	case 'W':
	  if (count < maxsize - 2)
	    {
	      sprintf (&s[count], "%2.2d",
		       (tim_p->tm_yday + ((8 -
					   tim_p->tm_wday) % 7)) / 7);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'x':
	  if (count < maxsize - 15)
	    {
	      for (i = 0; i < 3; i++)
		s[count++] =
		  dname[tim_p->tm_wday][i];
	      s[count++] = ' ';
	      for (i = 0; i < 3; i++)
		s[count++] =
		  mname[tim_p->tm_mon][i];

	      sprintf (&s[count],
		       " %.2d %.4d", tim_p->tm_mday,
		       1900 + tim_p->tm_year);
	      count += 8;
	    }
	  else
	    return 0;
	  break;
	case 'X':
	  if (count < maxsize - 8)
	    {
	      sprintf (&s[count],
		       "%2.2d:%2.2d:%2.2d",
		       tim_p->tm_hour, tim_p->tm_min,
		       tim_p->tm_sec);
	      count += 8;
	    }
	  else
	    return 0;
	  break;
	case 'y':
	  if (count < maxsize - 2)
	    {
	      /* The year could be greater than 100, so we need the value
		 modulo 100.  The year could be negative, so we need to
		 correct for a possible negative remainder.  */
	      sprintf (&s[count], "%2.2d",
		       (tim_p->tm_year % 100 + 100) % 100);
	      count += 2;
	    }
	  else
	    return 0;
	  break;
	case 'Y':
	  if (count < maxsize - 4)
	    {
	      sprintf (&s[count], "%.4d",
		       1900 + tim_p->tm_year);
	      count += 4;
	    }
	  else
	    return 0;
	  break;
	case 'Z':
	  break;
	case '%':
	  if (count < maxsize - 1)
	    s[count++] = '%';
	  else
	    return 0;
	  break;
	}
      if (*format)
	format++;
      else
	break;
    }
  s[count] = '\0';

  return count;
}

double copysign(double x, double y)
{
	return _copysign(x, y);
}


#define TIMESPEC_TO_FILETIME_OFFSET (((LONGLONG)27111902 << 32) + (LONGLONG)3577643008)
void __cdecl ftime(struct timeb *tp)
{
	FILETIME ft;
	SYSTEMTIME st;
	int val;

	GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);

	val = (int) ((*(LONGLONG *) &ft - TIMESPEC_TO_FILETIME_OFFSET) / 10000000);
	tp->time = (unsigned int) val;
	val = (int) ((*(LONGLONG *) &ft - TIMESPEC_TO_FILETIME_OFFSET - ((LONGLONG) val * (LONGLONG) 10000000)) * 100);
	tp->millitm = (unsigned int) val/1000;
}

BOOL WINAPI _DllMainCRTStartup(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}
	