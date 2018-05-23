#ifndef __GR8_BLOCKNODE_H__
#define __GR8_BLOCKNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace gr8 {

    class block_node : public cdk::basic_node {
	cdk::sequence_node *_decls;
	cdk::sequence_node *_insts;

    public:
        block_node(int lineno, cdk::sequence_node *decls, cdk::sequence_node *insts)
            : cdk::basic_node(lineno), _decls (decls), _insts(insts) {
	}
	inline cdk::sequence_node *declarations() {
		return _decls;
	}
	inline cdk::sequence_node *instructions() {
		return _insts;
	}

        void accept(basic_ast_visitor *sp, int level) {
            sp->do_block_node(this, level);
        }
    };
}

#endif
