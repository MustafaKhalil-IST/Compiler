#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

void gr8::type_checker::do_nil_node(cdk::nil_node * const node, int lvl) {
  //empty
}
void gr8::type_checker::do_data_node(cdk::data_node * const node, int lvl) {
  //empty
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_sequence_node(cdk::sequence_node * const node, int lvl) {
   for(auto nd : node->nodes()) {
	nd->accept(this, lvl+2);
   }
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_UNSPEC;

  node->argument()->accept(this, lvl + 2);

  if (node->argument()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in argument of not expression");
  }
  
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in left argument of or expression");
  }
  if (node->right()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in right argument of or expression");
  }
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in left argument of and expression");
  }
  if (node->right()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong type in right argument of and expression");
  }
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

void gr8::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}
//---------------------------------------------------------------------------

void gr8::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT) throw std::string("wrong type in argument of unary expression");
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void gr8::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  if (node->left()->type()->name() !=node->right()->type()->name() &&
      !( (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() ==  basic_type::TYPE_DOUBLE)
         || (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() ==  basic_type::TYPE_INT)
         || (node->left()->type()->name() == basic_type::TYPE_POINTER && node->right()->type()->name() ==  basic_type::TYPE_INT)
         || (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() ==  basic_type::TYPE_POINTER)
       ) ){
	throw std::string("types are not equals");
  }
  size_t sz = 0;
  if (node->left()->type()->name() == basic_type::TYPE_INT){
  	sz = 4;
  }
  if (node->left()->type()->name() == basic_type::TYPE_STRING){
  	sz = 4;
  }
  if (node->left()->type()->name() == basic_type::TYPE_POINTER){
  	sz = 4;
  }
  if (node->left()->type()->name() == basic_type::TYPE_DOUBLE){
  	sz = 8;
  }
  node->type(new basic_type(sz, node->left()->type()->name()));
}

void gr8::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
  if ( (node->left()->type()->name() == basic_type::TYPE_POINTER) && (node->right()->type()->name() == basic_type::TYPE_INT) ) {
    node->type(node->left()->type());
    return;
  }
  if ( (node->left()->type()->name() == basic_type::TYPE_INT) && (node->right()->type()->name() == basic_type::TYPE_POINTER) ) {
    node->type(node->right()->type());
    return;
  }
}
void gr8::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
  if ( (node->left()->type()->name() == basic_type::TYPE_POINTER) && (node->right()->type()->name() == basic_type::TYPE_INT) ) {
    node->type(node->left()->type());
    return;
  }
  if ( (node->left()->type()->name() == basic_type::TYPE_INT) && (node->right()->type()->name() == basic_type::TYPE_POINTER) ) {
    node->type(node->right()->type());
    return;
  }
}
void gr8::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void gr8::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC
  const std::string &id = node->name();
  auto symbol = _symtab.find(id);
  if (symbol == nullptr) {
    throw "Symbol not found";
  }

  node->type(symbol->type());
}

void gr8::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());  
}

void gr8::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {

  ASSERT_UNSPEC

  node->lvalue()->accept(this, lvl+2);
  node->rvalue()->accept(this, lvl+2);

  
  node->type(node->lvalue()->type());

  int lefttype = node->lvalue()->type()->name();
  
  int righttype = node->rvalue()->type()->name();


  if (lefttype == righttype) {
    return;
  }
  
  if ( (lefttype == basic_type::TYPE_DOUBLE) && (righttype == basic_type::TYPE_INT) ) {
    return;
  }
  
  if ( (lefttype == basic_type::TYPE_POINTER) && (righttype == basic_type::TYPE_UNSPEC) ) {
    return;
  }

  throw std::string("Incombatible types"); 


}

//---------------------------------------------------------------------------

void gr8::type_checker::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}


//---------------------------------------------------------------------------

void gr8::type_checker::do_read_node(gr8::read_node * const node, int lvl) {
  ASSERT_UNSPEC;
  //if ( node->type()->name() != basic_type::TYPE_INT || node->type()->name() != basic_type::TYPE_DOUBLE )
  //   throw std::string("wrong type in input, should be int or double");
  node->type(new basic_type(4, basic_type::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void gr8::type_checker::do_if_node(gr8::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if ( node->condition()->type()->name() != basic_type::TYPE_INT ) {
    throw std::string("Wrong Condition");
  }
  node->block()->accept(this, lvl + 2);
}

void gr8::type_checker::do_function_node(gr8::function_node *const node, int lvl) {

  const std::string &id = node->identifier();
  auto symbol = _symtab.find(id);
  
}

void gr8::type_checker::do_variable_node(gr8::variable_node *const node, int lvl) {
  const std::string &id = node->identifier();
  if (_symtab.find_local(id) != nullptr) {
    throw id + " redeclared";
  }
  if (node->initializeExpression() != nullptr) {
    node->initializeExpression()->accept(this, lvl+2);

    if (node->type()->name() == basic_type::TYPE_DOUBLE && node->initializeExpression()->type()->name() == basic_type::TYPE_INT) {
      return;
    }
    
    if (node->initializeExpression()->type()->name() == basic_type::TYPE_UNSPEC) {
      return;
    }

    if (node->type()->name() != node->initializeExpression()->type()->name()) {
      throw std::string("Incombatiple types");
    }
  }
}
  
void gr8::type_checker::do_block_node(gr8::block_node *const node, int lvl) {
   if(node->declarations()) { node->declarations()->accept(this, lvl+2); }
  if(node->instructions()) { node->instructions()->accept(this, lvl+2); }
}
void gr8::type_checker::do_address_node(gr8::address_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void gr8::type_checker::do_index_node(gr8::index_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->base()->accept(this, lvl+2);
  node->index()->accept(this, lvl+2);
  if ( node->base()->type()->name() != basic_type::TYPE_POINTER ) {
    throw std::string("the base should be a pointer");
  }
  if ( node->index()->type()->name() != basic_type::TYPE_INT ) {
    throw std::string("the index should be an integer");
  }
  node->type(node->base()->type()->subtype()); 

}

void gr8::type_checker::do_post_node(gr8::post_node *const node, int lvl) {
  // EMPTY
  std::cout << "\ntype checker post\n"; 
  node->argument()->accept(this, lvl+2);
}
void gr8::type_checker::do_sweep_by_node(gr8::sweep_by_node *const node, int lvl) {
  if (node->fromExpression() != nullptr) {
    node->fromExpression()->accept(this, lvl+2);

    if (node->fromExpression()->type()->name() != basic_type::TYPE_INT) {
      throw std::string("min limit must be integer");
    }
  }
  else{
     throw std::string("NO given min limit");
  }

  if (node->toExpression() != nullptr) {
    node->toExpression()->accept(this, lvl+2);

    if (node->toExpression()->type()->name() != basic_type::TYPE_INT) {
      throw std::string("max limit must be integer");
    }
  }
  else{
     throw std::string("NO given max limit");
  }

  if (node->byExpression() != nullptr) {
    node->byExpression()->accept(this, lvl+2);

    if (node->byExpression()->type()->name() != basic_type::TYPE_INT) {
      throw std::string("update value must be integer");
    }
  }
}

void gr8::type_checker::do_again_node(gr8::again_node *const node, int lvl) {
  // EMPTY
}
void gr8::type_checker::do_if_else_node(gr8::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
  if ( node->condition()->type()->name() != basic_type::TYPE_INT ) {
    throw std::string("Wrong Condition");
  }
  node->block()->accept(this, lvl + 2);
  node->elseblock()->accept(this, lvl + 2);
}
void gr8::type_checker::do_stop_node(gr8::stop_node *const node, int lvl) {
  // EMPTY
}
void gr8::type_checker::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  // EMPTY
   node->expression()->accept(this, lvl+2);
}
void gr8::type_checker::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;
  auto functionID = _symtab.find(node->identifier());
  if ( functionID == nullptr) {
    throw std::string("unknown function") + node->identifier();
  }

  if (node->arguments() != nullptr) {
    node->arguments()->accept(this, lvl + 2);

    auto expected_arguments = functionID->arguments();

    auto given_arguments = node->arguments()->nodes();

    if( expected_arguments.size() != given_arguments.size()) {
      throw std::string("Wrong number of arguments");
    }
   
    for (size_t i = 0; i < expected_arguments.size() ; i++){
      auto given_argument = (cdk::expression_node *) given_arguments[i];
      if (expected_arguments[i]->name() != given_argument->type()->name() &&
          !( (expected_arguments[i]->name() == basic_type::TYPE_INT && given_argument->type()->name() == basic_type::TYPE_DOUBLE)
             || (expected_arguments[i]->name() == basic_type::TYPE_DOUBLE && given_argument->type()->name() == basic_type::TYPE_INT) )  )
      {
          throw std::string("different call arguments types");
      }
    }
  } 

  node->type(functionID->type());
}
void gr8::type_checker::do_identity_node(gr8::identity_node *const node, int lvl) {
  // EMPTY
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->type(new basic_type(4, basic_type::TYPE_DOUBLE));
  } else {
    throw std::string("wrong type in argument of identity expression");
  }
}

void gr8::type_checker::do_allocation_node(gr8::allocation_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->expression()->accept(this, lvl+2);
  int type = node->expression()->type()->name();
  if ( type != basic_type::TYPE_INT ) {
    throw std::string("Error: must be integer");
  }
  node->type(new basic_type(0, basic_type::TYPE_UNSPEC));
}

void gr8::type_checker::do_null_node(gr8::null_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_UNSPEC));
}

void gr8::type_checker::do_return_node(gr8::return_node *const node, int lvl) {
  // EMPTY
}


