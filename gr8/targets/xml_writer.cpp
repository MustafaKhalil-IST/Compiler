#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void gr8::xml_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void gr8::xml_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}
void gr8::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  // EMPTY
}
void gr8::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}
void gr8::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  // EMPTY
}
void gr8::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}
/*

void gr8::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {

  // EMPTY
}
void gr8::xml_writer::do_and_node(gr8::and_node * const node, int lvl) {
  // EMPTY
}
void gr8::xml_writer::do_or_node(gr8::or_node * const node, int lvl) {
  // EMPTY
}
*/
//---------------------------------------------------------------------------

void gr8::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void gr8::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void gr8::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void gr8::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  /*ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;*/
}

void gr8::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

/*void gr8::xml_writer::do_program_node(gr8::program_node * const node, int lvl) {
  openTag(node, lvl);
  node->statements()->accept(this, lvl + 4);
  closeTag(node, lvl);
}*/

//---------------------------------------------------------------------------

void gr8::xml_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

/*void gr8::xml_writer::do_print_node(gr8::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}*/

//---------------------------------------------------------------------------

void gr8::xml_writer::do_read_node(gr8::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

/*void gr8::xml_writer::do_while_node(gr8::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}*/

//---------------------------------------------------------------------------

void gr8::xml_writer::do_if_node(gr8::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void gr8::xml_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}



void gr8::xml_writer::do_function_node(gr8::function_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("type", lvl + 2);
  os() << std::string(lvl + 4 ,' ') << node->type()->name() << std::endl;
  closeTag("type", lvl + 2);
  openTag("identifier", lvl + 2);
  os() << std::string(lvl + 4 ,' ') << node->identifier() << std::endl;
  closeTag("identifier", lvl + 2);
  if(node->variables() != nullptr) 
      node->variables()->accept(this, lvl + 2);
  node->block()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_variable_node(gr8::variable_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2, ' ') << "<type>" << node->type()->name() << "</type>" << std::endl;
  os() << std::string(lvl + 2, ' ') << "<identifier>" << node->identifier() << "</identifier>" << std::endl;
  if(node->initializeExpression() != nullptr)
     node->initializeExpression()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_block_node(gr8::block_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  if(node->declarations() != nullptr)
     node->declarations()->accept(this, lvl + 4);
  if(node->instructions() != nullptr)
     node->instructions()->accept(this, lvl + 4);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_address_node(gr8::address_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_index_node(gr8::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->base()->accept(this, lvl + 2);
  node->index()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_post_node(gr8::post_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_sweep_by_node(gr8::sweep_by_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("variable", lvl + 2);
  node->lvalue()->accept(this, lvl + 4);
  closeTag("variable", lvl + 2);
  openTag("from", lvl + 2);
  node->fromExpression()->accept(this, lvl + 4);
  closeTag("variable", lvl + 2);
  openTag("to", lvl + 2);
  node->toExpression()->accept(this, lvl + 4);
  closeTag("to", lvl + 2);
  if(node->byExpression() != nullptr){
    openTag("by", lvl + 2);
    node->byExpression()->accept(this, lvl + 4);
    closeTag("by", lvl + 2);
  }
  closeTag(node, lvl);
}
void gr8::xml_writer::do_again_node(gr8::again_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2,' ') << node->again() << std::endl;
  closeTag(node, lvl);
}
void gr8::xml_writer::do_stop_node(gr8::stop_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl + 2,' ') << node->stop() << std::endl;
  closeTag(node, lvl);
}
void gr8::xml_writer::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->expression()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_return_node(gr8::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->expression()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("identifier", lvl);
  os() << std::string(lvl + 4,' ') << node->identifier() << std::endl;
  closeTag("identifier", lvl);
  node->arguments()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_identity_node(gr8::identity_node *const node, int lvl) {
  do_unary_expression(node, lvl);
}
void gr8::xml_writer::do_allocation_node(gr8::allocation_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->expression()->accept(this, lvl + 2);
  closeTag(node, lvl);
}
void gr8::xml_writer::do_null_node(gr8::null_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl ,' ') << node->label() << std::endl;
}
/*void gr8::xml_writer::do_attribution_node(gr8::attribution_node *const node, int lvl) {
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->expression()->accept(this, lvl + 4);
  closeTag(node, lvl); 
}*/

/*void gr8::xml_writer::do_identifier_node(gr8::identifier_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  os() << std::string(lvl+1, ' ') << node->identifier(); 
  closeTag(node, lvl); 
}*/
