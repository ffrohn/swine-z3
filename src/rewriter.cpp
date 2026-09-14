#include "rewriter.h"

#include <ranges>

namespace swine {

Rewriter::Rewriter(Util &util): util(util) {}

z3::expr Rewriter::rewrite(const z3::expr &t) {
    const auto zero {util.ctx.int_val(0)};
    const auto one {util.ctx.int_val(1)};
    const auto mone {util.ctx.int_val(-1)};
    std::optional<z3::expr> res;
    if (t.is_lambda() || t.is_quantifier()) {
        return t;
    }
    if (t.num_args() == 0) {
        return t;
    } else {
        z3::expr_vector children{util.ctx};
        for (const auto &c: t.args()) {
            children.push_back(rewrite(c));
        }
        if (util.is_abstract_exp(t)) {
            const auto base {children[0]};
            const auto exp {children[1]};
            if (base.id() == one.id() || exp.id() == zero.id()) {
                res = util.term(1);
            } else if (exp.id() == one.id()) {
                res = base;
            } else if (util.is_value(exp)) {
                if (const auto val = util.value(exp); val >= 0) {
                    res = rewrite(z3::pw(base, exp));
                }
            } else if (util.is_abstract_exp(base)) {
                const auto inner_base{base.arg(0)};
                const auto inner_exp{base.arg(1)};
                res = z3::ite(
                    inner_exp < 0 && exp < 0,
                    util.make_exp(inner_base, -(exp * inner_exp)),
                    util.make_exp(inner_base, exp * inner_exp));
            }
        } else if (t.decl().decl_kind() == Z3_OP_MUL) {
            // rewrite exp(x,z) * exp(y,z)
            {
                std::unordered_map<unsigned, z3::expr_vector> map;
                z3::expr_vector new_children{util.ctx};
                for (const auto &c: children) {
                    if (util.is_abstract_exp(c)) {
                        const auto exp {c.arg(1)};
                        auto &set {map.emplace(exp.id(), z3::expr_vector(util.ctx)).first->second};
                        set.push_back(c);
                    } else {
                        new_children.push_back(c);
                    }
                }
                bool changed = false;
                for (const auto &set: map | std::views::values) {
                    if (set.size() > 1) {
                        changed = true;
                        z3::expr_vector bases{util.ctx};
                        for (const auto &e: set) {
                            bases.push_back(e.arg(0));
                        }
                        new_children.push_back(util.make_exp(t.decl()(bases), set.back().arg(1)));
                    } else {
                        new_children.push_back(*set.begin());
                    }
                }
                if (changed) {
                    res = t.decl()(new_children);
                }
            }
            // rewrite exp(x,y) * exp(x,z)
            if (!res) {
                std::unordered_map<unsigned, z3::expr_vector> map;
                z3::expr_vector new_children{util.ctx};
                for (const auto &c: children) {
                    if (util.is_abstract_exp(c)) {
                        const auto base {c.arg(0)};
                        auto &set {map.emplace(base.id(), z3::expr_vector(util.ctx)).first->second};
                        set.push_back(c);
                    } else {
                        new_children.push_back(c);
                    }
                }
                bool changed = false;
                for (const auto &set: map | std::views::values) {
                    if (set.size() > 1) {
                        changed = true;
                        z3::expr_vector exponents{util.ctx};
                        z3::expr_vector conditions{util.ctx};
                        z3::expr base{util.ctx};
                        for (const auto &e: set) {
                            base = e.arg(0);
                            exponents.push_back(e.arg(1));
                            conditions.push_back(e.arg(1) < 0);
                        }
                        z3::expr cond = base != -1 && base != 1 && z3::mk_or(conditions);
                        z3::expr exp = util.make_exp(base, z3::sum(exponents));
                        z3::expr ite = z3::ite(cond, util.term(0), exp);
                        new_children.push_back(ite);
                    } else {
                        new_children.push_back(*set.begin());
                    }
                }
                if (changed) {
                    res = t.decl()(new_children);
                }
            }
        } else if (t.decl().decl_kind() == Z3_OP_POWER) {
            const auto fst {rewrite(children[0])};
            const auto snd {rewrite(children[1])};
            if (util.is_value(snd)) {
                const auto val {util.value(snd)};
                if (util.is_abstract_exp(fst) && val >= 0) {
                    const auto base {fst.arg(0)};
                    const auto exp {fst.arg(1)};
                    res = util.make_exp(base, exp * snd);
                } else if (val <= 10) {
                    res = util.term(1);
                    for (int i = 0; i < val; ++i) {
                        res = *res * fst;
                    }
                }
            }
        }
        if (!res) {
            res = t.decl()(children);
        }
        return *res;
    }
}

}
