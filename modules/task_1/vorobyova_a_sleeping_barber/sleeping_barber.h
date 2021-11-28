// Copyright 2021 Vorobyova Anna
#ifndef MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_

typedef struct Message_s {
  int command;
  int id_proc;
} Message;

struct Client {
  int id_proc;
  int id_line;
  int id_barber;
  int place_in_line;
  explicit Client(int id_proc = -1, int id_line = 1, int id_barber = 0)
      : id_proc(id_proc), id_line(id_line), id_barber(id_barber) {
    place_in_line = 0;
  }
  void initiate();
  void cut();
  int getId_Proc() { return id_proc; }
};

void Line(int id_proc = 1, int length = 100, int id_barber = 0,
          int num_proc_exec = 2);
void Barber(int id_proc = 0, int id_line = 1);
void execute(int length);

#endif  // MODULES_TASK_1_VOROBYOVA_A_SLEEPING_BARBER_SLEEPING_BARBER_H_
