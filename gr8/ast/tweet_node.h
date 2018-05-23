// $Id: tweet_node.h,v 1.4 2018/03/26 07:36:57 ist427068 Exp $ -*- c++ -*-
#ifndef __GR8_TWEETNODE_H__
#define __GR8_TWEETNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace gr8 {

  /**
   * Class for describing tweet keyword.
   */
  class tweet_node: public cdk::basic_node {
    cdk::expression_node *_expression;

  public:
    inline tweet_node(int lineno, cdk::expression_node *expression) :
        cdk::basic_node(lineno), _expression(expression) {
    }

  public:
    inline cdk::expression_node *expression() {
      return _expression;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_tweet_node(this, level);
    }

  };

} // gr8

#endif
