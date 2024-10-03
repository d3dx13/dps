#include <Python.h>
#include "dma_proxy.h"

// https://docs.python.org/3/c-api/arg.html

static PyObject *py_borrow_fd_from_pid(PyObject *self, PyObject *args) {
  int pid;
  int fd;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &pid, &fd);
  return Py_BuildValue("i", borrow_fd_from_pid(pid, fd));
}

static PyMethodDef Methods[] = {
  {"borrow_fd_from_pid", py_borrow_fd_from_pid, METH_VARARGS, "borrow_fd_from_pid"},
  {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef module = {
  PyModuleDef_HEAD_INIT,
  "dma_proxy",
  "-",
  -1,
  Methods
};

PyMODINIT_FUNC PyInit_dma_proxy() {
  return PyModule_Create(&module);
}