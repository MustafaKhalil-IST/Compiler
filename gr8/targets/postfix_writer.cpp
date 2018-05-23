#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/locals_size_calculator.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}

void gr8::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

// NOT Node
void gr8::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl+2);
  _pf.NEG();
  _pf.EQ();
}

// AND Node
void gr8::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS
  int lbl = ++_lbl;
  node->left()->accept(this, lvl+2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  _pf.TRASH(node->type()->size());
  node->right()->accept(this, lvl+2);
  _pf.LABEL(mklbl(lbl)); 
}

// OR Node
void gr8::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS
  int lbl = ++_lbl;
  node->left()->accept(this, lvl+2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  _pf.TRASH(node->type()->size());
  node->right()->accept(this, lvl+2);
  _pf.LABEL(mklbl(lbl)); 
}


//---------------------------------------------------------------------------

void gr8::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {   // not complete
  if(lvl == 0){
    _pf.EXTERN("readi");
    _pf.EXTERN("printi");
    _pf.EXTERN("printd");
    _pf.EXTERN("prints");
    _pf.EXTERN("println");
    _pf.EXTERN("argc"); 
    _pf.EXTERN("argv"); 
    _pf.EXTERN("envp");
  }
  for (size_t i = 0; i < node->size(); i++) {
    if (node->node(i) != nullptr) {
      node->node(i)->accept(this, lvl + 2);
    }
  }
}

//---------------------------------------------------------------------------

// INTEGER Node
void gr8::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
    
    node->type(new basic_type(4, basic_type::TYPE_INT)); 
 
    std::cout<<"INT "<<node->value()<<"\n";
    if(_global) { 
       _pf.SINT(node->value());  
    }else{
       _pf.INT(node->value());  
    }
}

// DOUBLE Node
void gr8::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
   node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
   if (_infnc) {
    _pf.DOUBLE(node->value());
    std::cout<<"DOUBLE "<<node->value()<<"\n";
  } else {      
    _pf.SDOUBLE(node->value()); 
    std::cout<<"SDOUBLE "<<node->value()<<"\n";
  }
}

void gr8::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {

  node->type(new basic_type(4, basic_type::TYPE_STRING));

  int lbl = ++_lbl;

  _pf.RODATA(); 
  _pf.ALIGN(); 
  _pf.LABEL(mklbl(lbl)); 
  _pf.SSTRING(node->value()); 
  
  std::cout<<"RODATA\nALIGN\nLABEL "<<mklbl(lbl)<<"\nSSTRING "<<node->value()<<"\n";
  
  _lststradr = mklbl(lbl);

  if (_infnc) { 
    _pf.TEXT(); 
    _pf.ADDR(mklbl(lbl));
    std::cout<<"TEXT\nADDR "<<mklbl(lbl)<<"\n";
  } 
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl+2); 
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.NEG(); 
    std::cout<<"NEG\n";
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DNEG();
    std::cout<<"DNEG\n";
  } 
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->type()->name() == basic_type::TYPE_POINTER) {
    int sz = 0;
    if(node->left()->type()->name() == basic_type::TYPE_INT) {
      node->right()->accept(this, lvl+2); 
      sz = node->right()->type()->subtype()->size();
      node->left()->accept(this, lvl+2);  
    }else{
      node->left()->accept(this, lvl+2);  
      sz = node->left()->type()->subtype()->size();
      node->right()->accept(this, lvl+2); 
    } 
    _pf.INT(sz); 
    _pf.MUL();   
    _pf.ADD();    
    return;
  }
  node->left()->accept(this, lvl);
  if ( node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
    std::cout<<"I2D\n";
  }
  node->right()->accept(this, lvl);
  if ( node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
    std::cout<<"I2D\n";
  }
  _pf.ADD();
  std::cout<<"ADD\n";
}

void gr8::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if ( node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if ( node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
  }
  _pf.SUB();
}

void gr8::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if ( node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if ( node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
  }
  _pf.MUL();
  std::cout<<"MUL\n";
}

void gr8::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  if ( node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
  }
  node->right()->accept(this, lvl);
  if ( node->right()->type()->name() == basic_type::TYPE_INT && node->left()->type()->name()  == basic_type::TYPE_DOUBLE ) {
    _pf.I2D();
  }
  _pf.DIV();
}

void gr8::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();;
}

void gr8::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
  _pf.DUP32();
  std::cout<<"LT\nDUP32\n";
}

void gr8::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}

void gr8::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}

void gr8::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
  _pf.DUP32();
  std::cout<<"GT\nDUP32\n";
}

void gr8::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}

void gr8::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
  std::cout<<"EQ\n";
}

//---------------------------------------------------------------------------
void gr8::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {  // not complete
  ASSERT_SAFE_EXPRESSIONS;

  std::string id = node->name();
  auto symbol = _symtab.find(id);

  if (symbol->visibility() == 2) {
    std::cout<<"LOCAL "<< symbol->value() << "\n";
    _pf.LOCAL(symbol->value());
  } else {    
    std::cout<<"ADDR "<<id<<"\n";       
    _pf.ADDR(id);
  }
}



void gr8::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl+2);
  if(_adrs) return;
  if (node->lvalue()->type()->name() == basic_type::TYPE_INT || node->lvalue()->type()->name() == basic_type::TYPE_UNSPEC 
        || node->lvalue()->type()->name() == basic_type::TYPE_STRING || node->lvalue()->type()->name() == basic_type::TYPE_POINTER) {
    std::cout<<"LDINT\n";
    _pf.LDINT();
  } else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE) {
    std::cout<<"LDDOUBLE\n";
    _pf.LDDOUBLE();
  }
}

void gr8::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) { 
  ASSERT_SAFE_EXPRESSIONS;
  node->rvalue()->accept(this, lvl+2); 
  
  if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && node->rvalue()->type()->name() == basic_type::TYPE_INT) {
    _pf.I2D();
    node->rvalue()->type(node->lvalue()->type());
  }
 
  node->lvalue()->accept(this, lvl+2);

  if (node->rvalue()->type()->name() == basic_type::TYPE_INT || node->rvalue()->type()->name() == basic_type::TYPE_UNSPEC 
        || node->rvalue()->type()->name() == basic_type::TYPE_STRING || node->rvalue()->type()->name() == basic_type::TYPE_POINTER ) {
    std::cout<<"STINT\n";
    _pf.STINT();
  } else if (node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE) {
    std::cout<<"STDOUBLE\n";
    _pf.STDOUBLE();
  }
}


void gr8::postfix_writer::do_evaluation_node(gr8::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int evalsize = node->argument()->type()->size();
  node->argument()->accept(this, lvl+2);
  _pf.TRASH(evalsize); 
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_read_node(gr8::read_node * const node, int lvl) {
   //ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.LDFVAL32();
}


void gr8::postfix_writer::do_if_node(gr8::if_node * const node, int lvl) {
  std::cout<<"if \n";
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  std::cout<<"JZ "<<mklbl(lbl1)<<"\n";

  node->block()->accept(this, lvl + 2);

  std::cout<<"LABEL "<<mklbl(lbl1)<<"\n";
  _pf.LABEL(mklbl(lbl1));
  std::cout << "here\n";
}

//---------------------------------------------------------------------------

void gr8::postfix_writer::do_if_else_node(gr8::if_else_node * const node, int lvl) {
  std::cout<<"if else\n";
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;

  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));

  std::cout<<"JZ "<<mklbl(lbl1)<<"\n";
  node->block()->accept(this, lvl + 2);

  _pf.JMP(mklbl(lbl2 = ++_lbl));
  std::cout<<"JMP "<<mklbl(lbl2)<<"\n";

  _pf.LABEL(mklbl(lbl1));
  std::cout<<"LABEL "<<mklbl(lbl1)<<"\n";

  node->elseblock()->accept(this, lvl + 2);

  _pf.LABEL(mklbl(lbl1 = lbl2));
  std::cout<<"LABEL "<<mklbl(lbl1)<<"\n";
}
//---------------------------------------------------------------------------

void gr8::postfix_writer::do_function_node(gr8::function_node *const node, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;

  if(node->block()){

     _infnc = true;                         

     const std::string &id = node->identifier(); 

     auto symbol = _symtab.find(id);    
     _offset = 0;                        
     _return = ++_lbl;                 

     if (symbol == nullptr) { 
       symbol = std::make_shared<gr8::symbol>(node->type(), node->identifier(), 0, node->scope());
       _symtab.insert(id, symbol);
     }

     _pf.TEXT();
     std::cout<<"TEXT\n";
     _pf.ALIGN();
     std::cout<<"ALIGN\n";

     if(id == "covfefe"){
       if (node->scope() == 1) { 
	  _pf.GLOBAL("_main", _pf.FUNC()); 
 	  std::cout<<"GLOBAL _main,FUNC\n"; 
       }
       _pf.LABEL("_main");
       std::cout<<"LABEL _main\n";
     }
     else{
        if (node->scope() == 1) { 
	  _pf.GLOBAL(id, _pf.FUNC());
	  std::cout<<"GLOBAL "<<id<<",FUNC\n"; 
 	}
        _pf.LABEL(id);
        std::cout<<"LABEL "<<id<<"\n";
     }

     gr8::locals_size_calculator *calculator = new gr8::locals_size_calculator(_compiler);
     node->block()->accept(calculator, 0);
     int size_of_vars = calculator->size();
     delete calculator;
 
     _pf.ENTER(size_of_vars);
     std::cout<<"ENTER "<<size_of_vars<<"\n";
   
     _offset = 8;
     if(node->variables()){
 	 _funcarg = true;
         for (size_t i = 0; i < node->variables()->size(); i++) {
           if (node->variables()->node(i) != nullptr) {   
             auto *decl = dynamic_cast <gr8::variable_node*> (node->variables()->node(i)); 
             symbol->addarg(decl->type());
           }
         } 
	 node->variables()->accept(this, lvl + 2);
         _funcarg = false;
     }
     _offset = 0;
     node->block()->accept(this, lvl+2);
  
     _pf.ALIGN();
     std::cout<<"ALIGN\n";
     _pf.LABEL(mklbl(_return));
     std::cout<<"LABEL "<<mklbl(_return)<<"\n";

     if(node->type()->name() != basic_type::TYPE_VOID){
       if(node->type()->size() == 4){
          _pf.STFVAL32();
          std::cout<<"STFVAL32\n";
       }
       else{
         _pf.STFVAL64();
         std::cout<<"STFVAL64\n";
       }
     }
     _pf.LEAVE();
     _pf.RET();
     std::cout<<"LEAVE\nRET\n";

     _infnc = false;

   }
   else{
     const std::string &id = node->identifier();

     auto symbol = _symtab.find(id);

     if( symbol != nullptr) {
       return;
     }
     symbol = std::make_shared<gr8::symbol>(node->type(), node->identifier(), 0, node->scope());
     _symtab.insert(id, symbol);
    
     _symtab.push();
     if(node->variables()){
       for(auto var: node->variables()->nodes()){
	  auto *declaration = dynamic_cast <gr8::variable_node*> (var);
 	  symbol->addarg(declaration->type());
       }
     }
     _symtab.pop();
     _declared.insert(id);
    
     if(id == std::string("argc") || id == std::string("envp") || id == std::string("argv")){
        return;
     }
     _pf.TEXT();
     _pf.ALIGN();
     std::cout<<"TEXT\nALIGN\n";
     _pf.GLOBAL(node->identifier(), _pf.FUNC());
  }
}

void gr8::postfix_writer::do_block_node(gr8::block_node *const node, int lvl) {
  _symtab.push();

  if(node->declarations()) { node->declarations()->accept(this, lvl+2); }
  if(node->instructions()) { node->instructions()->accept(this, lvl+2); }

  _symtab.pop();
}

void gr8::postfix_writer::do_address_node(gr8::address_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _adrs = true;
  node->lvalue()->accept(this, lvl+2);
  _adrs = false;
}

void gr8::postfix_writer::do_variable_node(gr8::variable_node *const node, int lvl) {
  const int Public = 1, Used = 0;
  ASSERT_SAFE_EXPRESSIONS;
 
  const std::string &id = node->identifier();
  
  int size = node->type()->size();

  auto symbol = std::make_shared<gr8::symbol>(node->type(),node->identifier(),0,Public);
  _symtab.insert(id, symbol);
  
  if(node->scope() == Used){
    _pf.EXTERN(node->identifier());
    return;
  }
  if(_infnc){ 
    if(!_funcarg){ 
      _offset -= size;
      symbol->value(_offset);
      symbol->visibility(2);
      if(node->initializeExpression()){
         node->initializeExpression()->accept(this, lvl+2);
         _pf.LOCAL(_offset);
         std::cout<<"LOCAL "<<_offset<<"\n";
         if(node->type()->name() == basic_type::TYPE_INT){
	   _pf.STINT();
           std::cout<<"STINT\n";
         }
         else if(node->type()->name() == basic_type::TYPE_DOUBLE){
	   _pf.STDOUBLE();
           std::cout<<"STDOUBLE\n";
         }
       }
    }
    else { 
      symbol->value(_offset);
      _offset += size;
      symbol->visibility(2);
    }
  }else{
    _global = true;
    if(node->type()->name() == basic_type::TYPE_STRING){	
       if(node->initializeExpression()){ 
          node->initializeExpression()->accept(this, lvl + 2);
 	  std::cout<<"DATA\nALIGN\nGLOBAL "<<id<<"\nLABEL "<<id<<"\nSADDR "<<_lststradr<<"\n";
          _pf.DATA();
	  _pf.ALIGN();
 	  _pf.GLOBAL(id, _pf.OBJ());
	  _pf.LABEL(id);
	  _pf.SADDR(_lststradr);
       }
       else{ 
	  std::cout<<"BSS\nALIGN\nGLOBAL "<<id<<"\nLABEL "<<id<<"\nSALLOC 4\n";
 	  _pf.BSS();
	  _pf.ALIGN();
   	  _pf.GLOBAL(id, _pf.OBJ());
	  _pf.LABEL(id);
	  _pf.SALLOC(4);
       }
    }
    else{
      if(node->initializeExpression()){ 
        std::cout<<"DATA\n";
        _pf.DATA();
      }
      else{
  	std::cout<<"BSS\n";
        _pf.BSS();
      }
      _pf.ALIGN();
      _pf.GLOBAL(id, _pf.OBJ());
      _pf.LABEL(id);
      std::cout<<"ALIGN\nGLOBAL "<<id<<"\nLABEL "<<id<<"\n";
      if(node->initializeExpression()){
         node->initializeExpression()->accept(this, lvl+2);
      }
      else{
	 std::cout<<"\nSALLOC 4\n";
         _pf.SALLOC(node->type()->size());
      }
    }
    _global = false;
  }
}

void gr8::postfix_writer::do_index_node(gr8::index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->base()->accept(this, lvl+2);
  node->index()->accept(this, lvl+2);
  _pf.INT(node->base()->type()->subtype()->size());
  _pf.MUL();
  _pf.ADD();
}


void gr8::postfix_writer::do_post_node(gr8::post_node *const node, int lvl) {
  node->argument()->accept(this, lvl);
  if (node->argument()->type()->name() == basic_type::TYPE_INT ) {
    std::cout<<"CALL printi\n";
    _pf.CALL("printi");
    _pf.TRASH(4);
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    std::cout<<"CALL prints\n";
    _pf.CALL("prints");
    _pf.TRASH(4);
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    std::cout<<"CALL printd\n";
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
  else {
    throw std::string("Print not of type int, double or string");
  }
  std::cout<<"CALL println\n";
  _pf.CALL("println");
}



void gr8::postfix_writer::do_sweep_by_node(gr8::sweep_by_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int start = ++_lbl;
  int end = ++_lbl;

  _stops.push(end);
  _agains.push(end);
 
  node->byExpression()->accept(this, lvl + 2); 
  _pf.INT(0);
  std::cout<<"INT 0\n";
  _pf.LT();
  std::cout<<"LT\n";
  node->fromExpression()->accept(this, lvl + 2); 
  node->toExpression()->accept(this, lvl + 2);  
  _pf.GT();
  std::cout<<"GT\n";

  _pf.XOR();
  std::cout<<"XOR\n";

  _pf.JNZ(mklbl(end));
  std::cout<<"JNZ "<<mklbl(end)<<"\n";

  node->fromExpression()->accept(this, lvl + 2); 
  node->lvalue()->accept(this, lvl + 2);
  _pf.STINT();
  std::cout<<"STINT\n";
 
  _pf.ALIGN();
  std::cout<<"ALIGN\n";
  _pf.LABEL(mklbl(start));
  std::cout<<"LABEL "<<mklbl(start)<<"\n";

  node->doBlock()->accept(this, lvl + 2);

  std::cout << "here\n";

  node->toExpression()->accept(this, lvl + 2);
  
  node->lvalue()->accept(this, lvl + 2);
  _pf.LDINT();
  std::cout<<"LDINT\n";
  _pf.EQ();
  std::cout<<"EQ\n";
  _pf.JNZ(mklbl(end));
  std::cout<<"JNZ "<<mklbl(end)<<"\n";

  node->lvalue()->accept(this, lvl + 2);
  _pf.LDINT();
  std::cout<<"LDINT\n";

  node->byExpression()->accept(this, lvl + 2);
  
  _pf.ADD();
  std::cout<<"ADD\n";
  node->lvalue()->accept(this, lvl + 2);
  _pf.STINT();
  std::cout<<"STINT\n";

  _pf.JMP(mklbl(start));
  std::cout<<"JMP "<<mklbl(start)<<"\n";

  _pf.LABEL(mklbl(end));
  std::cout<<"LABEL "<<mklbl(end)<<"\n";

  _agains.pop();
  _stops.pop();
  
}

void gr8::postfix_writer::do_again_node(gr8::again_node *const node, int lvl) {
  if (_agains.empty()) {
    throw std::string("out of cycles");
  } 
  _pf.JMP( mklbl(_agains.top()) );
}

void gr8::postfix_writer::do_stop_node(gr8::stop_node *const node, int lvl) {
  if (_stops.empty()) {
    throw std::string("out of cycles");
  } 
  _pf.JMP(mklbl(_stops.top()));
}

void gr8::postfix_writer::do_tweet_node(gr8::tweet_node *const node, int lvl) {
  node->expression()->accept(this, lvl);
  if (node->expression()->type()->name() == basic_type::TYPE_INT ) {
    std::cout<<"CALL printi\n";
    _pf.CALL("printi");
    _pf.TRASH(4); 
  } else if (node->expression()->type()->name() == basic_type::TYPE_STRING) {
    std::cout<<"CALL prints\n";
    _pf.CALL("prints");
    _pf.TRASH(4); 
  } else if (node->expression()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8); 
  }
  else {
    throw std::string("Print not of type int, double or string");
  }
}

void gr8::postfix_writer::do_return_node(gr8::return_node *const node, int lvl) {
  node->expression()->accept(this, lvl + 2);
  _pf.JMP(mklbl(_return));
  std::cout<<"JMP "<<mklbl(_return)<<"\n";
}

void gr8::postfix_writer::do_function_call_node(gr8::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS
  const std::string &id = node->identifier();
  auto symb = _symtab.find(id);
  int funcsize = node->type()->size();
  int size = 0;
  if (node->arguments()) {
    for (int i = node->arguments()->size() - 1; i >= 0; i--) {
      node->arguments()->node(i)->accept(this, lvl+2);
      /*if( ((cdk::expression_node*)node->arguments()->node(i))->type()->name() == basic_type::TYPE_INT 
		&& symb->argument(i)->name() == basic_type::TYPE_DOUBLE){
	 _pf.I2D();
         std::cout<<"I2D\n";
      }*/
      size += ((cdk::expression_node *) node->arguments()->node(i))->type()->size();
    }
  }
  _pf.CALL(id);
  std::cout<<"CALL "<<id<<"\n";
  _pf.TRASH(size);
  std::cout<<"TRASH "<<size<<"\n";
  if (funcsize  == 4 && node->type()->name() != basic_type::TYPE_VOID) {
    _pf.LDFVAL32();
    std::cout<<"LDFVAL32\n";
  } else if(funcsize  == 8 && node->type()->name() != basic_type::TYPE_VOID){
    _pf.LDFVAL64();
    std::cout<<"LDFVAL64\n";
  }
}
void gr8::postfix_writer::do_identity_node(gr8::identity_node *const node, int lvl) {
  std::cout<<"identity \n";
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->argument()->accept(this, lvl+2);
  if (node->type()->name() == basic_type::TYPE_DOUBLE) { 
    std::cout<<"DUP64\n";
    _pf.DUP64();
  }else{
    _pf.DUP32();
    std::cout<<"DUP32\n";
  }
  _pf.INT(0);  
  std::cout<<"INT "<<0<<"\n";                       
  if (node->type()->name() == basic_type::TYPE_DOUBLE) { 
    _pf.I2D(); 
    _pf.DCMP();
    _pf.INT(0);
  }         
  _pf.LE();    
  std::cout<<"LE\n";                      
  _pf.JZ(mklbl(lbl));
  std::cout<<"JZ "<<mklbl(lbl)<<"\n";
  if (node->type()->name() == basic_type::TYPE_DOUBLE) { 
    _pf.DNEG(); 
    std::cout<<"DNEG\n"; 
  }else{
    _pf.NEG(); 
    std::cout<<"NEG\n"; 
  }          
  std::cout<<"ALIGN\nLABEL "<<mklbl(lbl)<<"\n";         
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void gr8::postfix_writer::do_allocation_node(gr8::allocation_node *const node, int lvl) {  // not complete
   ASSERT_SAFE_EXPRESSIONS;
   _pf.INT(8);
   std::cout<<"INT 8\n";
   node->expression()->accept(this, lvl+2);
   _pf.MUL();
   _pf.ALLOC();
   _pf.SP();
   std::cout<<"MUL\nALLOC\nSP\n";
}

void gr8::postfix_writer::do_null_node(gr8::null_node *const node, int lvl) {
  // EMPTY
}




