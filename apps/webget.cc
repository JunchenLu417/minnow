#include "socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>

#include <format>

using namespace std;

void get_URL( const string& host, const string& path )
{
  // cerr << "Function called: get_URL(" << host << ", " << path << ")\n";  // get_URL(cs144.keithw.org, /hello)
  // cerr << "Warning: get_URL() has not been implemented yet.\n";

  TCPSocket tcpsocket;
  tcpsocket.connect(Address(host, "http"));  // set up the connection -> telnet cs144.keithw.org http
  tcpsocket.write(format("GET {} HTTP/1.1\r\n", path));
  tcpsocket.write(format("Host: {}\r\n", host));
  tcpsocket.write("Connection: close\r\n");  // server can close the connection after replying
  tcpsocket.write("\r\n");  // done with http request

  while (true) {
    string response;
    tcpsocket.read(response);  // until EOF, print server's response to console
    if (!response.empty())
      cout << response;
    else
      break;
  }
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
