#pragma once

#include "util.h"

namespace swine {

class Rewriter {

    Util &util;

public:

    Rewriter(Util &util);

    z3::expr rewrite(const z3::expr &t);

};

}
