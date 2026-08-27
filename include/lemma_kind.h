#pragma once

#include <string>
#include <unordered_set>

namespace swine {

enum class LemmaKind {
    Negativity, Bounding, Interpolation, Monotonicity, Induction, Prime
};


namespace lemma_kind {
    static const std::unordered_set values{
        LemmaKind::Negativity,
        LemmaKind::Bounding,
        LemmaKind::Interpolation,
        LemmaKind::Monotonicity,
        LemmaKind::Induction,
        LemmaKind::Prime
    };

    std::string str(LemmaKind k);

}

std::ostream& operator<<(std::ostream &s, LemmaKind kind);

}
