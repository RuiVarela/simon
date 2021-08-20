#pragma once

#include "Project.h"

namespace re
{
    constexpr Real pi_angle = (Real)3.14159265358979323846;
    constexpr Real pi_div_2_angle = (Real)1.57079632679489661923;
    constexpr Real pi_div_4_angle = (Real)0.78539816339744830962;
    constexpr Real pi_mul_2_angle = (Real)2.0 * pi_angle;
    constexpr Real pi_div_180 = (Real)0.017453292519943296;
    constexpr Real pi_inverse_div_180 = (Real)57.2957795130823229;
    constexpr Real epsilon = (Real)1e-6;

	template<typename T>
	RENGINE_INLINE T minimum(T const left, T const right) { return ((left < right) ? left: right); }

	template<typename T>
	RENGINE_INLINE T maximum(T const left, T const right) { return ((left > right) ? left : right); }

	template<typename T>
	RENGINE_INLINE T absolute(T const value) { return ((value < T(0.0)) ? -value : value); }

	template<typename T>
	RENGINE_INLINE Bool equivalent(T const left, T const right, T const epsilon = 1e-6) { T delta = right - left; return (delta < T(0.0)) ? (delta >= -epsilon) : (delta <= epsilon); }

	template<typename T>
	RENGINE_INLINE T sign(T const value) { return ((value < T(0.0)) ? T(-1.0) : ((value == T(0.0)) ? T(0.0) : T(1.0)) ); }

	template<typename T>
	RENGINE_INLINE T square(T const value) { return value * value; }

	template<typename T>
	RENGINE_INLINE T clampNear(T const value, T const target, T const epsilon = 1e-6) { return (equivalent(value, target, epsilon) ? target : value); }

	template<typename T>
	RENGINE_INLINE T clampTo(T const value, T const minimum, T const maximum) { return ((value < minimum) ? minimum : ((value > maximum) ? maximum : value) ); }

	template<typename T>
	RENGINE_INLINE T clampAbove(T const value, T const minimum) { return ((value < minimum) ? minimum : value); }

	template<typename T>
	RENGINE_INLINE T clampBelow(T const value, T const maximum) { return ((value > maximum) ? maximum : value); }

	template<typename T>
	RENGINE_INLINE T signedSquare(T const value) { return ((value < T(0.0)) ? -value * value : value * value); }

	template<typename T>
	RENGINE_INLINE T radiansToDegrees(T const radians) { return radians * T(pi_inverse_div_180); }

	template<typename T>
	RENGINE_INLINE T degreesToRadians(T const degrees) { return degrees * T(pi_div_180); }

	template<typename T>
	RENGINE_INLINE T round(T const value) { return ( (value >= T(0.0)) ? floor(value + T(0.5)) : ceil(value - T(0.5)) ); }

	template<typename T>
	RENGINE_INLINE T isNAN(T const value) { return std::isnan(value); }

	template<typename T>
	RENGINE_INLINE void fastSwap(T& left, T& right, T& tmp) { tmp = left; left = right; right = tmp; }

	template<typename T>
	RENGINE_INLINE void slowSwap(T& left, T& right) { T tmp = left; left = right; right = tmp; }

	template<typename T>
	RENGINE_INLINE T sinc(T const& value)
	{
		if (absolute(value) < epsilon)
		{
			return T(1.0) + value * value * ( T(-1.0) / T(6.0) + value * value * T(1.0) / T(120.0));
		}
		else
		{
			return sin(value) / value;
		}
	}

    inline bool isValidNumber(float v) { return !isnan(v) && !isinf(v);  }


} // namespace rengine

