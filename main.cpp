#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[]) {
  using namespace boost::asio;
  boost::system::error_code ec;

  if(argc != 3) {
    std::cerr << "Error: Missing arguments.\n"
      << "Call like so: ./web_getter www.example.com / \n";
    return -1;
  }

  // what we need
  io_service svc;
  ssl::context ctx(ssl::context::method::sslv23_client);
  ssl::stream<ip::tcp::socket> ssock(svc, ctx);

  ip::tcp::resolver resolver(svc);
  ip::tcp::resolver::query query(argv[1], "https");
  ip::tcp::resolver::iterator iter = resolver.resolve(query);
  ip::tcp::resolver::iterator end; // End marker.
  ip::tcp::endpoint endpoint = *iter;
  std::cout << endpoint << std::endl;

  // Connect with the endpoint and exchange cryptographic content
  ssock.lowest_layer().connect(endpoint);
  ssock.handshake(ssl::stream_base::handshake_type::client);

  // send request
  std::ostringstream oss;
  oss << "GET " << argv[2] << " HTTP/1.1\r\n"
      << "Host: " << argv[1] << "\r\n"
      << "Accept: text/html\r\n"
      << "Connection: close\r\n\r\n";
  std::string request = oss.str();
  boost::asio::write(ssock, buffer(request));

  // read response
  std::string response;
  do {
    char buf[8192];
    size_t bytes_transferred = ssock.read_some(buffer(buf), ec);
    if (!ec) {
      std::cout << "Received: " << bytes_transferred << "\n";
      response.append(buf, buf + bytes_transferred);
    }
  } while (!ec);

  // print and exit
  std::cout << "Response received: \n" << response << "\n";
}
