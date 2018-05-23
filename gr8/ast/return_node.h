#ifndef __GR8_RETURNNODE_H__
#define __GR8_RETURNNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace gr8 {

  class return_node: public cdk::basic_node {
    cdk::expression_node *_expression;
  public:
    inline return_node(int lineno, cdk::expression_node *expression) :
        cdk::basic_node(lineno), _expression(expression)  {
    }

    inline cdk::expression_node *expression(){
      return _expression;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} //gr8

#endif
