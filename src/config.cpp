#include "config.h"

namespace swine {

void Config::deactivate(const LemmaKind k) {
    active_lemma_kinds.erase(k);
}

bool Config::is_active(const LemmaKind k) const {
    return active_lemma_kinds.contains(k);
}

void Config::deactivate(const PreprocKind k) {
    active_preprocessings.erase(k);
}

bool Config::is_active(const PreprocKind k) const {
    return active_preprocessings.contains(k);
}

void Config::set_rlimit(unsigned rlimit) {
    this->rlimit = rlimit;
}

}
