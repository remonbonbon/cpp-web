#include <iostream>

#include <httplib.h>
using namespace httplib;

int main(void) {
  Server svr;

  svr.Get("/hi", [](const Request &req, Response &res) {
    std::cout << req.method << " " << req.path << std::endl;
    res.set_content("Hello World!", "text/plain");
  });

  std::cout << "listen on 8080" << std::endl;
  svr.listen("0.0.0.0", 8080);
}
