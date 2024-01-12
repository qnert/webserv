#include "../includes/Server.hpp"

int main(void)
{
  try
  {
    Server httpServer;
    httpServer.serverLoop();
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
  return (0);
}
