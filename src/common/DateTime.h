#pragma once

#include <chrono>
#include <string>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class DateTime
{
public:
	// %H:%M:%S, empty zone is UTC time
	static std::chrono::system_clock::time_point convertStr2DayTime(const std::string &strTime, const std::string &zone);
	static std::chrono::system_clock::time_point convertStr2DayTime(const std::string &strTime);
	static std::string convertDayTime2Str(const std::chrono::system_clock::time_point &time);
	// +08:00:00
	static const std::string getLocalUtcOffset();
	// compared with local host
	static std::chrono::seconds getPosixZoneDiff(const std::string &targetZone, const std::string &hostZone);
	static std::chrono::system_clock::time_point parseISO8601DateTime(const std::string &strTime);
	static std::chrono::system_clock::time_point parseISO8601DateTime(const std::string &strTime, const std::string &posixTimeZone);
	// output 2017-09-11T21:52:13+00:00 in local time with offset
	static std::string formatISO8601Time(const std::chrono::system_clock::time_point &time);
	// output 2019-01-23T10:18:32.079Z in UTC
	static std::string formatRFC3339Time(const std::chrono::system_clock::time_point &time);
	static std::string formatLocalTime(const std::chrono::system_clock::time_point &time, const char *fmt);

	// Convert target zone time to current zone
	static std::chrono::system_clock::time_point convertToZoneTime(boost::posix_time::ptime &dst, const std::string &posixTimezone);

	// +08:00
	static std::string getISO8601TimeZone(const std::string &strTime);

	static boost::posix_time::time_duration getDayTimeDuration(const std::chrono::system_clock::time_point &time);
	static boost::posix_time::time_duration parseDayTimeDuration(const std::string &strTime, const std::string &posixTimezone);
};

// https://stackoverflow.com/questions/8746848/boost-get-the-current-local-date-time-with-current-time-zone-from-the-machine
class machine_time_zone : public boost::local_time::custom_time_zone
{
	typedef boost::local_time::custom_time_zone base_type;
	typedef base_type::time_duration_type time_duration_type;

public:
	machine_time_zone();
	// This method is not precise, real offset may be several seconds more or less.
	static const boost::posix_time::time_duration &GetUTCOffset();
};
