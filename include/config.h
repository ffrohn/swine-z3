#pragma once

#include "lemma_kind.h"
#include "preproc_kind.h"

#include <unordered_set>

namespace swine {

struct Config {

    bool validate_sat {false};
    std::optional<unsigned int> validate_unsat;
    bool log {false};
    bool statistics {false};
    bool get_lemmas {false};
    bool debug {false};
    std::unordered_set<LemmaKind> active_lemma_kinds {lemma_kind::values};
    std::unordered_set<PreprocKind> active_preprocessings{preproc_kind::values};

    void deactivate(const LemmaKind k) {
        active_lemma_kinds.erase(k);
    }

    bool is_active(const LemmaKind k) const {
        return active_lemma_kinds.contains(k);
    }

    void deactivate(const PreprocKind k) {
        active_preprocessings.erase(k);
    }

    bool is_active(const PreprocKind k) const {
        return active_preprocessings.contains(k);
    }

};

}
