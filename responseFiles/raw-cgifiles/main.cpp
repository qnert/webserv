#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

int main(void){
  std::cout << "Eine Ente hat dein Geburtstagsgeschenk geklaut ich hoffe du weißt den Zauberspruch, um die Ente zu rufen!\n" << std::endl;
  std::string msg;
  std::string duck = "   (@_   ---> Happy Birthday ---> CODE\n\\\\\\_\\\n<____)";
  std::cout << "Zauberspruch: ";
  std::getline(std::cin, msg);
  std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);
  if (msg == "biele biele biele")
    std::cout << duck  << std::endl;
  else
    std::cout << "Denk nochmal genauer nach... " << "\n";
}
