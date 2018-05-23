#ifndef __GR8_VARIABLENODE_H__
#define __GR8_VARIABLENODE_H__

#include "cdk/ast/expression_node.h"
#include "cdk/ast/basic_node.h"
#include "cdk/basic_type.h"


namespace gr8 {

  class variable_node: public cdk::basic_node {

    basic_type *_type;
    std::string _identifier;
    cdk::expression_node *_initializeExpression;
	
    int _scope;
  public:
    inline variable_node(int lineno, basic_type *type, const std::string *identifier, cdk::expression_node *initializeExpression,int scope) :
        cdk::basic_node(lineno), _type(type), _identifier(*identifier), _initializeExpression(initializeExpression), _scope(scope){
    }

    inline variable_node(int lineno, basic_type *type, const std::string &identifier, cdk::expression_node *initializeExpression,int scope) :
        cdk::basic_node(lineno), _type(type), _identifier(identifier), _initializeExpression(initializeExpression), _scope(scope){
    }

    basic_type *type() const {
      return _type;
    }

    const std::string &identifier() const {
      return _identifier;
    }

    cdk::expression_node *initializeExpression() const {
      return _initializeExpression;
    }

    void scope(int s){
      _scope = s;
    }

    int scope() const {
      return _scope;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_variable_node(this, level);
    }

  };

}

#endif
