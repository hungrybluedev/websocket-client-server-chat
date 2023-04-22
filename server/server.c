#include "../common/all_imports.h"
#include "../common/definitions.h"
#include "../common/erase_from_memory.h"
#include "../common/exit_with_error.h"

#define USAGE "Usage: %s <port_number>\n"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, USAGE, argv[0]);
    fprintf(stderr, "Please provide a port number.\n");
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
  int new_socket_fd;
  int port_number;

  int result;

  char buffer[BUFFER_SIZE];

  struct sockaddr_in server_address, client_address;

  socklen_t client_address_length;

  // 0 means that the OS will choose the protocol which should be TCP by default
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Ensure that the socket was created successfully
  if (socket_fd < 0) {
    return exit_with_error("Error opening socket");
  }

  // Clear the server_address struct
  erase_from_memory(&server_address, 0, sizeof(server_address));

  // Obtain the port number from the command line arguments
  port_number = atoi(argv[1]);

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(port_number);

  if (bind(socket_fd, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    return exit_with_error("Error on binding");
  }

  // Listen for connections on the socket
  listen(socket_fd, MAX_CONNECTIONS);

  client_address_length = sizeof(client_address);

  // Accept a connection on the socket
  new_socket_fd = accept(socket_fd, (struct sockaddr *)&client_address,
                         &client_address_length);

  if (new_socket_fd < 0) {
    return exit_with_error("Error on accept");
  }

  printf("Server started.\nWaiting for client to send message...\n");

  while (TRUE) {
    // Clear the buffer
    erase_from_memory(buffer, 0, BUFFER_SIZE);

    // Read the message from the client
    result = recv(new_socket_fd, buffer, BUFFER_SIZE - 1, 0);

    if (result < 0) {
      return exit_with_error("Error reading from socket");
    }

    // Print the message to the console
    printf("Client: %s\n", buffer);

    // Check if the client sent the "exit" command
    if (strncmp(buffer, "exit", 4) == 0) {
      break;
    }

    // Clear the buffer
    erase_from_memory(buffer, 0, BUFFER_SIZE);
    printf("Server: ");
    fgets(buffer, BUFFER_SIZE - 1, stdin);

    // Send the message to the client
    result = send(new_socket_fd, buffer, strlen(buffer), 0);

    if (result < 0) {
      return exit_with_error("Error writing to socket");
    }

    // Check if the user sent the "exit" command
    if (strncmp(buffer, "exit", 4) == 0) {
      break;
    }
  }

  // Close the socket and cleanup
#ifdef _WIN32
  closesocket(new_socket_fd);
  closesocket(socket_fd);

  printf("WSACleanup() called.\n");
  WSACleanup();
#else
  close(new_socket_fd);
  close(socket_fd);
#endif

  // The program terminated successfully
  return EXIT_SUCCESS;
}