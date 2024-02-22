#include <iostream>
#include <sstream>

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main(void) {
  httplib::Server svr;

  svr.Get("/", [](const Request &req, Response &res) {
    std::cout << req.method << " " << req.path << std::endl;

    std::stringstream html;
    html << "<h1>Hello World!</h1>";

    html << "<pre>";
    const json headersJson(req.headers);
    const std::string jsonStr = headersJson.dump(2);
    html << jsonStr;
    html << "</pre>";

    res.set_content(html.str(), "text/html");
  });

  std::cout << "listen http://localhost:8080/" << std::endl;
  svr.listen("0.0.0.0", 8080);
}
