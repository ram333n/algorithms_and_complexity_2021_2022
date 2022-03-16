#pragma once
#include <iostream>
#include <numeric>

class ComplexNumber {
public:
	ComplexNumber(); //������������
	ComplexNumber(int real, int imaginary);
	
	int GetRealPart() const; //������� ��� ��������� ����� �� ����� ������� ����. �����
	int GetImaginaryPart() const;
private:
	int real_, imaginary_;  //�����, ����� �������
};

bool operator==(ComplexNumber lhs, ComplexNumber rhs);//�������� == ��� ���������
std::ostream& operator<<(std::ostream& os, ComplexNumber number); //�������� << ��� ��������� � �������

class RationalNumber {
public:
	RationalNumber();
	RationalNumber(int numerator, int denominator);

	int GetNumerator() const;
	int GetDenominator() const;

	friend RationalNumber operator-(RationalNumber lhs, RationalNumber rhs);
	friend bool operator<(RationalNumber lhs, RationalNumber rhs);
	friend bool operator==(RationalNumber lhs, RationalNumber rhs);
	friend std::ostream& operator<<(std::ostream& os, RationalNumber num);

private:
	int numerator_, denominator_;
};