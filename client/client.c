#include "../common/all_imports.h"
#include "../common/definitions.h"
#include "../common/erase_from_memory.h"
#include "../common/exit_with_error.h"

#define USAGE "Usage: %s <hostname> <port_number>\n"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, USAGE, argv[0]);
    fprintf(stderr, "Please provide a host name and port number.\n");
    return EXIT_FAILURE;
  }

#ifdef _WIN32
  // Conditional compilation for Windows to ensure that the Winsock library is
  // initialized before use.

  printf("Initializing Winsock...\n");
  WSADATA wsaData;
  int iResult;

  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    return exit_with_error("WSAStartup failed.\n");
  }

  // Note that all exits from this point on must call WSACleanup() before exit.
#endif

  int socket_fd;
  int port_number;

  int result;

  port_number = atoi(argv[2]);

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    return exit_with_error("Error opening socket");
  }

  char buffer[BUFFER_SIZE];

  struct sockaddr_in server_address;

  // Test for loopback address
  char *self_addresses[] = {"localhost", "loopback"};
  char *address = argv[1];
  for (size_t i = 0; i < sizeof(self_addresses) / sizeof(char *); i++) {
    if (strncmp(argv[1], self_addresses[i], strlen(self_addresses[i])) == 0) {
      address = "127.0.0.1";
      break;
    }
  }

  server_address.sin_addr.s_addr = inet_addr(address);
  if (server_address.sin_addr.s_addr == INADDR_NONE) {
    return exit_with_error("Error: invalid IP address");
  }
  server_address.sin_port = htons(port_number);
  server_address.sin_family = AF_INET;

  if (connect(socket_fd, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    return exit_with_error("Error: connection failed");
  }

  printf("Connected to server.\nType 'exit' to quit.\n");

  while (true) {
    erase_from_memory(buffer, 0, BUFFER_SIZE);
    printf("Client: ");
    fgets(buffer, BUFFER_SIZE - 1, stdin);
    result = send(socket_fd, buffer, strlen(buffer), 0);

    if (result < 0) {
      return exit_with_error("Error: failed to send data");
    }

    if (strncmp(buffer, "exit", 4) == 0) {
      break;
    }

    erase_from_memory(buffer, 0, BUFFER_SIZE);
    result = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);

    if (result < 0) {
      return exit_with_error("Error: failed to receive data");
    }

    printf("Server: %s", buffer);
    if (strncmp(buffer, "exit", 4) == 0) {
      break;
    }
  }

  // Close the socket and cleanup
#ifdef _WIN32
  closesocket(socket_fd);

  printf("WSACleanup() called.\n");
  WSACleanup();
#else
  close(socket_fd);
#endif

  // The program terminated successfully
  return EXIT_SUCCESS;
}