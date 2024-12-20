#ifndef _MY_JOBS_CREATE_
#define _MY_JOBS_CREATE_

#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#include <httplib.h>
using Request = httplib::Request;
using Response = httplib::Response;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <cppcodec/base64_rfc4648.hpp>
using base64 = cppcodec::base64_rfc4648;

#include "./_define.h"
#include "./_run.hpp"

namespace jobs {

void create(const Request &req, Response &res) {
  const std::string id = req.path_params.at("id");
  const json params = json::parse(req.body);
  const std::string filename = params["filename"];
  const std::string type = params["type"];
  const std::string dataBase64 = params["data"];

  // 画像ファイルパスを決定
  std::filesystem::path imgPath = DATA_DIR;
  {
    std::string ext;
    if (params["type"] == "image/jpeg") {
      ext = ".jpg";
    } else if (params["type"] == "image/png") {
      ext = ".png";
    } else {
      throw std::invalid_argument("Unsupported file type");
    }
    imgPath.append(id + ext);
  }

  // 画像ファイルを保存
  {
    // std::cout << "base64 size: " << dataBase64.length() << std::endl;
    std::vector<uint8_t> dataBinary = base64::decode(dataBase64);
    // std::cout << "binary size: " << dataBinary.size() << std::endl;

    std::ofstream f(imgPath, std::ios::out | std::ios::binary);
    f.write((char *)&dataBinary[0], dataBinary.size() * sizeof(dataBinary[0]));
    f.close();
  }

  // JSONファイルを作成
  json data;
  {
    std::filesystem::path jsonPath = DATA_DIR;
    jsonPath.append(id + ".json");

    const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::system_clock::now().time_since_epoch())
                         .count();

    data["id"] = id;
    data["filename"] = imgPath.filename();
    data["originalFilename"] = filename;
    data["type"] = type;
    data["createdAt"] = now;

    std::ofstream f(jsonPath);
    f << data.dump(2);
    f.close();
  }

  // 実行
  std::thread t(_runJob, id);
  t.detach(); // スレッドを投げっぱなしにする

  res.set_content(data.dump(), "application/json");
}

} // namespace jobs

#endif
