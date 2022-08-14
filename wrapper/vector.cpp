#include "vector.h"
#include "log.h"

std::ostream& operator<<(std::ostream& os, const TVectorF& v)
{
	os << "(" << v.Vec[0] << ", " << v.Vec[1] << ", " << v.Vec[2] << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const TVectorD& v)
{
	os << "(" << v.Vec[0] << ", " << v.Vec[1] << ", " << v.Vec[2] << ")";
	return os;
}