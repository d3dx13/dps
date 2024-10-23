#include <Python.h>
#include "linux_proxy.h"

// https://docs.python.org/3/c-api/arg.html

static PyObject *py_borrow_fd_from_pid(PyObject *self, PyObject *args)
{
  int pid;
  int fd;
  if (PyTuple_Size(args) != 2)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &pid, &fd);
  return Py_BuildValue("i", borrow_fd_from_pid(pid, fd));
}

static PyObject *py_eventfd(PyObject *self, PyObject *args)
{
  unsigned int initval;
  int flags;
  if (PyTuple_Size(args) != 2)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "Ii", &initval, &flags);
  return Py_BuildValue("i", eventfd(initval, flags));
}

static PyObject *py_write(PyObject *self, PyObject *args)
{
  int fd;
  uint64_t value;
  if (PyTuple_Size(args) != 2)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "iK", &fd, &value);
  return Py_BuildValue("i", write(fd, &value, sizeof(value)));
}

static PyObject *py_read(PyObject *self, PyObject *args)
{
  int fd;
  uint64_t value;
  if (PyTuple_Size(args) != 2)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "iK", &fd, &value);
  int ret = read(fd, &value, sizeof(value));
  return Py_BuildValue("K", value);
}

static PyObject *py_inotify_init(PyObject *self, PyObject *args)
{
  if (PyTuple_Size(args) != 0)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  return Py_BuildValue("i", inotify_init());
}

static PyObject *py_inotify_init1(PyObject *self, PyObject *args)
{
  int flags;
  if (PyTuple_Size(args) != 1)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &flags);
  return Py_BuildValue("i", inotify_init1(flags));
}

static PyObject *py_inotify_add_watch(PyObject *self, PyObject *args)
{
  int fd;
  const char *pathname;
  uint32_t mask;
  if (PyTuple_Size(args) != 3)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "isI", &fd, &pathname, &mask);
  return Py_BuildValue("i", inotify_add_watch(fd, pathname, mask));
}

static PyObject *py_inotify_rm_watch(PyObject *self, PyObject *args)
{
  int fd;
  int wd;
  if (PyTuple_Size(args) != 3)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &fd, &wd);
  return Py_BuildValue("i", inotify_rm_watch(fd, wd));
}

#define EVENT_SIZE sizeof(struct inotify_event)
static PyObject *py_inotify_event_read(PyObject *self, PyObject *args)
{
  int fd;
  if (PyTuple_Size(args) != 1)
  {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &fd);

  int n;
  ioctl(fd, FIONREAD, &n);
  if (n == 0)
  {
    return Py_BuildValue("i", n);
  }
  printf("n %i\n", n);

  int length, i = 0, wd;
  char *buffer = new char[n];

  i = 0;
  /* высчитываем размер файлового дескриптора*/
  length = read(fd, buffer, n);
  if (length < 0)
  {
    // perror( "read" );
    delete[] buffer;
    return Py_BuildValue("i", length);
  }
  printf("length %i\n", length);
  while (i < length)
  {
    struct inotify_event *event = (struct inotify_event *)&buffer[i];
    printf("mask %d\n", event->mask);
    printf("name %s\n", event->name);
    if (event->mask == IN_DELETE_SELF)
    {
      printf("The file was IN_DELETE_SELF with WD %d\n", event->wd);
      delete[] buffer;
      return Py_BuildValue("i", -IN_DELETE_SELF);
    }
    if (event->len)
    {
      if (event->mask == IN_CLOSE_WRITE)
      {
        printf("The file %s was IN_CLOSE_WRITE with WD %d\n", event->name, event->wd);
      }
      else if (event->mask == IN_DELETE)
      {
        printf("The file %s was IN_DELETE with WD %d\n", event->name, event->wd);
      }
      i += EVENT_SIZE + event->len;
    }
    else
    {
      i += EVENT_SIZE;
      continue;
    }
  }
  // int ret = read(fd, buff, sizeof(buff));
  // return Py_BuildValue("y", buff);
  delete[] buffer;
  return Py_BuildValue("i", length);
}

static PyMethodDef Methods[] = {
    {"borrow_fd_from_pid", py_borrow_fd_from_pid, METH_VARARGS, "borrow_fd_from_pid"},
    {"eventfd", py_eventfd, METH_VARARGS, "eventfd"},
    {"write", py_write, METH_VARARGS, "write"},
    {"read", py_read, METH_VARARGS, "read"},
    {"inotify_init", py_inotify_init, METH_VARARGS, "inotify_init"},
    {"inotify_init1", py_inotify_init1, METH_VARARGS, "inotify_init1"},
    {"inotify_add_watch", py_inotify_add_watch, METH_VARARGS, "inotify_add_watch"},
    {"inotify_rm_watch", py_inotify_rm_watch, METH_VARARGS, "inotify_rm_watch"},
    {"inotify_event_read", py_inotify_event_read, METH_VARARGS, "inotify_event_read"},
    {nullptr, nullptr, 0, nullptr}};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "linux_proxy",
    "-",
    -1,
    Methods};

PyMODINIT_FUNC PyInit_linux_proxy()
{
  return PyModule_Create(&module);
}