#ifndef __GR8_SEMANTICS_SYMBOL_H__
#define __GR8_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace gr8 {

  class symbol {
    basic_type *_type;
    std::string _name;
    long _value; // hack!
    std::vector<basic_type*>_arguments;
    int _visibility;

  public:
    symbol(basic_type *type, const std::string &name, long value, int visibility) :
        _type(type), _name(name), _value(value), _visibility(visibility) {
    }

    virtual ~symbol() {
      delete _type;
    }
   
    int visibility() {
	return _visibility;
    }
    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    long value() const {
      return _value;
    }
    long value(long v) {
      return _value = v;
    }
    void visibility(long v) {
       _visibility = v;
    }
    std::vector<basic_type*> arguments(){
      return _arguments;
    }
    void addarg(basic_type *type){
      _arguments.push_back(type);
    }
    basic_type* argument(int i){
      return _arguments.at(i);
    }
  };

} // gr8

#endif
