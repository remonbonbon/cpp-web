#include <sstream>
#include <stdexcept>
#include <thread>

#include <httplib.h>

#include "./jobs/create.hpp"
#include "./utils/logger.hpp"

int main(void) {
  httplib::Server svr;

  //-------------------------------------
  // POST /api/jobs/:id
  svr.Post("/api/jobs/:id",
           [](const auto &req, auto &res) { jobs::create(req, res); });

  //-------------------------------------
  // OPTIONS /api/*
  svr.Options(R"(/api/(.+))", [](const auto &req, auto &res) {
    res.set_content("", "application/json");
  });

  //-------------------------------------
  // GET /thread
  // Tests threading
  svr.Get("/thread", [](const auto &req, auto &res) {
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
  // Allow CORS for all
  svr.set_post_routing_handler([](const auto &req, auto &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
  });

  //-------------------------------------
  // Mount static file server
  svr.set_mount_point("/public", "../public");

  //-------------------------------------
  // Logger
  svr.set_logger([](const auto &req, const auto &res) {
    if (200 <= res.status && res.status < 400) {
      logger::info(res.status, req.method, req.path);
    } else if (400 <= res.status && res.status < 500) {
      logger::warn(res.status, req.method, req.path);
    } else {
      logger::error(res.status, req.method, req.path);
    }
  });

  //-------------------------------------
  // Error handling
  svr.set_exception_handler([](const auto &req, auto &res, auto ep) {
    res.status = 500;
    try {
      std::rethrow_exception(ep);
    } catch (std::exception &e) {
      logger::error(e.what());
    } catch (...) {
      logger::error("Unknown Exception");
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
  svr.set_error_handler([](const auto &req, auto &res) {
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
      // DROP others
    }
  });

  //-------------------------------------
  std::cout << "listen http://localhost:8080/" << std::endl;
  svr.listen("0.0.0.0", 8080);
}
