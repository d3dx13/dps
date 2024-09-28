#include <Python.h>
#include "linux_proxy.h"

static PyObject *py_borrow_fd_from_pid(PyObject *self, PyObject *args) {
  int pid;
  int fd;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &pid, &fd);
  return Py_BuildValue("i", borrow_fd_from_pid(pid, fd));
}

static PyObject *py_eventfd(PyObject *self, PyObject *args) {
  unsigned int initval;
  int flags;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "Ii", &initval, &flags);
  return Py_BuildValue("i", eventfd(initval, flags));
}

static PyObject *py_write(PyObject *self, PyObject *args) {
  int fd;
  uint64_t value;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "iK", &fd, &value);
  return Py_BuildValue("i", write(fd, &value, sizeof(value)));
}

static PyObject *py_read(PyObject *self, PyObject *args) {
  int fd;
  uint64_t value;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "iK", &fd, &value);
  return Py_BuildValue("i", read(fd, &value, sizeof(value)));
}

static PyObject *py_memfd_create(PyObject *self, PyObject *args) {
  const char *name;
  ssize_t flags;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "sK", &name, &flags);
  return Py_BuildValue("i", memfd_create(name, flags));
}

static PyObject *py_ftruncate(PyObject *self, PyObject *args) {
  int fd;
  uint64_t len;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "iK", &fd, &len);
  return Py_BuildValue("i", ftruncate(fd, len));
}

static PyMethodDef Methods[] = {
  {"borrow_fd_from_pid", py_borrow_fd_from_pid, METH_VARARGS, "borrow_fd_from_pid"},
  {"eventfd", py_eventfd, METH_VARARGS, "eventfd"},
  {"write", py_write, METH_VARARGS, "write"},
  {"read", py_read, METH_VARARGS, "read"},
  {"memfd_create", py_memfd_create, METH_VARARGS, "memfd_create"},
  {"ftruncate", py_ftruncate, METH_VARARGS, "ftruncate"},
  {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef module = {
  PyModuleDef_HEAD_INIT,
  "linux_proxy",
  "-",
  -1,
  Methods
};

PyMODINIT_FUNC PyInit_linux_proxy() {
  return PyModule_Create(&module);
}