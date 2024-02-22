#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main(void) {
  httplib::Server svr;

  // Logger
  svr.set_logger([](const Request &req, const Response &res) {
    std::cout << res.status << " " << req.method << " " << req.path
              << std::endl;
  });

  //-------------------------------------
  // GET /
  // Hello World!
  svr.Get("/", [](const Request &req, Response &res) {
    std::stringstream html;
    html << "<h1>Hello World!</h1>";

    html << "<pre>";
    const json headersJson(req.headers);
    const std::string jsonStr = headersJson.dump(2);
    html << jsonStr;
    html << "</pre>";

    res.set_content(html.str(), "text/html");
  });

  //-------------------------------------
  // GET /thread
  // Tests threading
  svr.Get("/thread", [](const Request &req, Response &res) {
    std::thread th([&]() {
      std::cout << "Sleeping for 3 second.." << std::endl;

      std::this_thread::sleep_for(
          std::chrono::milliseconds(3000)); // 3秒間スリープします

      std::cout << "3 second elapsed.." << std::endl;
    });
    th.detach(); // スレッドを投げっぱなしにする

    res.set_content("OK", "text/html");
  });

  //-------------------------------------
  std::cout << "listen http://localhost:8080/" << std::endl;
  svr.listen("0.0.0.0", 8080);
}
