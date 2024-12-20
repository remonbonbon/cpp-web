#include <sstream>
#include <stdexcept>

#include <httplib.h>

#include "./utils/logger.hpp"

#include "./jobs/create.hpp"
#include "./jobs/get.hpp"
#include "./jobs/list.hpp"

int main(void) {
  httplib::Server svr;

  //-------------------------------------
  // GET /api/jobs
  svr.Get("/api/jobs",
          [](const auto &req, auto &res) { jobs::list(req, res); });

  //-------------------------------------
  // POST /api/jobs/:id
  svr.Post("/api/jobs/:id",
           [](const auto &req, auto &res) { jobs::create(req, res); });

  //-------------------------------------
  // GET /api/jobs/:id
  svr.Get("/api/jobs/:id",
          [](const auto &req, auto &res) { jobs::get(req, res); });

  //-------------------------------------
  // OPTIONS /api/*
  svr.Options(R"(/api/(.+))", [](const auto &req, auto &res) {
    res.set_content("", "application/json");
  });

  //-------------------------------------
  // Allow CORS for all
  svr.set_post_routing_handler([](const auto &req, auto &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
  });

  //-------------------------------------
  // Mount static file server
  svr.set_mount_point("/", "../public");
  svr.set_mount_point("/data", "./data");

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
