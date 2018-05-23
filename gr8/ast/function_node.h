#ifndef __GR8_FUNCTIONNODE_H__
#define __GR8_FUNCTIONNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>

namespace gr8 {
    class function_node : public cdk::basic_node {
	int _scope;
        basic_type *_type;
        std::string _identifier;
        cdk::sequence_node *_variables;
	cdk::basic_node *_block;

    public:
        /*function_node(int lineno, basic_type *type, std::string &identifier, cdk::sequence_node *variables, cdk::basic_node *block)
            : cdk::basic_node(lineno), _type(type), _identifier(identifier), _variables(variables) { }

	function_node(int lineno, basic_type *type, std::string *identifier, cdk::sequence_node *variables, cdk::basic_node *block)
            : cdk::basic_node(lineno), _type(type), _identifier(*identifier), _variables(variables), _block(block) { }*/

        function_node(int lineno, basic_type *type, std::string &identifier, cdk::sequence_node *variables, cdk::basic_node *block, int scope)
            : cdk::basic_node(lineno), _scope(scope), _type(type), _identifier(identifier), _variables(variables), _block(block) { }

	function_node(int lineno, basic_type *type, std::string *identifier, cdk::sequence_node *variables, cdk::basic_node *block, int scope)
            : cdk::basic_node(lineno), _scope(scope), _type(type), _identifier(*identifier), _variables(variables), _block(block)  { }

	int scope(){
	    return _scope;
	}

        basic_type *type() {
            return _type;
        }

        std::string identifier() {
            return _identifier;
        }

        cdk::sequence_node *variables() {
            return _variables;
        }
	
	cdk::basic_node *block(){
   	    return _block;
	}

        void accept(basic_ast_visitor *sp, int level) {
            sp->do_function_node(this, level);
        }
    };
}

#endif
