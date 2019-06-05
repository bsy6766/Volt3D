#ifndef V3D_TIMER_H
#define V3D_TIMER_H

#include <chrono>
#include <string>
#include <sstream>
#include <ctime>

namespace v3d
{
	/**
	*	@brief A simple static functions that measures time.
	*
	*	@note To get current time of application, use GLView's time.
	*/
	namespace Timer
	{
		enum class Unit
		{
			SECONDS = 0,
			MILLISECONDS,
			MICROSECONDS,
			NANOSECONDS,
		};

		// Get current time
		static std::chrono::steady_clock::time_point now()
		{
			return std::chrono::steady_clock::now();
		}

		// Convert time from start to end into seconds string
		static std::string toSecondString(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end, const bool addUnit = true)
		{
			std::stringstream ss;
			ss << std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
			if (addUnit)
				return ss.str() + " s";
			else
				return ss.str();
		}

		// Convert time from start to end into milli seconds string
		static std::string toMilliSecondString(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end, const bool addUnit = true)
		{
			std::stringstream ss;
			ss << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			if (addUnit)
				return ss.str() + " ms";
			else
				return ss.str();
		}

		// Convert time from start to end into micro seconds string
		static std::string toMicroSecondString(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end, const bool addUnit = true)
		{
			std::stringstream ss;
			ss << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			if (addUnit)
				return ss.str() + " us";
			else
				return ss.str();
		}

		// Convert time from start to end into nano seconds string
		static std::string toNanoSecondString(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end, const bool addUnit = true)
		{
			std::stringstream ss;
			ss << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
			if (addUnit)
				return ss.str() + " ns";
			else
				return ss.str();
		}

		// Convert time from start to end into string
		static std::string toString(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end, const v3d::Timer::Unit unit, const bool addUnit = true)
		{
			switch (unit)
			{
			case v3d::Timer::Unit::MILLISECONDS:
				return v3d::Timer::toMilliSecondString(start, end, addUnit);
				break;
			case v3d::Timer::Unit::MICROSECONDS:
				return v3d::Timer::toMicroSecondString(start, end, addUnit);
				break;
			case v3d::Timer::Unit::NANOSECONDS:
				return v3d::Timer::toNanoSecondString(start, end, addUnit);
				break;
			case v3d::Timer::Unit::SECONDS:
			default:
				break;
			}

			return v3d::Timer::toSecondString(start, end, addUnit);
		}

		// Convert time from start to end into seconds
		static long long toSeconds(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end)
		{
			return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		}

		// Convert time from start to end into milliseconds
		static long long toMilliSeconds(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end)
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		}

		// Convert time from start to end into microseconds
		static long long toMicroSeconds(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end)
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		}

		// Convert time from start to end into seconds
		static long long toNanoSeconds(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end)
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		}

		// Convert time from start to end into duration
		static long long toDuration(const std::chrono::steady_clock::time_point start, const std::chrono::steady_clock::time_point end, const v3d::Timer::Unit unit)
		{
			switch (unit)
			{
			case v3d::Timer::Unit::MILLISECONDS:
				return v3d::Timer::toMilliSeconds(start, end);
				break;
			case v3d::Timer::Unit::MICROSECONDS:
				return v3d::Timer::toMicroSeconds(start, end);
				break;
			case v3d::Timer::Unit::NANOSECONDS:
				return v3d::Timer::toNanoSeconds(start, end);
				break;
			case v3d::Timer::Unit::SECONDS:
			default:
				break;
			}

			return v3d::Timer::toSeconds(start, end);
		}

		// Get current date and time in string
		static std::string getDateAndTime(const std::string& format = "%Y-%m-%d %H:%M:%S")
		{
			const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			char date[100] = { 0 };
			struct tm buf;
			localtime_s(&buf, &now);
			std::strftime(date, sizeof(date), format.c_str(), &buf);
			return std::string(date);
		}
	};
}

#endif