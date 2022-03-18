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


RationalNumber::RationalNumber() : numerator_(0), denominator_(1) {}

RationalNumber::RationalNumber(int numerator, int denominator) {
	if (denominator == 0) {
		throw std::invalid_argument("Denominator is 0");
	}

	if (numerator == 0) {
		numerator_ = 0;
		denominator_ = 1;
	}
	else {
		int gcd = std::gcd(numerator, denominator);
		bool is_positive = (numerator > 0 && denominator > 0) || (numerator < 0 && denominator < 0);
		denominator_ = std::abs(denominator) / gcd;
		numerator_ = (is_positive ? 1 : -1) * std::abs(numerator) / gcd;
	}
}

int RationalNumber::GetNumerator() const {
	return numerator_;
}

int RationalNumber::GetDenominator() const {
	return denominator_;
}

RationalNumber operator-(RationalNumber lhs, RationalNumber rhs) {
	return { lhs.numerator_ * rhs.denominator_ - rhs.numerator_ * lhs.denominator_,rhs.denominator_ * lhs.denominator_ };
}

bool operator<(RationalNumber lhs, RationalNumber rhs) {
	return (lhs - rhs).numerator_ < 0;
}

bool operator<=(RationalNumber lhs, RationalNumber rhs) {
	return (lhs - rhs).numerator_ <= 0;
}

bool operator==(RationalNumber lhs, RationalNumber rhs) {
	return (lhs.numerator_ == rhs.numerator_) && (lhs.denominator_ == rhs.denominator_);
}

std::ostream& operator<<(std::ostream& os, RationalNumber num) {
	os << num.numerator_ << '/' << num.denominator_;
	return os;
}