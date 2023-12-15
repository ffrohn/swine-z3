#pragma once

#include "util.h"

namespace swine {

class ConstantPropagator {

    Util &util;

public:

    ConstantPropagator(Util &util);

    z3::expr propagate(const z3::expr &expression) const;

};

}
