#include "template.hpp"

namespace alg {
struct lazytree_structure {
    struct value_structure {
        struct value_type {

        };
        using S = value_type;
        static S op(const S& a, const S& b) {
            return S{};
        }
        static S e() {
            return S{};
        }
    };
    struct operator_structure {
        struct value_type {

        };
        using S = value_type;
        static S op(const S& a, const S& b) {
            return S{};
        }
        static S e() {
            return S{};
        }
    };

    using V = typename value_structure::value_type;
    using O = typename operator_structure::value_type;
    static V op(const V& v, const O& o) {
        return V{};
    }
};
}

namespace alg {
struct segtree_structure {
    struct value_structure {
        struct value_type {

        };
        using S = value_type;
        static S op(const S& a, const S& b) {
            return S{};
        }
        static S e() {
            return S{};
        }
    };
};
}