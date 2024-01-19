#include "preprocessor.h"

namespace swine {

Preprocessor::Preprocessor(Util &util): util(util), rewriter(util), constant_propagator(util) {}

z3::expr Preprocessor::preprocess(const z3::expr &term) {
    const auto log = [&](const z3::expr &term, const PreprocKind kind, const std::function<z3::expr(const z3::expr&)> &f){
        bool done {false};
        const z3::expr res {f(term)};
        if (util.config.log && res.id() != term.id()) {
            if (!done) {
                std::cout << "preprocessing" << std::endl;
                std::cout << "original term:" << std::endl;
                std::cout << term << std::endl;
                done = true;
            }
            std::cout << kind << ":" << std::endl;
            std::cout << res << std::endl;
        }
        return res;
    };
    const std::function<z3::expr(const z3::expr&)> cp {[&](const z3::expr &term) {
            return constant_propagator.propagate(term);
        }
    };
    const std::function<z3::expr(const z3::expr&)> rw {[&](const z3::expr &term) {
            return rewriter.rewrite(term);
        }
    };
    const std::function<z3::expr(const z3::expr&)> do_cp {[&](const z3::expr &term) {
            if (util.config.is_active(PreprocKind::ConstantFolding)) {
                return log(term, PreprocKind::ConstantFolding, cp);
            } else {
                return term;
            }
        }
    };
    const std::function<z3::expr(const z3::expr&)> do_rw {[&](const z3::expr &term) {
            if (util.config.is_active(PreprocKind::Rewriting)) {
                return log(term, PreprocKind::Rewriting, rw);
            } else {
                return term;
            }
        }
    };
    auto last {term};
    auto cterm {do_cp(term)};
    auto rterm {do_rw(cterm)};
    auto res {rterm};
    while (res.id() != last.id()) {
        last = res;
        if (res.id() != cterm.id()) {
            cterm = do_cp(res);
            res = cterm;
        }
        if (res.id() != rterm.id()) {
            rterm = do_rw(res);
            res = rterm;
        }
    }
    res = res.simplify();
    if (util.config.log && term.id() != res.id()) std::cout << "preprocessing finished" << std::endl;
    return res;
}

}
