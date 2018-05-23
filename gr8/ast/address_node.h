#ifndef __GR8_ADDRESSNODE_H__
#define __GR8_ADDRESSNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace gr8 {

  class address_node: public cdk::expression_node {
  	cdk::expression_node *_lvalue;
  public:
    inline address_node(int lineno, cdk::expression_node *lvalue) :
        cdk::expression_node(lineno), _lvalue(lvalue) {
    }
	
    inline cdk::expression_node *lvalue(){
	return _lvalue;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_address_node(this, level);
    }

  };

}

#endif
