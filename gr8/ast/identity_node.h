#ifndef __GR8_IDENTITYNODE_H__
#define __GR8_IDENTITYNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace gr8 {

  class identity_node: public cdk::unary_expression_node {
  public:
    inline identity_node(int lineno, expression_node *expression) :
        cdk::unary_expression_node(lineno, expression) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identity_node(this, level);
    }

  };

}

#endif
