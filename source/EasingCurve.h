#pragma once

#include "Project.h"

namespace re {

class EasingCurve
{
public:
    enum Type {
        Linear,
        InQuad, OutQuad, InOutQuad, OutInQuad,
        InCubic, OutCubic, InOutCubic, OutInCubic,
        InQuart, OutQuart, InOutQuart, OutInQuart,
        InQuint, OutQuint, InOutQuint, OutInQuint,
        InSine, OutSine, InOutSine, OutInSine,
        InExpo, OutExpo, InOutExpo, OutInExpo,
        InCirc, OutCirc, InOutCirc, OutInCirc,
        InElastic, OutElastic, InOutElastic, OutInElastic,
        InBack, OutBack, InOutBack, OutInBack,
        InBounce, OutBounce, InOutBounce, OutInBounce,
        InCurve, OutCurve, SineCurve, CosineCurve
    };

    EasingCurve(Type type, Real period, Real amplitude, Real overshoot);
    virtual ~EasingCurve();
    virtual Real value(Real t);
    static EasingCurve::Type fromText(const std::string & ec);
protected:
    Type _t;
    Real _p;
    Real _a;
    Real _o;
};

std::shared_ptr<EasingCurve> makeEasingCurve(EasingCurve::Type type, Real period = 0.3, Real amplitude = 1.0, Real overshoot = 1.70158);

} //namespcae

