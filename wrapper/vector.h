#pragma once

#include <lxvmath.h>
#include <lxu_vector.hpp>

#include <functional>
#include <numeric>
#include <type_traits>

template<typename TVec>
struct TVector
{
	TVec Vec;

	using TType = decltype(+Vec[0]);

	template <typename TVecOther>
	explicit TVector(const TVector<TVecOther>& other);
	TVector(TType x, TType y, TType z);
	TVector(const CLxVector& other);
	TVector(const CLxFVector& other);

	TVector() = default;

	TVector& max(const TVector& other);
	TVector& min(const TVector& other);
	TVector& operator/=(const TVector& other);
	TVector& operator*=(const TVector& other);
	TVector& operator-=(const TVector& other);
	TVector& operator+=(const TVector& other);
	TVector& operator*=(TType x) {
		LXx_VSCL(Vec, x);
		return *this;
	}
	TVector& operator/=(TType x) {
		*this *= (1 / x);
		return *this;
	}
	TVector& operator=(const TVector& other);
	bool operator==(const TVector& other) const;
	bool operator!=(const TVector& other) const;
	TVector& clear();
	bool empty() const;

	operator TType* () {
		return Vec;
	}

	operator const TType* () const {
		return Vec;
	}

	TType len()  {
		return LXx_VLEN(Vec);
	}
	TVector& normalized() {
		*this /= len();
		return *this;
	}

	TVector operator-() const;
};

using TVectorF = TVector<LXtFVector>;
using TVectorD = TVector<LXtVector>;

struct TColor : public TVectorD {
	static TColor White() { 
		TColor c; 
		c.Vec[0] = 1.0;
		c.Vec[1] = 1.0;
		c.Vec[2] = 1.0;
		return c;
	}

	static TColor Green() {
		TColor c;
		c.Vec[0] = 0.0;
		c.Vec[1] = 1.0;
		c.Vec[2] = 0.0;
		return c;
	}

	static TColor Red() {
		TColor c;
		c.Vec[0] = 1.0;
		c.Vec[1] = 0.0;
		c.Vec[2] = 0.0;
		return c;
	}

	static TColor Yellow() {
		TColor c;
		c.Vec[0] = 1.0;
		c.Vec[1] = 1.0;
		c.Vec[2] = 0.0;
		return c;
	}

	static TColor Blue() {
		TColor c;
		c.Vec[0] = 0.0;
		c.Vec[1] = 0.0;
		c.Vec[2] = 1.0;
		return c;
	}

	static TColor Purple() {
		TColor c;
		c.Vec[0] = 1.0;
		c.Vec[1] = 0.0;
		c.Vec[2] = 1.0;
		return c;
	}
};


std::ostream& operator<<(std::ostream& os, const TVectorF& v);
std::ostream& operator<<(std::ostream& os, const TVectorD& v);

template<typename TVec>
TVector<TVec> operator+(const TVector<TVec>& left, const TVector<TVec>& right);

template<typename TVec>
TVector<TVec> operator-(const TVector<TVec>& left, const TVector<TVec>& right);

/*
template<typename TVec>
TVector<TVec> operator*(typename TVector<TVec>::TType left, const TVector<TVec>& right);


template<typename TVec>
TVector<TVec> operator*(const TVector<TVec>& left, typename TVector<TVec>::TType right);
*/

template<typename TVec>
TVector<TVec> cross(const TVector<TVec>& a, const TVector<TVec>& b);

template<typename TVec>
typename TVector<TVec>::TType dot(const TVector<TVec>& a, const TVector<TVec>& b);

template<typename TVec>
TVector<TVec> project(const TVector<TVec>& p, const TVector<TVec>& proj);

template<typename TVec, typename T>
TVec AverageNormals(const T& container) {
	auto zero = TVec().clear();
	auto result = std::accumulate(begin(container), end(container), zero);
	return result.normalized();
}

template<typename TVec>
inline TVector<TVec>::TVector(TType x, TType y, TType z)
	: Vec{ x, y, z }
{
}

template<typename TVec>
inline TVector<TVec>::TVector(const CLxVector& other)
	: Vec{(TType)other.v[0], (TType)other.v[1], (TType)other.v[2]}
{
}

template<typename TVec>
inline TVector<TVec>::TVector(const CLxFVector& other)
	: Vec{ (TType)other.v[0], (TType)other.v[1], (TType)other.v[2] }
{
}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::max(const TVector& other)
{
	LXx_VBOP(Vec, other.Vec, LXxMAX);
	return *this;
}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::min(const TVector& other)
{
	LXx_VBOP(Vec, other.Vec, LXxMIN);
	return *this;
}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::operator/=(const TVector& other)
{
	LXx_VDIV(Vec, other.Vec);
	return *this;
}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::operator*=(const TVector& other)
{
	LXx_VMUL(Vec, other.Vec);
	return *this;
}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::operator-=(const TVector& other)
{
	LXx_VSUB(Vec, other.Vec);
	return *this;
}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::operator+=(const TVector& other)
{
	LXx_VADD(Vec, other.Vec);
	return *this;

}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::operator=(const TVector& other)
{
	LXx_VCPY(Vec, other.Vec);
	return *this;
}

template<typename TVec>
inline bool TVector<TVec>::operator==(const TVector& other) const
{
	return LXx_VEQ(Vec, other.Vec);
}

template<typename TVec>
inline bool TVector<TVec>::operator!=(const TVector& other) const
{
	return !(*this == other);
}

template<typename TVec>
inline TVector<TVec>& TVector<TVec>::clear()
{
	LXx_VCLR(Vec);
	return *this;
}

template<typename TVec>
inline bool TVector<TVec>::empty() const
{
	return LXx_VEQS(Vec, 0.0);
}

template<typename TVec>
inline TVector<TVec> TVector<TVec>::operator-() const
{
	auto tmp = *this;
	LXx_VNEG(tmp.Vec);
	return tmp;
}

template<typename TVec>
inline TVector<TVec> operator+(const TVector<TVec>& left, const TVector<TVec>& right)
{
	auto tmp = left;
	tmp += right;
	return tmp;
}

template<typename TVec>
inline TVector<TVec> operator-(const TVector<TVec>& left, const TVector<TVec>& right)
{
	auto tmp = left;
	tmp -= right;
	return tmp;
}

/*
template<typename TVec>
inline TVector<TVec> operator*(typename TVector<TVec>::TType left, const TVector<TVec>& right)
{
	auto tmp = right;
	right *= left;
	return tmp;
}
*/

template<typename TVec>
inline TVector<TVec> operator*(const TVector<TVec>& left, typename TVector<TVec>::TType right)
{
	auto tmp = left;
	tmp *= right;
	return tmp;
}

template<typename TVec>
inline TVector<TVec> cross(const TVector<TVec>& a, const TVector<TVec>& b)
{
	TVector<TVec> res;

	LXx_VCROSS(res, a.Vec, b.Vec);
	return res;
}

template<typename TVec>
inline typename TVector<TVec>::TType dot(const TVector<TVec>& a, const TVector<TVec>& b)
{
	return LXx_VDOT(a.Vec, b.Vec);
}

template<typename TVec>
inline TVector<TVec> project(const TVector<TVec>& p, const TVector<TVec>& proj)
{
	if (proj.empty()) {
		return TVector<TVec>().clear();
	}

	const float mul = dot(p, proj) / dot(proj, proj);
	return proj * mul;
}

template<typename TVec>
inline TVector<TVec> operator*(typename TVector<TVec>::TType left, const TVector<TVec>& right)
{
	auto tmp = right;
	tmp *= left;
	return tmp;
}

template<typename TVec>
inline TVector<TVec> operator/(const TVector<TVec>& left, typename TVector<TVec>::TType right)
{
	auto tmp = left;
	tmp /= right;
	return tmp;
}

template<typename TVec>
template<typename TVecOther>
inline TVector<TVec>::TVector(const TVector<TVecOther>& other)
{
	LXx_VCPY(Vec, other.Vec);
}
