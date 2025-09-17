#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "crow.h"
#include "../include/maze.hpp"

int main() {
    crow::SimpleApp app;

    Maze maze;

    CROW_ROUTE(app, "/")([&] {
        std::ifstream file("/usr/local/bin//static/index.html");
        if (!file.is_open()) {
            return crow::response(500, "index.html not found");
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/start")([&](const crow::request& req) {
        auto& params = req.url_params;

        if (!params.get("rows") || !params.get("cols")) {
            return crow::response(400, "Missing parameters");
        }

        int rows = std::stoi(params.get("rows"));
        int cols = std::stoi(params.get("cols"));

        try {
            maze = Maze(rows, cols);
        } catch (const std::exception& e) {
            return crow::response(400, "Invalid dimensions");
        }

        crow::response res;
        res.code = 302;
        res.set_header("Location", "/maze");
        return res;
    });

    CROW_ROUTE(app, "/maze")([] {
        std::ifstream file("/usr/local/bin/static/maze.html");
        if (!file.is_open()) {
            return crow::response(500, "maze.html not found");
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html; charset=utf-8");
        return res;
    });

    CROW_ROUTE(app, "/move/<int>/<int>")
    ([&](int dx, int dy) {
        try {
            maze.move_player(dx, dy);

            std::ostringstream oss;
            oss << maze;

            crow::json::wvalue result;
            result["maze"] = oss.str();
            result["done"] = maze.is_finished();

            return crow::response(result);
        } catch (const std::exception& e) {
            return crow::response(500, "Move failed: " + std::string(e.what()));
        }
    });

    CROW_ROUTE(app, "/static/<string>")
    ([](std::string filename) {
        std::ifstream file("/usr/local/bin/static/" + filename);
        if (!file.is_open()) return crow::response(404, "File not found");

        std::ostringstream buffer;
        buffer << file.rdbuf();
        crow::response res(buffer.str());

        if (filename.ends_with(".js")) res.set_header("Content-Type", "application/javascript");
        else if (filename.ends_with(".css")) res.set_header("Content-Type", "text/css");
        else if (filename.ends_with(".html")) res.set_header("Content-Type", "text/html");

        return res;
    });

    CROW_ROUTE(app, "/maze-data")([&] {
        std::ostringstream oss;
        oss << maze;
        return crow::response(oss.str());
    });

    CROW_ROUTE(app, "/win")([] {
        std::ifstream file("/usr/local/bin/static/win.html");
        if (!file.is_open()) {
            return crow::response(500, "win.html not found");
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html; charset=utf-8");
        return res;
    });

    app.port(8080).multithreaded().run();
    return 0;
}