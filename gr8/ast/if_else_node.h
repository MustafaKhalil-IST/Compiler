// $Id: if_else_node.h,v 1.6 2018/04/18 15:56:26 ist427068 Exp $ -*- c++ -*-
#ifndef __GR8_IFELSENODE_H__
#define __GR8_IFELSENODE_H__

#include <cdk/ast/expression_node.h>

namespace gr8 {

  /**
   * Class for describing if-then-else nodes.
   */
  class if_else_node: public cdk::basic_node {
    cdk::expression_node *_condition;
    cdk::basic_node *_block, *_elseblock;

  public:
    inline if_else_node(int lineno, cdk::expression_node *condition, cdk::basic_node *block, cdk::basic_node *elseblock) :
                       cdk::basic_node(lineno), _condition(condition), _block(block), _elseblock(elseblock) {
    }

  public:
    inline cdk::expression_node *condition() {
      return _condition;
    }
    inline cdk::basic_node *block() {
      return _block;
    }
    inline cdk::basic_node *elseblock() {
      return _elseblock;
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_if_else_node(this, level);
    }

  };

} // gr8

#endif
