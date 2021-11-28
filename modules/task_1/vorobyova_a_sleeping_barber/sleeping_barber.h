// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_

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
      : id_proc(id_proc), id_line(id_line), id_barber(id_barber) {}
  int onHold();
  void initiate();
  void cut();
  int getId_Proc() { return id_proc; }
};
class Line {
  Client *ClientLine;
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
