// Copyright 2018 Nesterov Alexander
#include "../../../modules/task_1/vorobyova_a_sleeping_barber/sleeping_barber.h"

#include <mpi.h>
#if defined(WIN32)
#include <windows.h>
#elif defined(__unix__)
#include <time.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <unistd.h>
#else
#endif
#include <iostream>

int millisleep(unsigned ms) {
#if defined(WIN32)
  SetLastError(0);
  Sleep(ms);
  return GetLastError() ? -1 : 0;
#elif _POSIX_C_SOURCE >= 199309L
  const struct timespec ts = {
      ms / 1000,                /* seconds */
      (ms % 1000) * 1000 * 1000 /* nano seconds */
  };
  return nanosleep(&ts, NULL);
#elif _BSD_SOURCE ||                                                     \
    (_XOPEN_SOURCE >= 500 || _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED) && \
        !(_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
  return usleep(1000 * ms);
#elif __APPLE__
  return usleep(1000 * ms);
#else
#error("No millisecond sleep available for this platform!")
  return -1;
#endif
}

void Client::initiate() {
  MPI_Status status;
  Message message;
  message.command = 1;
  message.id_proc = id_proc;
  printf("[C]-[%d] \n", id_proc);
  fflush(stdout);
  while (-2 != message.command) {
    message.command = 1;
    MPI_Send(&message, sizeof(Message), MPI_CHAR, id_line, 123, MPI_COMM_WORLD);
    MPI_Recv(&message, sizeof(Message), MPI_CHAR, id_line, 123, MPI_COMM_WORLD,
             &status);
    switch (message.command) {
      case -2: {
        break;
      }
      case -1: {
        millisleep(500);
        break;
      }
      default: {  // 1.information is sent to the line
        // 2.changes are made dictated by the barber
        MPI_Send(this, sizeof(Client), MPI_CHAR, id_line, 123, MPI_COMM_WORLD);
        MPI_Recv(this, sizeof(Client), MPI_CHAR, id_barber, 10, MPI_COMM_WORLD,
                 &status);
        printf("[C]-[%d] got their new haircut\n", id_proc);
        //
        MPI_Send(&message, sizeof(Message), MPI_CHAR, id_barber, 10,
                 MPI_COMM_WORLD);
      }
    }
  }
  printf("[C]-[%d] went home\n", id_proc);
  fflush(stdout);
}
void Client::cut() {}
int Line::check_line() {
  if (0 == length) {
    return -2;  // end of work day
  }
  if (0 == clients_num) {
    return 1;  // check the barber's actions
  }
  if (clients_num < max_clients) {
    return 0;  //  stand in the line
  }
  if (clients_num >= max_clients) {
    return -1;  //  nothing to do
  }
  return -1;
}
void Line::initiate() {
  // 1.communication with the barber
  // 2.main loop
  printf(
      "[L] started with num_proc_exec = %d and max_clientes in the line = "
      "%d\n",
      num_proc_exec, max_clients);
  fflush(stdout);
  MPI_Status status;
  Message message;
  //
  MPI_Recv(&message.command, 1, MPI_INT, id_barber, 1, MPI_COMM_WORLD, &status);
  MPI_Send(&message.command, 1, MPI_INT, id_barber, 1, MPI_COMM_WORLD);
  //
  while ((num_proc_exec > 0) || (!barber_sleeping)) {
    MPI_Recv(&message, sizeof(Message), MPI_CHAR, MPI_ANY_SOURCE, 123,
             MPI_COMM_WORLD, &status);
    printf("[L] got a request from %d proc with the command = %d\n",
           message.id_proc, message.command);
    fflush(stdout);
    // commands generated to customers.
    switch (message.command) {
      case 1: {
        message.command = check_line();
        if (-2 == message.command) {
          num_proc_exec--;
        }
        MPI_Send(&message, sizeof(Message), MPI_CHAR, message.id_proc, 123,
                 MPI_COMM_WORLD);
        if (message.command >= 0) {
          length--;
          clients_num++;
          MPI_Recv(&ClientLine[length], sizeof(Client), MPI_CHAR,
                   message.id_proc, 123, MPI_COMM_WORLD, &status);
          ClientLine[length].place_in_line = length;
          printf("[L] with the proc id = %d in the %d place\n",
                 ClientLine[length].getId_Proc(), length);
          fflush(stdout);
          if ((1 == message.command) && (barber_sleeping)) {
            printf("[L] is going to wake up the barber\n");
            fflush(stdout);
            MPI_Send(&message.command, 1, MPI_INT, id_barber, 100,
                     MPI_COMM_WORLD);
            MPI_Recv(&message.command, 1, MPI_INT, id_barber, 100,
                     MPI_COMM_WORLD, &status);
            barber_sleeping = false;
          }
        }
        break;
      }
      case 2: {  // the barber's order is empty (query)
        message.command = clients_num;
        MPI_Send(&message, sizeof(Message), MPI_CHAR, id_barber, 1,
                 MPI_COMM_WORLD);
        if (message.command > 0) {
          // get the barber's response
          MPI_Send(&ClientLine[length + clients_num - 1], sizeof(Client),
                   MPI_CHAR, id_barber, 2, MPI_COMM_WORLD);
          //
          MPI_Recv(&message, sizeof(Message), MPI_CHAR, id_barber, 1,
                   MPI_COMM_WORLD, &status);
          clients_num--;
        } else {
          barber_sleeping = true;
        }
        break;
      }
    }
  }
  if (barber_sleeping) {
    printf("[L] is going to wake up the barber\n");
    fflush(stdout);
    message.command = -2;
    MPI_Send(&message.command, 1, MPI_INT, id_barber, 100, MPI_COMM_WORLD);
    MPI_Recv(&message.command, 1, MPI_INT, id_barber, 100, MPI_COMM_WORLD,
             &status);
    printf("[L] wakes up the barber (%d)\n", message.command);
    fflush(stdout);
  }
  free(ClientLine);
  printf("[L] finished its work\n");
  fflush(stdout);
}
void Barber::initiate() {
  Client CurrentClient;
  MPI_Status status;
  Message message;
  message.command = 2;
  message.id_proc = id_proc;
  // connect with the line
  MPI_Send(&message.command, 1, MPI_INT, id_line, 1, MPI_COMM_WORLD);
  MPI_Recv(&message.command, 1, MPI_INT, id_line, 1, MPI_COMM_WORLD, &status);
  while (message.command != -2) {
    message.command = 2;
    MPI_Send(&message, sizeof(Message), MPI_CHAR, id_line, 123, MPI_COMM_WORLD);
    MPI_Recv(&message, sizeof(Message), MPI_CHAR, id_line, 1, MPI_COMM_WORLD,
             &status);
    if (message.command > 0) {
      MPI_Recv(&CurrentClient, sizeof(Client), MPI_CHAR, id_line, 2,
               MPI_COMM_WORLD, &status);
      // for synchronization, a response is sent to the line
      MPI_Send(&message, sizeof(Message), MPI_CHAR, id_line, 1, MPI_COMM_WORLD);
      printf("[B] starts [%d] with the place = *%d*\n",
             CurrentClient.getId_Proc(), CurrentClient.place_in_line);
      fflush(stdout);
      CurrentClient.cut();
      MPI_Send(&CurrentClient, sizeof(Client), MPI_CHAR,
               CurrentClient.getId_Proc(), 10, MPI_COMM_WORLD);
      // for synchronization, the response is received from the client
      MPI_Recv(&message, sizeof(Message), MPI_CHAR, CurrentClient.getId_Proc(),
               10, MPI_COMM_WORLD, &status);
      message.id_proc = id_proc;
    } else if (message.command != -2) {
      printf("[B] went to sleep\n");
      fflush(stdout);
      MPI_Recv(&message.command, 1, MPI_INT, id_line, 100, MPI_COMM_WORLD,
               &status);
      printf("[B] woke up (%d)\n", message.command);
      fflush(stdout);
      MPI_Send(&message.command, 1, MPI_INT, id_line, 100, MPI_COMM_WORLD);
    }
  }
  printf("[B] finished his work\n");
  fflush(stdout);
}
void execute(int length) {
  int num_proc, id_proc;
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
  if (num_proc <= 2) {
    throw "Not enough processes";
  } else if (id_proc == 0) {
    Barber barber(0, 1);
    barber.initiate();
  } else if (id_proc == 1) {
    Line line(1, length, 0, num_proc);
    line.initiate();
  } else {
    Client client(id_proc, 1, 0);
    client.initiate();
  }
}
