#pragma once

#include "util.h"

namespace swine {

class ExpInQuantifierException final : public std::exception {};

class ExpGroup {

    const z3::expr t;
    Util &util;

    const bool ground_base;
    const bool neg_base;

public:

    ExpGroup(const z3::expr &t, Util &util);

    z3::expr_vector all() const;
    z3::expr_vector maybe_non_neg_base() const;
    bool has_ground_base() const;
    z3::expr orig() const;

};

class ExpFinder {

public:

    Util &util;

    bool find_exps(const z3::expr &term, z3::expr_vector &res);

public:

    ExpFinder(Util &util);

    std::vector<ExpGroup> find_exps(const z3::expr & term);

};

}
