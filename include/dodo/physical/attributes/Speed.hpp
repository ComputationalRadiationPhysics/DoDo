#pragma once

namespace dodo
{
namespace physical
{
namespace attributes
{

struct Speed
{
    double value;

	std::string toString() const
	{
		return std::to_string(value);
	}

	void updateValue()
	{
		value = 42.;
	}
};



}
}
}
