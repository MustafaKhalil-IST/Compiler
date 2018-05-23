#ifndef __GR8_STOPNODE_H__
#define __GR8_STOPNODE_H__

#include <cdk/ast/basic_node.h>

namespace gr8 {

  class stop_node: public cdk::basic_node {
    int _stop;
  public:
    inline stop_node(int lineno, int stop) :
        cdk::basic_node(lineno), _stop(stop)  {
    }

    inline int stop() {
      return _stop;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

}

#endif
