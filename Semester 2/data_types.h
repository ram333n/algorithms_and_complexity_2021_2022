#pragma once
#include <iostream>

class ComplexNumber {
public:
	ComplexNumber();
	ComplexNumber(int real, int imaginary);
	
	int GetRealPart() const;
	int GetImaginaryPart() const;
private:
	int real_, imaginary_;
};

bool operator==(ComplexNumber lhs, ComplexNumber rhs);
std::ostream& operator<<(std::ostream& os, ComplexNumber number);