#ifndef __GR8_LOCALS_SIZE_CALCULATOR_H__
#define __GR8_LOCALS_SIZE_CALCULATOR_H__

#include <cdk/ast/basic_node.h>
#include "targets/basic_ast_visitor.h"

namespace gr8 {

  class locals_size_calculator: public basic_ast_visitor {
    int _size = 0;

  public:
    locals_size_calculator(std::shared_ptr<cdk::compiler> compiler) :
        basic_ast_visitor(compiler) {
    }

    int size() {
      return _size;
    }

public:
  // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
  // do not edit these lines: end


 
  };

} 

#endif
