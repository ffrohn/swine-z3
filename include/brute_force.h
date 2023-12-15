#pragma once

#include "util.h"

#include <z3++.h>
#include <map>

namespace swine {

class BruteForce {
    Util &util;
    z3::solver solver;
    z3::expr_vector assertions;
    z3::expr_vector exps;
    unsigned long bound {0};
    std::vector<std::pair<z3::expr, unsigned long>> current;

    bool next();
    bool next(const unsigned long weight, std::vector<std::pair<z3::expr, unsigned long>>::iterator begin);

public:

    BruteForce(Util &util, const z3::expr_vector &assertions, const z3::expr_vector &exps);
    bool check_sat();

};

}
