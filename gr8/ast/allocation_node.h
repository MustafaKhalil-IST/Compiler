#ifndef __GR8_ALLOCATIONNODE_H__
#define __GR8_ALLOCATIONNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/expression_node.h>

namespace gr8 {

  class allocation_node: public cdk::expression_node {
    cdk::expression_node *_expression;
    
  public:
    inline allocation_node(int lineno, cdk::expression_node *expression) :
        cdk::expression_node(lineno), _expression(expression){
    }

    cdk::expression_node *expression() const {
	return _expression;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_allocation_node(this, level);
    }

  };

}

#endif
