#include "swine.h"

#include "brute_force.h"
#include "preprocessor.h"
#include "exp_finder.h"
#include "term_evaluator.h"
#include "util.h"

#include <cassert>
#include <limits>
#include <ranges>
#include <utility>

namespace swine {

using namespace boost::multiprecision;

std::ostream& operator<<(std::ostream &s, const Swine::EvaluatedExponential &exp) {
    return s <<
           "abstract: exp(" <<
           exp.base <<
           ", " <<
           exp.exponent <<
           "); concrete: exp(" <<
           exp.base_val <<
           ", " <<
           exp.exponent_val <<
           ") = " <<
           exp.exp_expression_val;
}

std::ostream& operator<<(std::ostream &s, const Swine::Statistics &stats) {
    s << "assertions           : " << stats.num_assertions << std::endl;
    s << "iterations           : " << stats.iterations << std::endl;
    s << "symmetry lemmas      : " << stats.symmetry_lemmas << std::endl;
    s << "bounding lemmas      : " << stats.bounding_lemmas << std::endl;
    s << "monotonicity lemmas  : " << stats.monotonicity_lemmas << std::endl;
    s << "induction lemmas     : " << stats.induction_lemmas << std::endl;
    s << "prime lemmas         : " << stats.prime_lemmas << std::endl;
    s << "interpolation lemmas : " << stats.interpolation_lemmas << std::endl;
    s << "non constant base    : " << (stats.non_constant_base ? "true" : "false") << std::endl;
    return s;
}

std::ostream& operator<<(std::ostream &s, const Swine &swine) {
    return s << swine.get_solver();
}

Swine::Frame::Frame(z3::context &ctx): exps(ctx) {}

Swine::Swine(const Config &config, z3::context &ctx):
    config(config),
    ctx(ctx),
    solver(ctx),
    util(std::make_unique<Util>(ctx, this->config)),
    preproc(std::make_unique<Preprocessor>(*util)),
    exp_finder(std::make_unique<ExpFinder>(*util)),
    model(ctx) {
    solver.set("model", true);
    if (config.get_lemmas) {
        solver.set("unsat_core", true);
    }
    frames.emplace_back(ctx);
}

Swine::~Swine() = default;

void Swine::add_lemma(const z3::expr &t, const LemmaKind kind) {
    if (config.log) {
        std::cout << kind << " lemma:" << std::endl;
        std::cout << t << std::endl;
    }
    if (config.validate_unsat || config.get_lemmas) {
        frames.back().lemma_kinds.emplace(t.id(), kind);
        frames.back().lemmas.emplace(t.id(), t);
    }
    if (config.get_lemmas) {
        static unsigned int count {0};
        const auto assumption {ctx.bool_const(("assumption_" + std::to_string(count)).c_str())};
        ++count;
        frames.back().assumptions.emplace_back(assumption, t);
        solver.add(assumption == t);
    } else {
        solver.add(t);
    }
    switch (kind) {
    case LemmaKind::Interpolation: ++stats.interpolation_lemmas;
        break;
    case LemmaKind::Symmetry: ++stats.symmetry_lemmas;
        break;
    case LemmaKind::Prime: ++stats.prime_lemmas;
        break;
    case LemmaKind::Bounding: ++stats.bounding_lemmas;
        break;
    case LemmaKind::Monotonicity: ++stats.monotonicity_lemmas;
        break;
    case LemmaKind::Induction: ++stats.induction_lemmas;
        break;
    default: throw std::invalid_argument("unknown lemma kind");
    }
}

z3::expr Swine::get_value(const z3::expr &exp) const {
    return model.eval(exp, true);
}

void Swine::symmetry_lemmas(std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) const {
    if (!config.is_active(LemmaKind::Symmetry)) {
        return;
    }
    z3::expr_vector sym_lemmas{ctx};
    for (const auto &f: frames) {
        for (const auto &e: f.exps) {
            const auto ee {evaluate_exponential(e)};
            if (ee.exp_expression_val != ee.expected_val) {
                if (ee.base_val < 0) {
                    base_symmetry_lemmas(e, sym_lemmas);
                }
            }
        }
    }
    for (const auto &l: sym_lemmas) {
        lemmas.emplace_back(l, LemmaKind::Symmetry);
    }
}

void Swine::base_symmetry_lemmas(const z3::expr &e, z3::expr_vector &lemmas) const {
    if (!config.is_active(LemmaKind::Symmetry)) {
        return;
    }
    const auto base {e.arg(0)};
    const auto exp {e.arg(1)};
    if (!util->is_value(base) || util->value(base) < 0) {
        const auto conclusion_even {e == util->make_exp(-base, exp)};
        const auto conclusion_odd {e == -util->make_exp(-base, exp)};
        const auto premise_even {z3::mod(exp, 2) == 0};
        const auto premise_odd {z3::mod(exp, 2) == 1};
        lemmas.push_back(z3::implies(premise_even, conclusion_even));
        lemmas.push_back(z3::implies(premise_odd, conclusion_odd));
    }
}

void Swine::exp_symmetry_lemmas(const z3::expr &e, z3::expr_vector &lemmas) const {
    if (!config.is_active(LemmaKind::Symmetry)) {
        return;
    }
    const auto base {e.arg(0)};
    const auto exp {e.arg(1)};
    const auto lemma {z3::implies(base == -1, e == util->make_exp(base, -exp))};
    lemmas.push_back(lemma);
}

void Swine::compute_bounding_lemmas(const ExpGroup &g) {
    if (!config.is_active(LemmaKind::Bounding)) {
        return;
    }
    for (const auto &e: g.all()) {
        auto [it, inserted] {frames.back().bounding_lemmas.emplace(e.id(), z3::expr_vector(ctx))};
        if (!inserted) {
            return;
        }
        auto &set {it->second};
        const auto base {e.arg(0)};
        const auto exp {e.arg(1)};
        // exp = 0 ==> base^exp = 1
        set.push_back(z3::implies(exp == 0, e == 1));
        // exp = 1 ==> base^exp = base
        set.push_back(z3::implies(exp == 1, e == base));
        if (!util->is_value(base) || util->value(base) < -1) {
            // exp < 0 && base < -1 ==> base^exp = 0
            set.push_back(z3::implies(exp < 0 && base < -1, e == 0));
        }
        if (!util->is_value(base) || util->value(base) == 0) {
            // base = 0 && exp != 0 ==> base^exp = 0
            set.push_back(z3::implies(base == 0 && exp != 0, e == 0));
        }
        if (!util->is_value(base) || util->value(base) == 1) {
            // base = 1 ==> base^exp = 1
            set.push_back(z3::implies(base == 1, e == 1));
        }
        if (!util->is_value(base) || util->value(base) > 1) {
            // exp < 0 && base > 1 ==> base^exp = 0
            set.push_back(z3::implies(exp < 0 && base > 1, e == 0));
            // exp + base > 4 && base > 1 && exp > 1 ==> base^exp > base * exp + 1
            set.push_back(z3::implies(
                base + exp > 4 && base > 1 && exp > 1,
                e > base * exp + 1));
        }
    }
}

void Swine::bounding_lemmas(std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) const {
    if (!config.is_active(LemmaKind::Bounding)) {
        return;
    }
    std::unordered_set<unsigned> seen;
    for (const auto &f: frames) {
        for (const auto &g: f.exp_groups) {
            if (seen.contains(g->orig().id())) {
                continue;
            }
            seen.emplace(g->orig().id());
            for (const auto &e: g->all()) {
                if (const auto ee {evaluate_exponential(e)}; ee.exp_expression_val != ee.expected_val) {
                    for (const auto &l: f.bounding_lemmas.at(e.id())) {
                        lemmas.emplace_back(l, LemmaKind::Bounding);
                    }
                }
            }
        }
    }
}

void Swine::add(const z3::expr &t) {
    try {
        ++stats.num_assertions;
        if (config.log) {
            std::cout << "assertion:" << std::endl;
            std::cout << t << std::endl;
        }
        const auto preprocessed {preproc->preprocess(t)};
        frames.back().preprocessed_assertions.emplace_back(preprocessed, t);
        solver.add(preprocessed);
        try {
            for (const auto &g: exp_finder->find_exps(preprocessed)) {
                if (frames.back().exp_ids.emplace(g.orig().id()).second) {
                    frames.back().exps.push_back(g.orig());
                    frames.back().exp_groups.emplace_back(std::make_shared<ExpGroup>(g));
                    stats.non_constant_base |= !g.has_ground_base();
                    compute_bounding_lemmas(g);
                }
            }
        } catch (const ExpInQuantifierException&) {
            frames.back().assert_failed = "exp in quantifier";
        }
    } catch (const ExponentOverflow&) {
        frames.back().assert_failed = "exponent-overflow";
    }
}

Swine::EvaluatedExponential::EvaluatedExponential(const z3::expr &exp_expression):
    exp_expression(exp_expression),
    base(exp_expression.arg(0)),
    exponent(exp_expression.arg(1)) {}

Swine::EvaluatedExponential Swine::evaluate_exponential(const z3::expr &exp_expression) const {
    EvaluatedExponential res{exp_expression};
    res.exp_expression_val = util->value(get_value(res.exp_expression));
    res.base_val = util->value(get_value(res.base));
    res.exponent_val = Util::to_int(get_value(res.exponent));
    if (res.exponent_val >= 0 || abs(res.base_val) == 1) {
        res.expected_val = pow(res.base_val, res.exponent_val);
    } else {
        res.expected_val = 0;
    }
    return res;
}

Swine::Interpolant::Interpolant(z3::expr t): t(std::move(t)) {}

Swine::Interpolant Swine::interpolate(const z3::expr &t, const unsigned pos, const cpp_int& x1, const cpp_int& x2) const {
    Interpolant res{t};
    z3::expr_vector children{ctx};
    for (const auto &c: t.args()) {
        children.push_back(c);
    }
    const auto x {children[pos]};
    auto t1 {util->term(x1)};
    children.set(pos, t1);
    const auto at_x1 {t.decl()(children)};
    res.factor = abs(x2 - x1);
    if (res.factor == 0) {
        res.factor = 1;
        res.t = at_x1;
    } else {
        auto t2 = util->term(x2);
        children.set(pos, t2);
        const auto at_x2 {t.decl()(children)};
        res.t = util->term(res.factor) * at_x1 + (at_x2 - at_x1) * (x - util->term(x1));
    }
    return res;
}

z3::expr Swine::interpolation_lemma(const z3::expr &t, const bool upper, const std::pair<cpp_int, long long>& a, const std::pair<cpp_int, long long>& b) const {
    const auto x1 {min(a.first, b.first)};
    const auto x2 {max(a.first, b.first)};
    const auto y1 {std::min(a.second, b.second)};
    const auto y2 {std::max(a.second, b.second)};
    const auto base {t.arg(0)};
    const auto exp {t.arg(1)};
    // const auto op = upper ? Le : Ge;
    // y1 <= exponent <= y2
    const auto exponent_in_bounds {util->term(y1) <= exp && exp <= util->term(y2)};
    // exponent > 0
    const auto exponent_positive {exp > util->term(0)};
    if (util->is_value(base)) {
        const auto i {interpolate(t, 1, y1, y2)};
        const auto premise = upper ? exponent_in_bounds : exponent_positive;
        const auto conclusion_lhs {t * util->term(i.factor)};
        const auto conclusion = upper ? conclusion_lhs <= i.t : conclusion_lhs >= i.t;
        return z3::implies(premise, conclusion);
    }
    const auto at_y1 {util->make_exp(base, util->term(y1))};
    const auto at_y2 {util->make_exp(base, util->term(y2))};
    const auto i1 {interpolate(at_y1, 0, x1, x2)};
    const auto i2 {interpolate(at_y2, 0, x1, x2)};
    z3::expr premise{ctx};
    if (upper) {
        // x1 <= base <= x2
        const auto base_in_bounds {util->term(x1) <= base && base <= util->term(x2)};
        premise = base_in_bounds && exponent_in_bounds;
    } else {
        // exponent >= y1
        const auto exponent_above_threshold {exp >= util->term(y1)};
        // base > 0
        const auto base_positive {base > util->term(0)};
        premise = base_positive && exponent_above_threshold;
    }
    z3::expr conclusion {ctx};
    if (y2 == y1) {
        const auto lhs {t * util->term(i1.factor)};
        conclusion = upper ? lhs <= i1.t : lhs >= i1.t;
    } else {
        const auto y_diff {util->term(y2 - y1)};
        const auto lhs {t * util->term(i1.factor) * y_diff};
        const auto rhs {i1.t * y_diff + (i2.t - i1.t) * (exp - util->term(y1))};
        conclusion = upper ? lhs <= rhs : lhs >= rhs;
    }
    return z3::implies(premise, conclusion);
}

void Swine::interpolation_lemma(const EvaluatedExponential &e, std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) {
    z3::expr lemma{ctx};
    auto &vec {interpolation_points.emplace(e.exp_expression.id(), std::vector<std::pair<cpp_int, long long>>()).first->second};
    if (e.exp_expression_val < e.expected_val) {
        const auto min_base = e.base_val > 1 ? e.base_val - 1 : e.base_val;
        const auto min_exp = e.exponent_val > 1 ? e.exponent_val - 1 : e.exponent_val;
        lemma = interpolation_lemma(e.exp_expression, false, {min_base, min_exp}, {min_base + 1, min_exp + 1});
    } else {
        std::optional<std::pair<cpp_int, long long>> nearest;
        std::optional<cpp_int> min_dist;
        for (const auto &[x, y]: vec) {
            const auto x_dist {x - e.base_val};
            const auto y_dist {y - e.exponent_val};
            if (const auto dist {x_dist * x_dist + y_dist * y_dist}; !min_dist || dist <= *min_dist) {
                nearest = {x, y};
                min_dist = dist;
            }
        }
        const std::pair current {e.base_val, e.exponent_val};
        lemma = interpolation_lemma(e.exp_expression, true, current, nearest.value_or(current));
    }
    vec.emplace_back(e.base_val, e.exponent_val);
    lemmas.emplace_back(lemma, LemmaKind::Interpolation);
}

void Swine::interpolation_lemmas(std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) {
    if (!config.is_active(LemmaKind::Interpolation)) {
        return;
    }
    for (const auto &f: frames) {
        for (const auto &g: f.exp_groups) {
            for (const auto &e: g->maybe_non_neg_base()) {
                if (const auto ee {evaluate_exponential(e)}; ee.exp_expression_val != ee.expected_val && ee.base_val > 0 && ee.exponent_val > 0) {
                    interpolation_lemma(ee, lemmas);
                }
            }
        }
    }
}

std::optional<z3::expr> Swine::induction_lemma(EvaluatedExponential e1, EvaluatedExponential e2) const {
    if (e1.base_val != e2.base_val || e1.exponent_val <= 0 || e2.exponent_val <= 0 || e1.exponent_val == e2.exponent_val || e1.exp_expression_val == 0) {
        return {};
    }
    if (e1.exponent_val > e2.exponent_val) {
        const auto tmp = e1;
        e1 = e2;
        e2 = tmp;
    }
    const auto base_val {e1.base_val};
    if (const auto diff_val {e2.exponent_val - e1.exponent_val}; e2.exp_expression_val % e1.exp_expression_val != 0 || e2.exp_expression_val / e1.exp_expression_val != pow(base_val, diff_val)) {
        const auto diff {util->term(diff_val)};
        const z3::expr premise{e1.base == e2.base && e2.exponent - e1.exponent == diff && e1.exponent >= 0};
        const z3::expr conclusion{e2.exp_expression == e1.exp_expression * z3::pw(e1.base, diff)};
        return z3::implies(premise, conclusion);
    }
    return {};
}

void Swine::induction_lemmas(std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) const {
    if (!config.is_active(LemmaKind::Induction)) {
        return;
    }
    std::unordered_map<cpp_int, std::vector<EvaluatedExponential>> exps_by_base;
    for (const auto &f: frames) {
        for (const auto &g: f.exp_groups) {
            for (const auto e: g->maybe_non_neg_base()) {
                if (const auto eval {evaluate_exponential(e)}; eval.base_val > 1) {
                    const auto it {exps_by_base.emplace(eval.base_val, std::vector<EvaluatedExponential>()).first};
                    it->second.emplace_back(eval);
                }
            }
        }
    }
    for (auto& exps : exps_by_base | std::views::values) {
        if (exps.size() > 1) {
            std::ranges::sort(exps, [](const auto &e1, const auto &e2) {
                return e1.exponent_val < e2.exponent_val;
            });
            for (auto it = exps.begin(); std::next(it) != exps.end(); ++it) {
                if (const auto lem {induction_lemma(*it, *std::next(it))}) {
                    lemmas.emplace_back(*lem, LemmaKind::Induction);
                }
            }
        }
    }
}

std::optional<z3::expr> Swine::monotonicity_lemma(const EvaluatedExponential &e1, const EvaluatedExponential &e2) const {
    if ((e1.base_val > e2.base_val && e1.exponent_val < e2.exponent_val)
        || (e1.base_val < e2.base_val && e1.exponent_val > e2.exponent_val)
        || (e1.base_val == e2.base_val && e1.exponent_val == e2.exponent_val)) {
        return {};
    }
    bool is_smaller = e1.base_val < e2.base_val || e1.exponent_val < e2.exponent_val;
    const auto [smaller, greater] = is_smaller ? std::pair(e1, e2) : std::pair(e2, e1);
    if (smaller.exp_expression_val < greater.exp_expression_val) {
        return {};
    }
    z3::expr premise{ctx};
    const z3::expr strict_exp_premise {smaller.exponent < greater.exponent};
    const z3::expr non_strict_exp_premise {smaller.exponent <= greater.exponent};
    if (!util->is_value(smaller.base) || !util->is_value(greater.base)) {
        const z3::expr strict_base_premise {smaller.base < greater.base};
        const z3::expr non_strict_base_premise {smaller.base <= greater.base};
        premise = non_strict_base_premise && non_strict_exp_premise && (strict_base_premise || strict_exp_premise);
    } else if (smaller.base_val < greater.base_val) {
        premise = non_strict_exp_premise;
    } else {
        premise = strict_exp_premise;
    }
    premise = 1 < smaller.base && 0 < smaller.exponent && premise;
    return z3::implies(premise, smaller.exp_expression < greater.exp_expression);
}

void Swine::monotonicity_lemmas(std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) const {
    if (!config.is_active(LemmaKind::Monotonicity)) {
        return;
    }
    // search for pairs exp(b,e1), exp(b,e2) whose models violate monotonicity of exp
    z3::expr_vector exps{ctx};
    for (const auto &f: frames) {
        for (const auto &g: f.exp_groups) {
            for (const auto &e: g->maybe_non_neg_base()) {
                const auto base {e.arg(0)};
                if (const auto exp {e.arg(1)}; util->value(get_value(base)) > 1 && util->value(get_value(exp)) > 0) {
                    exps.push_back(e);
                }
            }
        }
    }
    for (auto it1 = exps.begin(); it1 != exps.end(); ++it1) {
        const auto e1 {evaluate_exponential(*it1)};
        for (auto it2 = it1; ++it2 != exps.end();) {
            const auto e2 {evaluate_exponential(*it2)};
            if (const auto mon_lemma {monotonicity_lemma(e1, e2)}) {
                lemmas.emplace_back(*mon_lemma, LemmaKind::Monotonicity);
            }
        }
    }
}

void Swine::prime_lemmas(std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) {
    if (!config.is_active(LemmaKind::Prime)) {
        return;
    }
    const auto mk_lem = [&](const auto &ee, const auto &dt) {
        return (z3::mod(ee.base, dt) == 0 && ee.exponent != 0) == (z3::mod(ee.exp_expression, dt) == 0);
    };
    int inc[8]{4, 2, 4, 2, 4, 6, 2, 6};
    for (const auto& f: frames) {
        for (auto e: f.exps) {
            const auto ee = evaluate_exponential(e);
            auto min = std::min(ee.base_val, ee.exp_expression_val);
            auto max = std::max(ee.base_val, ee.exp_expression_val);
            if (min <= 0 || max % min == 0) {
                continue;
            }
            const auto gcd = boost::multiprecision::gcd(max, min);
            min = min / gcd;
            max = max / gcd;
            auto done = false;
            const auto process_divisor = [&](const auto &d) {
                if (min % d == 0 || max % d == 0) {
                    const auto dt = util->term(d);
                    lemmas.emplace_back(mk_lem(ee, dt), LemmaKind::Prime);
                    done = true;
                }
                return done;
            };
            for (const auto d: std::vector{2, 3, 5}) {
                if (process_divisor(d)) {
                    break;
                }
            }
            cpp_int d = 7;
            auto i = 0;
            while (!done && d * d <= max) {
                if (process_divisor(d)) {
                    break;
                }
                d = d + inc[i];
                i = (i + 1) % 8;
            }
            if (!done) {
                // both min and max are prime
                process_divisor(min);
                assert(done);
            }
        }
    }
}

void Swine::add_bounds() {
    std::unordered_set<unsigned int> seen;
    const auto b {util->term(pow(cpp_int(2), bound))};
    for (const auto &f: frames) {
        for (const auto &g: f.exp_groups) {
            for (const auto &e: g->all()) {
                if (const auto exponent {e.arg(1)}; seen.insert(exponent.id()).second) {
                    solver.add(exponent <= b);
                    solver.add(-b <= exponent);
                }
            }
        }
    }
}

std::vector<std::pair<z3::expr, LemmaKind>> Swine::preprocess_lemmas(const std::vector<std::pair<z3::expr, LemmaKind>> &lemmas) const {
    std::vector<std::pair<z3::expr, LemmaKind>> res;
    for (const auto &[l,k]: lemmas) {
        // lemmas *must* be preprocessed before checking the value to make sure that ground exp-subterms are interpreted
        // according to the semantics of exp, and not according to the current model
        if (const auto p = preproc->preprocess(l); get_value(p).is_false()) {
            res.emplace_back(p, k);
        }
    }
    return res;
}

z3::check_result Swine::check(z3::expr_vector assumptions) {
    for (const auto &f: frames) {
        if (f.assert_failed) {
            reason_unknown = *f.assert_failed;
            return z3::unknown;
        }
    }
    auto res {z3::unknown};
    unsigned rconsumption {0};
    while (config.rlimit == 0 || rconsumption < config.rlimit) {
        try {
            ++rconsumption;
            ++stats.iterations;
            if (config.get_lemmas) {
                for (const auto &f: frames) {
                    for (const auto& a : f.assumptions | std::views::keys) {
                        assumptions.push_back(a);
                    }
                }
            }
            if (config.toggle_mode && sat_mode) {
                solver.push();
                add_bounds();
            }
            if (!assumptions.empty()) {
                res = solver.check(assumptions);
            } else {
                res = solver.check();
            }
            if (res == z3::unsat) {
                if (config.toggle_mode && sat_mode) {
                    sat_mode = false;
                    solver.pop();
                    continue;
                }
                if (config.get_lemmas) {
                    const auto core {solver.unsat_core()};
                    std::unordered_set<unsigned> ids;
                    for (const auto &c: core) {
                        ids.insert(c.id());
                    }
                    std::cout << "===== lemmas =====" << std::endl;
                    for (const auto &k: lemma_kind::values) {
                        auto first {true};
                        for (const auto &f: frames) {
                            for (const auto &[a,l]: f.assumptions) {
                                if (ids.contains(a.id()) && f.lemma_kinds.at(l.id()) == k) {
                                    if (first) {
                                        std::cout << "----- " << k << " lemmas -----" << std::endl;
                                        first = false;
                                    }
                                    std::cout << l << std::endl;
                                }
                            }
                        }
                    }
                }
                if (config.validate_unsat) {
                    brute_force();
                }
                break;
            }
            if (res == z3::unknown) {
                if (config.log) {
                    std::cout << "unknown from z3" << std::endl;
                }
                reason_unknown = "(z3-unknown " + solver.reason_unknown() + ")";
                return z3::unknown;
            }
            if (res == z3::sat) {
                if (config.toggle_mode && !sat_mode) {
                    sat_mode = true;
                    ++bound;
                    continue;
                }
                model = solver.get_model();
                if (config.toggle_mode) {
                    solver.pop();
                }
                bool sat {true};
                if (config.log) {
                    std::cout << "candidate model:" << std::endl;
                    std::cout << model << std::endl;
                }
                std::vector<std::pair<z3::expr, LemmaKind>> lemmas;
                // check if the model can be lifted
                TermEvaluator evaluator{*util};
                for (const auto &f: frames) {
                    for (const auto &e: f.preprocessed_assertions | std::views::values) {
                        if (!evaluator.evaluate(e, model).is_true()) {
                            sat = false;
                            break;
                        }
                    }
                    if (!sat) {
                        break;
                    }
                }
                if (sat) {
                    if (config.get_lemmas) {
                        std::cout << "===== lemmas =====" << std::endl;
                        for (const auto &k: lemma_kind::values) {
                            auto first {true};
                            for (const auto &f: frames) {
                                for (const auto &[id,l]: f.lemmas) {
                                    if (f.lemma_kinds.at(id) == k) {
                                        if (first) {
                                            std::cout << "----- " << k << " lemmas -----" << std::endl;
                                            first = false;
                                        }
                                        std::cout << l << std::endl;
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
                symmetry_lemmas(lemmas);
                lemmas = preprocess_lemmas(lemmas);
                if (lemmas.empty()) {
                    bounding_lemmas(lemmas);
                    lemmas = preprocess_lemmas(lemmas);
                }
                if (lemmas.empty()) {
                    monotonicity_lemmas(lemmas);
                    lemmas = preprocess_lemmas(lemmas);
                }
                if (lemmas.empty()) {
                    prime_lemmas(lemmas);
                    induction_lemmas(lemmas);
                    interpolation_lemmas(lemmas);
                    lemmas = preprocess_lemmas(lemmas);
                }
                if (lemmas.empty()) {
                    if (config.is_active(LemmaKind::Interpolation) && config.is_active(LemmaKind::Bounding) && config.is_active(LemmaKind::Symmetry)) {
                        throw std::logic_error("refinement failed, but interpolation, bounding, and symmetry lemmas are enabled");
                    }
                    reason_unknown = "failed-refinement";
                    return z3::unknown;
                }
                for (const auto &[l, kind]: lemmas) {
                    add_lemma(l, kind);
                }
            }
        } catch (const ExponentOverflow&) {
            reason_unknown = "exponent-overflow";
            return z3::unknown;
        }
    }
    if (config.statistics) {
        std::cout << stats << std::endl;
    }
    return res;
}

z3::check_result Swine::check() {
    return check(z3::expr_vector(ctx));
}

void Swine::push() {
    solver.push();
    frames.emplace_back(ctx);
}

void Swine::pop() {
    solver.pop();
    frames.pop_back();
}

void Swine::reset() {
    solver.reset();
    frames.clear();
    frames.emplace_back(ctx);
}

void Swine::verify() const {
    TermEvaluator eval{*util};
    const auto m {this->model};
    for (const auto &f: frames) {
        for (const auto& a : f.preprocessed_assertions | std::views::values) {
            if (!eval.evaluate(a, m).is_true()) {
                std::cout << "Validation of the following assertion failed:" << std::endl;
                std::cout << a << std::endl;
                std::cout << "model:" << std::endl;
                std::cout << m << std::endl;
                return;
            }
        }
    }
}

void Swine::brute_force() const {
    z3::expr_vector assertions{ctx};
    for (const auto &f: frames) {
        for (const auto& a : f.preprocessed_assertions | std::views::keys) {
            assertions.push_back(a);
        }
    }
    z3::expr_vector exps{ctx};
    for (const auto &f: frames) {
        for (const auto &e: f.exps) {
            exps.push_back(e);
        }
    }
    if (BruteForce bf(*util, assertions, exps); bf.check_sat()) {
        std::cout << "sat via brute force" << std::endl;
        if (config.log) {
            std::cout << "candidate model:" << std::endl;
            std::cout << solver.get_model() << std::endl;
        }
        for (const auto &f: frames) {
            for (const auto &[id,l]: f.lemmas) {
                if (!get_value(l).is_true()) {
                    std::cout << "violated " << f.lemma_kinds.at(id) << " lemma" << std::endl;
                    std::cout << l << std::endl;
                }
            }
        }
        verify();
    }
}

z3::context& Swine::get_ctx() const {
    return ctx;
}

z3::func_decl& Swine::get_exp() const {
    return *util->exp;
}

const z3::solver& Swine::get_solver() const {
    return solver;
}

z3::solver& Swine::get_solver() {
    return solver;
}

z3::model Swine::get_model() const {
    return solver.get_model();
}

std::string Swine::get_reason_unknown() const {
    return reason_unknown;
}

}
