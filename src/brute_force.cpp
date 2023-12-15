#include "brute_force.h"

namespace swine {

BruteForce::BruteForce(Util &util, const z3::expr_vector &assertions, const z3::expr_vector &exps): util(util), solver(util.ctx), assertions(assertions), exps(exps) {
}

bool BruteForce::next(const unsigned long weight, std::vector<std::pair<z3::expr, unsigned long>>::iterator begin) {
    auto it = begin;
    if (++it == current.end()) {
        return false;
    } else if (next(weight - begin->second, it)) {
        return true;
    } else if (begin->second == 0) {
        return false;
    } else {
        begin->second--;
        it->second = weight - begin->second;
        while (++it != current.end()) {
            it->second = 0;
        }
        return true;
    }
}

bool BruteForce::next() {
    if (!next(bound, current.begin())) {
        if (bound == *util.config.validate_unsat) {
            return false;
        }
        ++bound;
        auto it {current.begin()};
        it->second = bound;
        while (++it != current.end()) {
            it->second = 0;
        }
    }
    return true;
}

bool BruteForce::check_sat() {
    for (const auto &a: assertions) {
        solver.add(a);
    }
    for (const auto &e: exps) {
        current.emplace_back(e, 0);
    }
    do {
        if (util.config.log) {
            std::cout << "brute force:";
            for (const auto &c: current) {
                std::cout << " " << c.first << "=" << c.second;
            }
            std::cout << std::endl;
        }
        solver.push();
        for (const auto &[exp, v]: current) {
            const auto val {util.term(v)};
            const auto base {exp.arg(0)};
            const auto exponent {exp.arg(1)};
            const auto exp_eq {exponent == val};
            solver.add(exp_eq);
            const auto res_eq {exp == z3::pw(base, val)};
            solver.add(res_eq);
        }
        const auto res {solver.check()};
        if (res == z3::sat) {
            return true;
        }
        solver.pop();
    } while (next());
    return false;
}

}
