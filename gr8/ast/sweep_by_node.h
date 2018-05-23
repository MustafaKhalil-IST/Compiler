#ifndef __GR8_SWEEPBYNODE_H__
#define __GR8_SWEEPBYNODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace gr8 {

  /**
   * Class for describing the sweep by keyword
   */
  class sweep_by_node: public cdk::basic_node {
    cdk::lvalue_node *_lvalue;
    cdk::expression_node *_fromExpression;
    cdk::expression_node *_toExpression;
    cdk::expression_node *_byExpression;
    cdk::basic_node *_doBlock;
  public:
    inline sweep_by_node(int lineno,cdk::lvalue_node *lvalue, cdk::expression_node *fromExpression,
	              cdk::expression_node *toExpression, cdk::expression_node *byExpression , cdk::basic_node *doBlock) :
        cdk::basic_node(lineno) ,_lvalue(lvalue) ,_fromExpression(fromExpression) ,_toExpression(toExpression) 
        ,_byExpression(byExpression)  ,_doBlock(doBlock){
    }
    
    inline  cdk::lvalue_node *lvalue(){
	return _lvalue;
    }

    inline  cdk::expression_node *fromExpression(){
	return _fromExpression;
    }

    inline  cdk::expression_node *toExpression(){
	return _toExpression;
    }
    inline  cdk::expression_node *byExpression(){
	return _byExpression;
    }
    inline cdk::basic_node *doBlock(){
        return _doBlock;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweep_by_node(this, level);
    }

  };

}

#endif
