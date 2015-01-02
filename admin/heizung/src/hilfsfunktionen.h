#define ARRAYSIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static struct timespec time_add(struct timespec time, long milliseconds)
{
	time.tv_sec += milliseconds / 1000;
	time.tv_nsec += (milliseconds % 1000) * 1000000;
	if(time.tv_nsec >= 1000000000) {
		time.tv_sec += 1;
		time.tv_nsec -= 1000000000;
	}
	return time;
}
static struct timespec time_diff(struct timespec start, struct timespec end)
{
	end.tv_sec -= start.tv_sec;
	end.tv_nsec -= start.tv_nsec;
	if(end.tv_nsec < 0) {
		end.tv_sec -= 1;
		end.tv_nsec += 1000000000;
	}
	return end;
}

// Viessmann sendet teilweise Daten im BCD-Code
static inline int b2d(char byte)
{
	div_t retVal = div(byte, 16);
	return 10 * retVal.quot + retVal.rem;
}
static inline int_fast8_t b2c(const char bytes[])
{
	return (int8_t)*bytes;
}
static inline int_fast16_t b2s(const char bytes[])
{
	return (int16_t)le16toh(*((uint16_t *)bytes));
}
static inline int_fast32_t b2l(const char bytes[])
{
	return (int32_t)le32toh(*((uint32_t *)bytes));
}
static inline int_fast64_t b2ll(const char bytes[])
{
	return (int64_t)le64toh(*((uint64_t *)bytes));
}
static time_t parseTime(const char bytes[])
{
	struct tm time = {
			.tm_sec = b2d(bytes[7]),
			.tm_min = b2d(bytes[6]),
			.tm_hour = b2d(bytes[5]),
			.tm_mday = b2d(bytes[3]),
			.tm_mon = b2d(bytes[2]) - 1,
			.tm_year = b2d(bytes[0]) * 100 + b2d(bytes[1]) - 1900,
			.tm_wday = b2d(bytes[4]) % 7,
			.tm_isdst = -1
			};
	return mktime(&time);
}
static size_t convertTemp2(const char bytes[], char *string, size_t maxlen)
{
	return snprintf(string, maxlen, "%0.1f", b2s(bytes) / 10.0);
}
static size_t convertBool(const char bytes[], char *string, size_t maxlen)
{
	return snprintf(string, maxlen, "%" PRIdFAST8, b2c(bytes));
}
static size_t convertLong(const char bytes[], char *string, size_t maxlen)
{
	return snprintf(string, maxlen, "%" PRIdFAST32, b2l(bytes));
}
static size_t convertTime(const char bytes[], char *string, size_t maxlen)
{
	return snprintf(string, maxlen, "%" PRIu64, (uint64_t)parseTime(bytes));
}
