#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cppcodec/base64_rfc4648.hpp>
using base64 = cppcodec::base64_rfc4648;

int main(void) {
  httplib::Server svr;

  // Logger
  svr.set_logger([](const Request &req, const Response &res) {
// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */
    if (200 <= res.status && res.status < 400) {
      std::cout << GREEN;
    } else if (400 <= res.status && res.status < 500) {
      std::cout << YELLOW;
    } else {
      std::cout << RED;
    }
    std::cout << res.status << " " << req.method << " " << req.path;
    std::cout << RESET << std::endl;
  });

  // Mount static file server
  svr.set_mount_point("/public", "../public");

  // Error handling
  svr.set_error_handler([](const Request &req, Response &res) {
    std::stringstream body;
    if (0 == req.path.rfind("/api/", 0)) {
      // API route
      if (400 <= res.status && res.status < 500) {
        body << R"({"message":"not found"})";
      } else {
        body << R"({"message":"system error"})";
      }
      res.set_content(body.str(), "application/json");
    } else {
      // other route
      // => DROP request
    }
  });
  svr.set_exception_handler(
      [](const Request &req, Response &res, std::exception_ptr ep) {
        res.status = 500;
        try {
          std::rethrow_exception(ep);
        } catch (std::exception &e) {
          std::cout << "[ERROR] " << e.what() << std::endl;
        } catch (...) {
          std::cout << "Unknown Exception" << std::endl;
        }

        std::stringstream body;
        if (0 == req.path.rfind("/api/", 0)) {
          // API route
          if (400 <= res.status && res.status < 500) {
            body << R"({"message":"not found"})";
          } else {
            body << R"({"message":"system error"})";
          }
          res.set_content(body.str(), "application/json");
        } else {
          // other route
          // => DROP request
        }
      });

  //-------------------------------------
  // GET /
  // Hello World!
  svr.Get("/", [](const Request &req, Response &res) {
    std::stringstream body;
    body << "<h1>Hello World!</h1>";

    body << "<pre>";
    const json headersJson(req.headers);
    const std::string jsonStr = headersJson.dump(2);
    body << jsonStr;
    body << "</pre>";

    res.set_content(body.str(), "text/html");
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
  // GET /error
  // Throws error
  svr.Get("/error", [](const Request &req, Response &res) {
    throw std::invalid_argument("MyFunc argument too large.");
  });

  //-------------------------------------
  // GET /api/error
  // Throws API error
  svr.Get("/api/error", [](const Request &req, Response &res) {
    throw std::invalid_argument("MyFunc argument too large.");
  });

  //-------------------------------------
  // OPTIONS /api/*
  // Allow all
  svr.Options(R"(/api/(.+))", [](const Request &req, Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
    res.set_content("", "application/json");
  });

  //-------------------------------------
  // POST /api/jobs
  svr.Post("/api/jobs", [](const Request &req, Response &res) {
    // std::cout << req.body << std::endl;
    const json reqBody = json::parse(req.body);
    const std::string imageBase64 = reqBody["image"];

    std::cout << "base64 size: " << imageBase64.length() << std::endl;
    std::vector<uint8_t> decoded = base64::decode(imageBase64);
    std::cout << "binary size: " << decoded.size() << std::endl;

    // std::ofstream f;
    // f.open("output.jpeg", std::ios::out | std::ios::binary);
    // f.write((char *)&decoded[0], decoded.size() * sizeof(decoded[0]));
    // f.close();

    std::stringstream body;
    body << R"({"message":"OK"})";
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
    res.set_content(body.str(), "application/json");
  });

  //-------------------------------------
  std::cout << "listen http://localhost:8080/" << std::endl;
  svr.listen("0.0.0.0", 8080);
}
