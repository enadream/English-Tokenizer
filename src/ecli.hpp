#ifndef ECLI_HPP
#define ECLI_HPP

#include <string>
#include <iostream>

// User defined libs
#include "misc/data_types.hpp"
#include "misc/string.hpp"

#define PARAM_NAME_SIZE 10
#define PARAM_VALUE_SIZE 10

typedef struct Parameter
{
	String name;
	String value;

	Parameter() = default;

	Parameter(uint32 name_s, uint32 value_s) {
		name.NewMemory(name_s);
		value.NewMemory(value_s);
	}
}Parameter;


class Ecli {
private:
	int8 ParseParameters(const String& line, Parameter* out_params) const;
	int8 GetParamIdWName(const char* param, const Parameter* params, int8 search_range) const; // Returns the index of parameter
	void PrintParams(Parameter* params, int8 amount);
public:
	void Start(); // Command Line Interface
};

#endif 
