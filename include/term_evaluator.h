#pragma once

#include "util.h"

namespace swine {

class TermEvaluator {

    Util &util;

public:

    TermEvaluator(Util &util);

    z3::expr evaluate(const z3::expr &expression, const z3::model &model);

};

}
