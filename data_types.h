#pragma once
#include <iostream>

class ComplexNumber {
public:
	ComplexNumber(); //конструктори
	ComplexNumber(int real, int imaginary);
	
	int GetRealPart() const; //функції для отримання дійсної та уявної частини комп. числа
	int GetImaginaryPart() const;
private:
	int real_, imaginary_;  //дійсна, уявна частини
};

bool operator==(ComplexNumber lhs, ComplexNumber rhs);//оператор == для порівняння
std::ostream& operator<<(std::ostream& os, ComplexNumber number); //оператор << для виведення у консоль