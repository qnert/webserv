#include "../../includes/Statuscodes.hpp"

std::string const&  Statuscodes::operator[](int const& key){
  return (this->data[key]);
}

Statuscodes::Statuscodes(){
  std::string buff;
  std::ifstream file("./data/status.csv");

  if (!file.is_open())
    std::runtime_error("Coudln't open status.csv!\n");
  std::getline(file, buff);
  if (buff != "STATUSCODE|MEANING")
    std::runtime_error("Invalid header in status.csv!\n");
  while (std::getline(file, buff)){
    this->data[std::atoi(buff.substr(0, buff.find_first_of("|")).c_str())] = buff.substr(buff.find_first_of("|") + 1, buff.size() - 1);
  }
  file.close();
}

Statuscodes::Statuscodes(Statuscodes const& other){
  *this = other;
}

Statuscodes& Statuscodes::operator=(Statuscodes const& other){
  this->data = other.data;
  return (*this);
}

Statuscodes::~Statuscodes(){}
