// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_

#include <algorithm>
#include <memory>

typedef struct Message_s {
  int command;
  int id_proc;
} Message;

class Client {
  int id_proc;
  int id_line;
  int id_barber;

 public:
  int place_in_line;
  explicit Client(int id_proc = -1, int id_line = 1, int id_barber = 0)
      : id_proc(id_proc), id_line(id_line), id_barber(id_barber) {
    place_in_line = 0;
  }
  int onHold();
  void initiate();
  void cut();
  int getId_Proc() { return id_proc; }
  // ~Client() {
  //   if (this != nullptr) {
  //     id_proc = 0;
  //     id_line = 0;
  //     id_barber = 0;
  //     place_in_line = 0;
  //   }
  // }
};
class Line {
  Client* ClientLine;
  //  number of clients in general
  int length;
  //  current clients at the barbershop
  int clients_num;
  //  max in line
  int max_clients;
  int id_proc;
  int num_proc_exec;
  bool barber_sleeping;
  int id_barber;

 public:
  explicit Line(int id_proc = 1, int length = 100, int id_barber = 0,
                int num_proc_exec = 2) {
    if (length > 0) {
      this->id_proc = id_proc;
      this->length = length;
      this->id_barber = id_barber;
      this->num_proc_exec = num_proc_exec - 2;
      ClientLine = new Client[length];
      clients_num = 0;
      max_clients = length / 2;
      barber_sleeping = false;
    } else {
      throw "length must be > 0";
    }
  }
  int check_line();
  void initiate();
  // Copy constructor
  Line(const Line& l1) {
    this->id_proc = l1.id_proc;
    this->length = l1.length;
    this->id_barber = l1.id_barber;
    this->num_proc_exec = l1.num_proc_exec;
    ClientLine = new Client[length];
    clients_num = 0;
    max_clients = length / 2;
    barber_sleeping = false;
  }
  Line& operator=(const Line& other) {
    // Guard self assignment
    if (this == &other) return *this;
    // assume *this manages a reusable resource, such as a heap-allocated buffer
    // mArray
    if (this->length != other.length) {  // resource in *this cannot be reused
      delete[] ClientLine;               // release resource in *this
      this->ClientLine = nullptr;
      this->length = 0;  // preserve invariants in case next line throws
      this->ClientLine =
          new Client[other.length];  // allocate resource in *this
      this->length = other.length;
    }
    std::copy(other.ClientLine, other.ClientLine + other.length, ClientLine);

    this->id_proc = other.id_proc;
    this->id_barber = other.id_barber;
    this->num_proc_exec = other.num_proc_exec;
    clients_num = 0;
    max_clients = length / 2;
    barber_sleeping = false;

    return *this;
  }
  ~Line() = default;
};
class Barber {
  int id_proc;
  int id_line;

 public:
  explicit Barber(int id_proc = 0, int id_line = 1)
      : id_proc(id_proc), id_line(id_line) {}
  void initiate();
  bool wakeup();
};
void execute(int length);

#endif  // MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
