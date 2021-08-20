#include "Project.h"

using namespace re;

/**
 * Easing equation function for a simple linear tweening, with no easing.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeNone(Real progress)
{
    return progress;
}
/**
 * Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInQuad(Real t)
{
    return t*t;
}
/**
* Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
*
* @param t		Current time (in frames or seconds).
* @return		The correct value.
*/
static Real easeOutQuad(Real t)
{
    return -t*(t-2);
}
/**
 * Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInOutQuad(Real t)
{
    t*=2.0;
    if (t < 1) {
        return t*t/Real(2);
    } else {
        --t;
        return -Real(0.5) * (t*(t-2) - 1);
    }
}
/**
 * Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutInQuad(Real t)
{
    if (t < 0.5) return easeOutQuad (t*2)/2;
    return easeInQuad((2*t)-1)/2 + Real(0.5);
}
/**
 * Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInCubic(Real t)
{
    return t*t*t;
}
/**
 * Easing equation function for a cubic (t^3) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutCubic(Real t)
{
    t-=1.0;
    return t*t*t + 1;
}
/**
 * Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInOutCubic(Real t)
{
    t*=2.0;
    if(t < 1) {
        return Real(0.5)*t*t*t;
    } else {
        t -= Real(2.0);
        return Real(0.5)*(t*t*t + 2);
    }
}
/**
 * Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutInCubic(Real t)
{
    if (t < 0.5) return easeOutCubic (2*t)/2;
    return easeInCubic(2*t - 1)/2 + Real(0.5);
}
/**
 * Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInQuart(Real t)
{
    return t*t*t*t;
}
/**
 * Easing equation function for a quartic (t^4) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutQuart(Real t)
{
    t-= Real(1.0);
    return - (t*t*t*t- 1);
}
/**
 * Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInOutQuart(Real t)
{
    t*=2;
    if (t < 1) return Real(0.5)*t*t*t*t;
    else {
        t -= 2.0f;
        return -Real(0.5) * (t*t*t*t- 2);
    }
}
/**
 * Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutInQuart(Real t)
{
    if (t < 0.5) return easeOutQuart (2*t)/2;
    return easeInQuart(2*t-1)/2 + 0.5f;
}
/**
 * Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInQuint(Real t)
{
    return t*t*t*t*t;
}
/**
 * Easing equation function for a quintic (t^5) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutQuint(Real t)
{
    t-=1.0;
    return t*t*t*t*t + 1;
}
/**
 * Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInOutQuint(Real t)
{
    t*=2.0;
    if (t < 1) return 0.5f*t*t*t*t*t;
    else {
        t -= 2.0;
        return 0.5f*(t*t*t*t*t + 2);
    }
}
/**
 * Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutInQuint(Real t)
{
    if (t < 0.5) return easeOutQuint (2*t)/2;
    return easeInQuint(2*t - 1)/2 + 0.5f;
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInSine(Real t)
{
    return (t == 1.0f) ? 1.0f : -::cos(t * pi_div_2_angle) + 1.0f;
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutSine(Real t)
{
    return ::sin(t* pi_div_2_angle);
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInOutSine(Real t)
{
    return -0.5f * (::cos(pi_angle*t) - 1);
}
/**
 * Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutInSine(Real t)
{
    if (t < 0.5) return easeOutSine (2*t)/2;
    return easeInSine(2*t - 1)/2 + 0.5f;
}
/**
 * Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInExpo(Real t)
{
    return (t==0 || t == 1.0) ? t : ::pow(2.0f, 10 * (t - 1)) - 0.001f;
}
/**
 * Easing equation function for an exponential (2^t) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutExpo(Real t)
{
    return (t==1.0f) ? 1.0f : 1.001f * (-::pow(2.0f, -10 * t) + 1);
}
/**
 * Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInOutExpo(Real t)
{
    if (t==0.0) return Real(0.0);
    if (t==1.0) return Real(1.0);
    t*=2.0;
    if (t < 1) return 0.5f * ::pow(Real(2.0), 10 * (t - 1)) - 0.0005f;
    return 0.5f * 1.0005f * (-::pow(Real(2.0), -10 * (t - 1)) + 2);
}
/**
 * Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutInExpo(Real t)
{
    if (t < 0.5) return easeOutExpo (2*t)/2;
    return easeInExpo(2*t - 1)/2 + 0.5f;
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInCirc(Real t)
{
    return -(::sqrt(1 - t*t) - 1);
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutCirc(Real t)
{
    t-= Real(1.0);
    return ::sqrt(1 - t* t);
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeInOutCirc(Real t)
{
    t*=Real(2.0);
    if (t < 1) {
        return -0.5f * (::sqrt(1 - t*t) - 1);
    } else {
        t -= Real(2.0);
        return 0.5f * (::sqrt(1 - t*t) + 1);
    }
}
/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static Real easeOutInCirc(Real t)
{
    if (t < 0.5) return easeOutCirc (2*t)/2;
    return easeInCirc(2*t - 1)/2 + 0.5f;
}
static Real easeInElastic_helper(Real t, Real b, Real c, Real d, Real a, Real p)
{
    if (t==0) return b;
    Real t_adj = (Real)t / (Real)d;
    if (t_adj==1) return b+c;
    Real s;
    if(a < ::fabs(c)) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * pi_angle) * ::asin(c / a);
    }
    t_adj -= 1.0f;
    return -(a*::pow(2.0f,10*t_adj) * ::sin( (t_adj*d-s)*(2*pi_angle)/p )) + b;
}
/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static Real easeInElastic(Real t, Real a, Real p)
{
    return easeInElastic_helper(t, 0, 1, 1, a, p);
}

static Real easeOutElastic_helper(Real t, Real /*b*/, Real c, Real /*d*/, Real a, Real p)
{
    if (t==0) return 0;
    if (t==1) return c;
    Real s;
    if(a < c) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * pi_angle) * ::asin(c / a);
    }
    return (a*::pow(2.0f,-10*t) * ::sin( (t-s)*(2*pi_angle)/p ) + c);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static Real easeOutElastic(Real t, Real a, Real p)
{
    return easeOutElastic_helper(t, 0, 1, 1, a, p);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static Real easeInOutElastic(Real t, Real a, Real p)
{
    if (t==0) return 0.0;
    t*=2.0f;
    if (t==2) return 1.0;
    Real s;
    if(a < 1.0) {
        a = 1.0;
        s = p / 4.0f;
    } else {
        s = p / (2 * pi_angle) * ::asin(1.0f / a);
    }
    if (t < 1) return -.5f*(a*::pow(2.0f,10*(t-1)) * ::sin( (t-1-s)*(2*pi_angle)/p ));
    return a*::pow(2.0f,-10*(t-1)) * ::sin( (t-1-s)*(2*pi_angle)/p )*.5f + 1.0f;
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static Real easeOutInElastic(Real t, Real a, Real p)
{
    if (t < 0.5) return easeOutElastic_helper(t*2, 0.0f, 0.5f, 1.0f, a, p);
    return easeInElastic_helper(2*t - 1.0f, 0.5f, 0.5f, 1.0f, a, p);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static Real easeInBack(Real t, Real s)
{
    return t*t*((s+1)*t - s);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static Real easeOutBack(Real t, Real s)
{
    t-= Real(1.0);
    return t*t*((s+1)*t+ s) + 1;
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static Real easeInOutBack(Real t, Real s)
{
    t *= 2.0;
    if (t < 1) {
        s *= 1.525f;
        return 0.5f*(t*t*((s+1)*t - s));
    } else {
        t -= 2;
        s *= 1.525f;
        return 0.5f*(t*t*((s+1)*t+ s) + 2);
    }
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static Real easeOutInBack(Real t, Real s)
{
    if (t < 0.5) return easeOutBack (2*t, s)/2;
    return easeInBack(2*t - 1, s)/2 + 0.5f;
}
static Real easeOutBounce_helper(Real t, Real c, Real a)
{
    if (t == 1.0) return c;
    if (t < (4/11.0)) {
        return c*(7.5625f*t*t);
    } else if (t < (8/11.0)) {
        t -= (6/11.0f);
        return -a * (1.f - (7.5625f*t*t + .75f)) + c;
    } else if (t < (10/11.0f)) {
        t -= (9/11.0f);
        return -a * (1.f - (7.5625f*t*t + .9375f)) + c;
    } else {
        t -= (21/22.0f);
        return -a * (1.f - (7.5625f*t*t + .984375f)) + c;
    }
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating to zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static Real easeOutBounce(Real t, Real a)
{
    return easeOutBounce_helper(t, 1, a);
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static Real easeInBounce(Real t, Real a)
{
    return 1.0f - easeOutBounce_helper(1.0f-t, 1.0, a);
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static Real easeInOutBounce(Real t, Real a)
{
    if (t < 0.5f) return easeInBounce (2*t, a)/2;
    else return (t == 1.0f) ? 1.0f : easeOutBounce (2*t - 1, a)/2 + 0.5f;
}
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static Real easeOutInBounce(Real t, Real a)
{
    if (t < 0.5f) return easeOutBounce_helper(t*2, 0.5f, a);
    return 1.0f - easeOutBounce_helper (2.0f-2*t, 0.5f, a);
}
static inline Real qt_sinProgress(Real value)
{
    return sin((value * pi_angle) - pi_div_2_angle) / 2 + Real(0.5);
}
static inline Real qt_smoothBeginEndMixFactor(Real value)
{
    return minimum(maximum(1 - value * 2 + Real(0.3), Real(0.0)), Real(1.0));
}
// SmoothBegin blends Smooth and Linear Interpolation.
// Progress 0 - 0.3      : Smooth only
// Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
// Progress ~ 0.5  - 1   : Linear only
/**
 * Easing function that starts growing slowly, then increases in speed. At the end of the curve the speed will be constant.
 */
static Real easeInCurve(Real t)
{
    const Real sinProgress = qt_sinProgress(t);
    const Real mix = qt_smoothBeginEndMixFactor(t);
    return sinProgress * mix + t * (1 - mix);
}
/**
 * Easing function that starts growing steadily, then ends slowly. The speed will be constant at the beginning of the curve.
 */
static Real easeOutCurve(Real t)
{
    const Real sinProgress = qt_sinProgress(t);
    const Real mix = qt_smoothBeginEndMixFactor(1 - t);
    return sinProgress * mix + t * (1 - mix);
}
/**
 * Easing function where the value grows sinusoidally. Note that the calculated  end value will be 0 rather than 1.
 */
static Real easeSineCurve(Real t)
{
    return (sin(((t * pi_angle * 2)) - pi_div_2_angle) + 1) / 2;
}
/**
 * Easing function where the value grows cosinusoidally. Note that the calculated start value will be 0.5 and the end value will be 0.5
 * contrary to the usual 0 to 1 easing curve.
 */
static Real easeCosineCurve(Real t)
{
    return (cos(((t * pi_angle * 2)) - pi_div_2_angle) + 1) / 2;
}

namespace re {


typedef Real (*EasingFunction)(Real progress);

static EasingFunction curveToFunc(EasingCurve::Type curve)
{
    switch(curve) {
    case EasingCurve::Linear:
        return &easeNone;
    case EasingCurve::InQuad:
        return &easeInQuad;
    case EasingCurve::OutQuad:
        return &easeOutQuad;
    case EasingCurve::InOutQuad:
        return &easeInOutQuad;
    case EasingCurve::OutInQuad:
        return &easeOutInQuad;
    case EasingCurve::InCubic:
        return &easeInCubic;
    case EasingCurve::OutCubic:
        return &easeOutCubic;
    case EasingCurve::InOutCubic:
        return &easeInOutCubic;
    case EasingCurve::OutInCubic:
        return &easeOutInCubic;
    case EasingCurve::InQuart:
        return &easeInQuart;
    case EasingCurve::OutQuart:
        return &easeOutQuart;
    case EasingCurve::InOutQuart:
        return &easeInOutQuart;
    case EasingCurve::OutInQuart:
        return &easeOutInQuart;
    case EasingCurve::InQuint:
        return &easeInQuint;
    case EasingCurve::OutQuint:
        return &easeOutQuint;
    case EasingCurve::InOutQuint:
        return &easeInOutQuint;
    case EasingCurve::OutInQuint:
        return &easeOutInQuint;
    case EasingCurve::InSine:
        return &easeInSine;
    case EasingCurve::OutSine:
        return &easeOutSine;
    case EasingCurve::InOutSine:
        return &easeInOutSine;
    case EasingCurve::OutInSine:
        return &easeOutInSine;
    case EasingCurve::InExpo:
        return &easeInExpo;
    case EasingCurve::OutExpo:
        return &easeOutExpo;
    case EasingCurve::InOutExpo:
        return &easeInOutExpo;
    case EasingCurve::OutInExpo:
        return &easeOutInExpo;
    case EasingCurve::InCirc:
        return &easeInCirc;
    case EasingCurve::OutCirc:
        return &easeOutCirc;
    case EasingCurve::InOutCirc:
        return &easeInOutCirc;
    case EasingCurve::OutInCirc:
        return &easeOutInCirc;

    // Internal for, compatibility with QTimeLine only ??
    case EasingCurve::InCurve:
        return &easeInCurve;
    case EasingCurve::OutCurve:
        return &easeOutCurve;
    case EasingCurve::SineCurve:
        return &easeSineCurve;
    case EasingCurve::CosineCurve:
        return &easeCosineCurve;
    default:
        return 0;
    };
}



EasingCurve::EasingCurve(EasingCurve::Type type, Real period, Real amplitude, Real overshoot)
    : _t(type), _p(period), _a(amplitude), _o(overshoot)
{ }

EasingCurve::~EasingCurve() {

}

Real EasingCurve::value(Real t)
{
    EasingFunction func = curveToFunc(_t);
    return func(t);
}

EasingCurve::Type EasingCurve::fromText(const std::string &ec) {
    EasingCurve::Type out = EasingCurve::Linear;
#define EC_EVAL(T) if(equalCaseInsensitive(#T, ec)) return T;
    EC_EVAL(InQuad);
    EC_EVAL(OutQuad);
    EC_EVAL(InOutQuad);
    EC_EVAL(OutInQuad);
    EC_EVAL(InCubic);
    EC_EVAL(OutCubic);
    EC_EVAL(InOutCubic);
    EC_EVAL(OutInCubic);
    EC_EVAL(InQuart);
    EC_EVAL(OutQuart);
    EC_EVAL(InOutQuart);
    EC_EVAL(OutInQuart);
    EC_EVAL(InQuint);
    EC_EVAL(OutQuint);
    EC_EVAL(InOutQuint);
    EC_EVAL(OutInQuint);
    EC_EVAL(InSine);
    EC_EVAL(OutSine);
    EC_EVAL(InOutSine);
    EC_EVAL(OutInSine);
    EC_EVAL(InExpo);
    EC_EVAL(OutExpo);
    EC_EVAL(InOutExpo);
    EC_EVAL(OutInExpo);
    EC_EVAL(InCirc);
    EC_EVAL(OutCirc);
    EC_EVAL(InOutCirc);
    EC_EVAL(OutInCirc);
    EC_EVAL(InElastic);
    EC_EVAL(OutElastic);
    EC_EVAL(InOutElastic);
    EC_EVAL(OutInElastic);
    EC_EVAL(InBack);
    EC_EVAL(OutBack);
    EC_EVAL(InOutBack);
    EC_EVAL(OutInBack);
    EC_EVAL(InBounce);
    EC_EVAL(OutBounce);
    EC_EVAL(InOutBounce);
    EC_EVAL(OutInBounce);
    EC_EVAL(InCurve);
    EC_EVAL(OutCurve);
    EC_EVAL(SineCurve);
    EC_EVAL(CosineCurve);
    return out;
}

class ElasticEase : public EasingCurve
{
public:
    ElasticEase(EasingCurve::Type type, Real period, Real amplitude, Real overshoot)
        : EasingCurve(type, period, amplitude, overshoot)
    { }

    Real value(Real t) override
    {
        Real p = (_p < 0) ? Real(0.3) : _p;
        Real a = (_a < 0) ? Real(1.0) : _a;
        switch(_t) {
        case EasingCurve::InElastic:
            return easeInElastic(t, a, p);
        case EasingCurve::OutElastic:
            return easeOutElastic(t, a, p);
        case EasingCurve::InOutElastic:
            return easeInOutElastic(t, a, p);
        case EasingCurve::OutInElastic:
            return easeOutInElastic(t, a, p);
        default:
            return t;
        }
    }
};

struct BounceEase : public EasingCurve
{
public:
    BounceEase(EasingCurve::Type type, Real period, Real amplitude, Real overshoot)
        : EasingCurve(type, period, amplitude, overshoot)
    { }

    Real value(Real t) override
    {
        Real a = (_a < 0) ? Real(1.0) : _a;
        switch(_t) {
        case EasingCurve::InBounce:
            return easeInBounce(t, a);
        case EasingCurve::OutBounce:
            return easeOutBounce(t, a);
        case EasingCurve::InOutBounce:
            return easeInOutBounce(t, a);
        case EasingCurve::OutInBounce:
            return easeOutInBounce(t, a);
        default:
            return t;
        }
    }
};

class BackEase : public EasingCurve
{
public:
    BackEase(EasingCurve::Type type, Real period, Real amplitude, Real overshoot)
        : EasingCurve(type, period, amplitude, overshoot)
    { }


    Real value(Real t) override
    {
        Real o = (_o < 0) ? Real(1.70158) : _o;
        switch(_t) {
        case EasingCurve::InBack:
            return easeInBack(t, o);
        case EasingCurve::OutBack:
            return easeOutBack(t, o);
        case EasingCurve::InOutBack:
            return easeInOutBack(t, o);
        case EasingCurve::OutInBack:
            return easeOutInBack(t, o);
        default:
            return t;
        }
    }
};

std::shared_ptr<EasingCurve> makeEasingCurve(EasingCurve::Type type, Real period, Real amplitude, Real overshoot)
{
    switch(type) {
    case EasingCurve::InElastic:
    case EasingCurve::OutElastic:
    case EasingCurve::InOutElastic:
    case EasingCurve::OutInElastic:
        return std::make_shared<ElasticEase>(type, period, amplitude, overshoot);
    case EasingCurve::OutBounce:
    case EasingCurve::InBounce:
    case EasingCurve::OutInBounce:
    case EasingCurve::InOutBounce:
        return std::make_shared<BounceEase>(type, period, amplitude, overshoot);
    case EasingCurve::InBack:
    case EasingCurve::OutBack:
    case EasingCurve::InOutBack:
    case EasingCurve::OutInBack:
        return std::make_shared<BackEase>(type, period, amplitude, overshoot);
    default:
        return std::make_shared<EasingCurve>(type, period, amplitude, overshoot);
    }
    return 0;
}



} // namespace

