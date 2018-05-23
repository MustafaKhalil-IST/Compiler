#ifndef __GR8_AGAINNODE_H__
#define __GR8_AGAINNODE_H__

#include <cdk/ast/basic_node.h>

namespace gr8 {

  class again_node: public cdk::basic_node {
    int _again;
  public:
    inline again_node(int lineno, int again) :
        cdk::basic_node(lineno), _again(again)  {
    }

    inline int again() {
      return _again;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_again_node(this, level);
    }

  };

}

#endif
