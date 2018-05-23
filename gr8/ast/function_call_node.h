#ifndef __GR8_FUNCTIONCALLNODE_H__
#define __GR8_FUNCTIONCALLNODE_H__

#include <cdk/basic_type.h>
#include <cdk/ast/basic_node.h>

namespace gr8 {
    class function_call_node : public cdk::expression_node {
        std::string _identifier;
        cdk::sequence_node *_arguments;

    public:
        function_call_node(int lineno
                          , std::string &identifier
                          , cdk::sequence_node *arguments)
            : cdk::expression_node(lineno)
            , _identifier(identifier)
            , _arguments(arguments){ }
	function_call_node(int lineno
                          , std::string *identifier
                          , cdk::sequence_node *arguments)
            : cdk::expression_node(lineno)
            , _identifier(*identifier)
            , _arguments(arguments){ }

        std::string identifier() {
            return _identifier;
        }
	
	cdk::sequence_node *arguments(){
   	    return _arguments;
	}

        void accept(basic_ast_visitor *sp, int level) {
            sp->do_function_call_node(this, level);
        }
    };
} //gr8

#endif
