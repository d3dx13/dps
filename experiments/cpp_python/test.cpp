#include <Python.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>

static PyObject *get_answer_c(PyObject *self, PyObject *args) {
  int pid = 3281;
  int fd = 4;
  std::cout << "WOW" << "\n";

  int pid_fd = syscall(SYS_pidfd_open, pid, 0);
  int dst_fd = syscall(SYS_pidfd_getfd, pid_fd, fd, 0);
  std::cout << dst_fd << "\n";

  return PyLong_FromLong(dst_fd);
}

static PyMethodDef TestMethods[] = {
  {"get_answer", get_answer_c, METH_VARARGS,
   "Get the answer to life, Universe and everything."},
  {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef testmodule = {
  PyModuleDef_HEAD_INIT,
  "answer",
  "Answer module.",
  -1,
  TestMethods
};

PyMODINIT_FUNC PyInit_answer() {
  return PyModule_Create(&testmodule);
}