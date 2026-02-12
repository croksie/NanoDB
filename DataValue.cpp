#include <iostream>
#include "DataValue.h"


//String Value
StringValue::StringValue(const std::string& Value){
	this->Value = Value;
}

void StringValue::display() {
	std::cout << this -> Value << std::endl;
}

//Int Value
IntValue::IntValue(const int& Value) {
	this->Value = Value;
}

void IntValue::display() {
	std::cout << Value << std::endl;
}


//List Value
ListValue::ListValue(const std::list<std::string>& Value) {
	this->Value = Value;
}

void ListValue::display() {
	std::cout << "test" << std::endl;
}