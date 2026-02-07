#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <iostream>

namespace asio = boost::asio;
namespace http = boost::beast::http;
using tcp = asio::ip::tcp;

http::response<http::string_body> make_response(const http::status status_code, const std::string& content_type,
    const std::string& body, const http::request<http::string_body>& request) {
    http::response<http::string_body> response{status_code, request.version()};
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, content_type);
    response.keep_alive(request.keep_alive());
    response.body() = body;
    response.prepare_payload();
    return response;
}

http::response<http::string_body> make_json_response(const http::request<http::string_body>& request) {
    return make_response(http::status::ok, "application/json",
        boost::json::serialize(
            boost::json::object{{"confession", "This is the strangest way to tell her you love her"}}),
        request);
}

http::response<http::string_body> make_not_found(const http::request<http::string_body>& request) {
    return make_response(http::status::not_found, "text/plain", "Resource not found", request);
}

int main() {
    try {
        asio::io_context io_context{1};
        tcp::acceptor acceptor{io_context, {asio::ip::make_address("0.0.0.0"), 8080}};
        std::cout << "open localhost:8080" << '\n';
        while (true) {
            tcp::socket socket{io_context};
            acceptor.accept(socket);

            boost::beast::flat_buffer buffer;
            http::request<http::string_body> request;
            http::read(socket, buffer, request);

            const http::response<http::string_body> response =
                (request.method() == http::verb::get && request.target() == "/") ? make_json_response(request)
                                                                                 : make_not_found(request);

            http::write(socket, response);

            socket.shutdown(tcp::socket::shutdown_send);
        }
    } catch (const std::exception& exception) {
        std::cerr << "Error: " << exception.what();
        return 1;
    }
}
