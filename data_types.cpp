#include "data_types.h"

ComplexNumber::ComplexNumber() : real_(0), imaginary_(0) {}

ComplexNumber::ComplexNumber(int real, int imaginary) : real_(real), imaginary_(imaginary) {}

int ComplexNumber::GetRealPart() const {
	return real_;
}

int ComplexNumber::GetImaginaryPart() const {
	return imaginary_;
}

bool operator==(ComplexNumber lhs, ComplexNumber rhs) {
	return lhs.GetRealPart() == rhs.GetRealPart() && lhs.GetImaginaryPart() == rhs.GetImaginaryPart();
}

std::ostream& operator<<(std::ostream& os, ComplexNumber number) {
	char sign = number.GetImaginaryPart() >= 0 ? '+' : '-';
	os << number.GetRealPart() << " " << sign << " " << abs(number.GetImaginaryPart()) << "i";
	return os;
}
