// 
//   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "log.h"
#include "Date.h"
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>

namespace gnash {

Date::Date() {
}

Date::~Date() {
}

void
Date::getTime()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getTimezoneOffset()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCDate()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCDay()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCFullYear()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCHours()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCMilliseconds()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCMinutes()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCMonth()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getUTCSeconds()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::getYear()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setDate()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setFullYear()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setHours()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setMilliseconds()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setMinutes()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setMonth()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setSeconds()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setTime()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setUTCDate()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setUTCFullYear()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setUTCHours()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setUTCMilliseconds()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setUTCMinutes()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setUTCMonth()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setUTCSeconds()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::setYear()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::toString()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

void
Date::UTC()
{
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

tm
Date::convertUTC()
{
	tm utctime;

	utctime.tm_sec = second;
	utctime.tm_min = minute;
	utctime.tm_hour = hour;
	utctime.tm_mday = date;
	utctime.tm_mon = month;
	utctime.tm_year = year;
	utctime.tm_wday = dayWeek;
	utctime.tm_yday = dayYear;
	utctime.tm_isdst = isDST;

	time_t normalized;

	normalized = mktime(&utctime);

	tm *result = gmtime(&normalized);

	return *result;
}

void
date_new(const fn_call& fn)
{
	date_as_object *date_obj = new date_as_object;

	date_obj->set_member("getdate", &date_getdate);
	date_obj->set_member("getday", &date_getday);
	date_obj->set_member("getfullyear", &date_getfullyear);
	date_obj->set_member("gethours", &date_gethours);
	date_obj->set_member("getmilliseconds", &date_getmilliseconds);
	date_obj->set_member("getminutes", &date_getminutes);
	date_obj->set_member("getmonth", &date_getmonth);
	date_obj->set_member("getseconds", &date_getseconds);
	date_obj->set_member("gettime", &date_gettime);
	date_obj->set_member("gettimezoneoffset", &date_gettimezoneoffset);
	date_obj->set_member("getutcdate", &date_getutcdate);
	date_obj->set_member("getutcday", &date_getutcday);
	date_obj->set_member("getutcfullyear", &date_getutcfullyear);
	date_obj->set_member("getutchours", &date_getutchours);
	date_obj->set_member("getutcmilliseconds", &date_getutcmilliseconds);
	date_obj->set_member("getutcminutes", &date_getutcminutes);
	date_obj->set_member("getutcmonth", &date_getutcmonth);
	date_obj->set_member("getutcseconds", &date_getutcseconds);
	date_obj->set_member("getyear", &date_getyear);
	date_obj->set_member("setdate", &date_setdate);
	date_obj->set_member("setfullyear", &date_setfullyear);
	date_obj->set_member("sethours", &date_sethours);
	date_obj->set_member("setmilliseconds", &date_setmilliseconds);
	date_obj->set_member("setminutes", &date_setminutes);
	date_obj->set_member("setmonth", &date_setmonth);
	date_obj->set_member("setseconds", &date_setseconds);
	date_obj->set_member("settime", &date_settime);
	date_obj->set_member("setutcdate", &date_setutcdate);
	date_obj->set_member("setutcfullyear", &date_setutcfullyear);
	date_obj->set_member("setutchours", &date_setutchours);
	date_obj->set_member("setutcmilliseconds", &date_setutcmilliseconds);
	date_obj->set_member("setutcminutes", &date_setutcminutes);
	date_obj->set_member("setutcmonth", &date_setutcmonth);
	date_obj->set_member("setutcseconds", &date_setutcseconds);
	date_obj->set_member("setyear", &date_setyear);
	date_obj->set_member("tostring", &date_tostring);
	date_obj->set_member("utc", &date_utc);

	struct tm *ti;
	if (fn.nargs == 0) {
#ifndef HAVE_GETTIMEOFDAY
		struct timeb tb;
		
		ftime (&tb);
		ti = localtime(&tb.time); 
		log_error("date_new constructor doesn't set timezone or milliseconds on your system - using defaults\n");
		date_obj->obj.millisecond = 0;
		date_obj->obj.minutesEast = 0;
#else		
		struct timeval tEnd;
		struct timezone tZone;
		gettimeofday(&tEnd,&tZone);
		date_obj->obj.millisecond = tEnd.tv_usec;
		date_obj->obj.minutesEast = -tZone.tz_minuteswest;
		time_t t = time(&t);
		ti = localtime(&t);
#endif
		date_obj->obj.second = ti->tm_sec;
		date_obj->obj.minute = ti->tm_min;
		date_obj->obj.hour = ti->tm_hour;
		date_obj->obj.date = ti->tm_mday;
		date_obj->obj.month = ti->tm_mon;
		date_obj->obj.year = ti->tm_year;
		date_obj->obj.dayWeek = ti->tm_wday;
		date_obj->obj.dayYear = ti->tm_yday;
		date_obj->obj.isDST = ti->tm_isdst;
	}
	else
		log_error("date_new constructor with %d arguments unimplemented!\n",fn.nargs);

	fn.result->set_as_object(date_obj);
}
void date_getdate(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.date);
}
void date_getday(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.dayWeek);
}
void date_getfullyear(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.year + 1900);
}
void date_gethours(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.hour);
}
void date_getmilliseconds(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.millisecond);
}
void date_getminutes(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.minute);
}
void date_getmonth(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.month);
}
void date_getseconds(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.second);
}
void date_gettime(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_gettimezoneoffset(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.minutesEast);
}
void date_getutcdate(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	tm result = date->obj.convertUTC();

	fn.result->set_int(int(result.tm_mday));
}
void date_getutcday(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	tm result = date->obj.convertUTC();

	fn.result->set_int(int(result.tm_wday));
}
void date_getutcfullyear(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	tm result = date->obj.convertUTC();

	fn.result->set_int(int(result.tm_year)+1900);
}
void date_getutchours(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	tm result = date->obj.convertUTC();

	fn.result->set_int(int(result.tm_hour));
}
void date_getutcmilliseconds(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	// Milliseconds (value between 0 and 999) won't be affected by timezone
	fn.result->set_int(int(date->obj.millisecond));
}
void date_getutcminutes(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	tm result = date->obj.convertUTC();

	fn.result->set_int(int(result.tm_min));
}
void date_getutcmonth(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	tm result = date->obj.convertUTC();

	fn.result->set_int(int(result.tm_mon));
}
void date_getutcseconds(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	tm result = date->obj.convertUTC();

	fn.result->set_int(int(result.tm_sec));
}
void date_getyear(const fn_call& fn) {
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	fn.result->set_int(date->obj.year);
}
// TODO: for all these "set" functions, what do we do if sent illegal values?
// Clamp them to a proper range? Ignore and keep previous value? Throw an error?
// Right now we're doing _none_ of these, because I don't know what's appropriate!
void date_setdate(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.date = (long int)(fn.arg(0).to_number());
}
void date_setfullyear(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.year = (long int)(fn.arg(0).to_number() - 1900);
}
void date_sethours(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.hour = (long int)(fn.arg(0).to_number());
}
void date_setmilliseconds(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.millisecond = (long int)(fn.arg(0).to_number());
}
void date_setminutes(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.minute = (long int)(fn.arg(0).to_number());
}
void date_setmonth(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.month = (long int)(fn.arg(0).to_number());
}
void date_setseconds(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.second = (long int)(fn.arg(0).to_number());
}
void date_settime(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setutcdate(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setutcfullyear(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setutchours(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setutcmilliseconds(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setutcminutes(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setutcmonth(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setutcseconds(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}
void date_setyear(const fn_call& fn) {
	assert(fn.nargs == 1);
	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;
	date->obj.year = (long int)(fn.arg(0).to_number());
}
void date_tostring(const fn_call& fn) {
	// TODO: I have no idea what the real flash player does, but at least this
	// gives something functional for now. Tried to mimic the "date" program
	char buffer[32];
	char* monthname[12] =
		{"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	char* dayweekname[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

	date_as_object* date = (date_as_object*) (as_object*) fn.this_ptr;

	snprintf((char *)&buffer,128,"%s %s %2ld %.2ld:%.2ld:%.2ld %ld",
		dayweekname[date->obj.dayWeek],monthname[date->obj.month],
		date->obj.date,date->obj.hour,date->obj.minute,date->obj.second,
		1900+date->obj.year);

	fn.result->set_string((char *)&buffer);
}
void date_utc(const fn_call& fn) {
	log_msg("%s:unimplemented \n", __FUNCTION__);
}

} // end of gnaash namespace

