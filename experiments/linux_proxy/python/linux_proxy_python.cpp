#include <Python.h>
#include "linux_proxy.h"

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
  int ret = read(fd, &value, sizeof(value));
  return Py_BuildValue("K", value);
}

static PyObject *py_inotify_init(PyObject *self, PyObject *args) {
  if (PyTuple_Size(args) != 0) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  return Py_BuildValue("i", inotify_init());
}

static PyObject *py_inotify_init1(PyObject *self, PyObject *args) {
  int flags;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &flags);
  return Py_BuildValue("i", inotify_init1(flags));
}

static PyObject *py_inotify_add_watch(PyObject *self, PyObject *args) {
  int fd;
  const char *pathname;
  uint32_t mask;
  if (PyTuple_Size(args) != 3) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "isI", &fd, &pathname, &mask);
  return Py_BuildValue("i", inotify_add_watch(fd, pathname, mask));
}

static PyObject *py_inotify_rm_watch(PyObject *self, PyObject *args) {
  int fd;
  int wd;
  if (PyTuple_Size(args) != 3) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &fd, &wd);
  return Py_BuildValue("i", inotify_rm_watch(fd, wd));
}

#define EVENT_SIZE  sizeof(struct inotify_event)
#define BUF_LEN 100000
static PyObject *py_inotify_event_read(PyObject *self, PyObject *args) {
  int fd;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &fd);

  int length, i = 0, wd;
  char buffer[BUF_LEN];

  i = 0;
  /* высчитываем размер файлового дескриптора*/
  length = read( fd, buffer, BUF_LEN );
  printf( "length %i\n", length );
  if ( length < 0 ) {
      perror( "read" );
  }
  while ( i < length ) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    printf( "i %i \n" );
    if ( event->len ) {
        printf( "mask %i \n", event->mask );
        printf( "name %s \n", event->name );
        printf( "name %d \n", event->wd );
        if ( event->mask & IN_OPEN) {
            if (event->mask & IN_ISDIR)
                printf( "The directory %s was opened.\n", event->name );
            else
                printf( "The file %s was opened with WD %d\n", event->name, event->wd );
        } else if ( event->mask & IN_CREATE) {
            if (event->mask & IN_ISDIR)
                printf( "The directory %s was created.\n", event->name );
            else
                printf( "The file %s was created with WD %d\n", event->name, event->wd );
        } else if ( event->mask & IN_DELETE) {
            if (event->mask & IN_ISDIR)
                printf( "The directory %s was deleted.\n", event->name );
            else
                printf( "The file %s was deleted with WD %d\n", event->name, event->wd );
        } else if ( event->mask & IN_CLOSE) {
            if (event->mask & IN_ISDIR)
                printf( "The directory %s was closed.\n", event->name );
            else
                printf( "The file %s was closed with WD %d\n", event->name, event->wd );
        } else if ( event->mask & IN_MODIFY) {
            if (event->mask & IN_ISDIR)
                printf( "The directory %s was modified.\n", event->name );
            else
                printf( "The file %s was modified with WD %d\n", event->name, event->wd );
        } else {
          printf( "UNKNOWN\n" );
        }
        i += EVENT_SIZE + event->len;
    } else {
      i += EVENT_SIZE;
      continue;
    }
  }
  // int ret = read(fd, buff, sizeof(buff));
  // return Py_BuildValue("y", buff);
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