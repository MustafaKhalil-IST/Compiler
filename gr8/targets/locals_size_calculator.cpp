#include "targets/locals_size_calculator.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void gr8::locals_size_calculator::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    if (node->node(i) != nullptr) {
      node->node(i)->accept(this, lvl + 2);
    }
  }
}

void gr8::locals_size_calculator::do_integer_node(cdk::integer_node * const node, int lvl) {
  _size += 4;
}

void gr8::locals_size_calculator::do_double_node(cdk::double_node * const node, int lvl) {
  _size += 8;
}

void gr8::locals_size_calculator::do_string_node(cdk::string_node * const node, int lvl) {
  _size += 4;
}

void gr8::locals_size_calculator::do_variable_node(gr8::variable_node * const node, int lvl) {
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    _size += 8;
  }else{
    _size += 4;
  }
}

void gr8::locals_size_calculator::do_block_node(gr8::block_node * const node, int lvl) {
  node->declarations()->accept(this, lvl);
  node->instructions()->accept(this, lvl);
}

void gr8::locals_size_calculator::do_if_node(gr8::if_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}

void gr8::locals_size_calculator::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  node->block()->accept(this, lvl);
  node->elseblock()->accept(this, lvl);
}

void gr8::locals_size_calculator::do_sweep_by_node(gr8::sweep_by_node * const node, int lvl) {
  node->doBlock()->accept(this, lvl);
}

void gr8::locals_size_calculator::do_lt_node(cdk::lt_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_eq_node(cdk::eq_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_le_node(cdk::le_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_ge_node(cdk::ge_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_not_node(cdk::not_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_neg_node(cdk::neg_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_mod_node(cdk::mod_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_or_node(cdk::or_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_ne_node(cdk::ne_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_identifier_node(cdk::identifier_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_gt_node(cdk::gt_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_add_node(cdk::add_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_div_node(cdk::div_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_sub_node(cdk::sub_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_and_node(cdk::and_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_mul_node(cdk::mul_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_function_node(gr8::function_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_address_node(gr8::address_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_read_node(gr8::read_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_index_node(gr8::index_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_post_node(gr8::post_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_identity_node(gr8::identity_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_allocation_node(gr8::allocation_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_evaluation_node(gr8::evaluation_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_null_node(gr8::null_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_again_node(gr8::again_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_return_node(gr8::return_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_stop_node(gr8::stop_node *const node, int lvl) {
  // EMPTY
}
void gr8::locals_size_calculator::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  // EMPTY
}

