#ifndef __GR8_NULLNODE_H__
#define __GR8_NULLNODE_H__

#include <cdk/ast/data_node.h>

namespace gr8 {

  class null_node: public cdk::expression_node {
    
  public:
    inline null_node(int lineno) :
        cdk::expression_node(lineno) {
    }
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_null_node(this, level);
    }
  };
} 

#endif
