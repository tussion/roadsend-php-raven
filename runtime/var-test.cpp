
/*

playing around with boost::variant, to see if it's suitable to store php vals

*/

#include "boost/variant.hpp"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <typeinfo>

// just a demo class, we'd also have php_object
class php_hash {
    private:
        int size;
    public:
        php_hash(int sizevar) : size(sizevar) { }

        php_hash(const php_hash& h) {
            size = h.size;
        }
        int getSize() const { return size; }

};

std::ostream& operator << (std::ostream& os, const php_hash& h)
{
    return os << "php_hash" << std::endl;
}


// using the c++ bool as a type seems to cause problems, so we implement php bool
// as an int. note that php integers are type long
// we also use in to decide if this value is NULL. unfortunately a null value would evaluate
// to true (in c++, since it's non 0). can we do this better?
typedef int p3state;
#define PFALSE 0
#define PTRUE  1
#define PNULL -1

// this would be the typedef for a generic php variable in our runtime
typedef boost::variant< p3state, long, double, std::string, php_hash> pvar;

// used for determining type at runtime
typedef enum { PVAR_NULL, PVAR_BOOL, PVAR_LONG, PVAR_DOUBLE, PVAR_STRING, PVAR_HASH } pvartype;

// generic visitor which can be applied to any pvar
// correct method runs based on the appropriate type
class my_visitor : public boost::static_visitor<int>
{
public:
    int operator()(const p3state &i) const
    {
    	if (i >= 0) {
    		std::cout << "i see a bool" << std::endl;
    	}
    	else {
    		std::cout << "i see a null" << std::endl;
    	}
        return i;
    }

    int operator()(const long &i) const
    {
        std::cout << "i see a long" << std::endl;
        return i;
    }

    int operator()(const double &i) const
    {
        std::cout << "i see a float" << std::endl;
        return 0;
    }

    int operator()(const std::string &str) const
    {
        std::cout << "i see a string" << std::endl;
        return str.length();
    }

    int operator()(const php_hash &h) const
    {
        std::cout << "i see a php_hash" << std::endl;
        return 0;
    }

};

// a visitor for determining type
class type_checker : public boost::static_visitor<int>
{
public:
    pvartype operator()(const long &i) const
    {
        return PVAR_LONG;
    }

    pvartype operator()(const double &i) const
    {
		return PVAR_DOUBLE;
    }

    pvartype operator()(const std::string &str) const
    {
    	return PVAR_STRING;
    }

    pvartype operator()(const php_hash &h) const
    {
    	return PVAR_HASH;
    }

    pvartype operator()(const p3state &h) const
    {
    	return (h >= 0) ? PVAR_BOOL : PVAR_NULL;
    }

};

// a visitor for converting to a num
class convert_to_num_visitor : public boost::static_visitor<>
{
protected:
  pvar &var;
public:
  convert_to_num_visitor(pvar &v) : var(v) {}

  void operator()(const std::string &a) const {
    try {
      var = boost::lexical_cast<long>(a);
    } catch(boost::bad_lexical_cast &) {
      var = 0l;
    }
  }

  void operator()(const double &i) const
  {
	  // nothing, already numeric
  }

  void operator()(const php_hash &h) const
  {
	  var = (long)h.getSize();
  }

  void operator()(const p3state &h) const
  {
	  (h <= 0) ? var = 0l : var = 1l;
  }

  void operator()(const long &a) const {
    // Do nothing - already correct type
  }
};


// driver
int main()
{
    pvar u;

    // string
    u = "hello world there";

    std::cout << u << std::endl;
    int result = boost::apply_visitor( my_visitor(), u );

    // long
    u = 15l;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // float
    u = 2.3123;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // bool
    u = PTRUE;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    if (boost::get<p3state>(u) == PTRUE) {
    	std::cout << "the bool was true" << std::endl;
    }

    // null
    u = PNULL;

    std::cout << u << std::endl;
    result = boost::apply_visitor( my_visitor(), u );

    // php hash
    php_hash h(5);
    std::cout << h;
    u = h;
    std::cout << u << std::endl;

    result = boost::apply_visitor( my_visitor(), u );

    ////

    // type checking?
    int pt = boost::apply_visitor( type_checker(), u );
    switch (pt) {
    case PVAR_HASH:
    	std::cout << "found a hash" << std::endl;
    	break;
    case PVAR_DOUBLE:
    	std::cout << "found a float" << std::endl;
    	break;
    default:
    	std::cout << "woops, what type was it?" << std::endl;
    }
    // RTTI: output is ugly, it also might not be available everywhere
    std::cout << typeid( u ).name() << std::endl;

    ////

    // type conversion
    std::cout << "type conversion:" << std::endl;
    u = "55";
    boost::apply_visitor( my_visitor(), u );
    boost::apply_visitor( convert_to_num_visitor(u), u );
    boost::apply_visitor( my_visitor(), u );
    std::cout << "final: " << u << std::endl;

}
