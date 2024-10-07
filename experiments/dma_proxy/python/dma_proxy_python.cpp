#include <Python.h>
#include "dma_proxy.h"

#include<iostream>

// https://docs.python.org/3/c-api/arg.html

/*
borrow_fd_from_pid

Pizdim fd from pid
*/ 
static PyObject *py_borrow_fd_from_pid(PyObject *self, PyObject *args) {
  int pid;
  int fd;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &pid, &fd);
  return Py_BuildValue("i", borrow_fd_from_pid(pid, fd));
}

static PyObject *py_pidfd_open(PyObject *self, PyObject *args) {
  int fd;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &fd);
  return Py_BuildValue("i", pidfd_open(fd));
}

static PyObject *py_pidfd_getfd(PyObject *self, PyObject *args) {
  int pid;
  int fd;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &pid, &fd);
  return Py_BuildValue("i", pidfd_getfd(pid, fd));
}

/*
tmpfile
*/
static PyObject *py_tmpfile(PyObject *self, PyObject *args) {
  char *path;
  char *name;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ss", &path, &name);
  printf("Vremenno %s slave %s\n", name, path);
  #define P_tmpdir "/dev/shm"
  FILE* tmpf = tmpfile();
  char writeToFile[50] = "Creating a temporary file adawdadadadwadawdwa\n";
  fputs(writeToFile, tmpf);
	rewind(tmpf);
  char readFromFile[50];
	fgets(readFromFile, sizeof(readFromFile), tmpf);
  std::cout << readFromFile << "\n";
  std::cout << fileno(tmpf) << "\n";
	// fclose(tmpf);
  return Py_BuildValue("i", fileno(tmpf));
}

/*
dma_buf

int dmabuf_heap_open();
void dmabuf_heap_close(int heap_fd);
int dmabuf_heap_alloc(int heap_fd, const char *name, size_t size);
int dmabuf_sync_start(int buf_fd);
int dmabuf_sync_stop(int buf_fd);
*/
static PyObject *py_dmabuf_heap_open(PyObject *self, PyObject *args) {
  if (PyTuple_Size(args) != 0) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  return Py_BuildValue("i", dmabuf_heap_open());
}

static PyObject *py_dmabuf_heap_close(PyObject *self, PyObject *args) {
  int heap_fd;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &heap_fd);
  dmabuf_heap_close(heap_fd);
  return Py_BuildValue("");
}

static PyObject *py_dmabuf_heap_alloc(PyObject *self, PyObject *args) {
  int heap_fd;
  const char *name;
  size_t size;
  if (PyTuple_Size(args) != 3) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "isn", &heap_fd, &name, &size);
  return Py_BuildValue("i", dmabuf_heap_alloc(heap_fd, name, size));
}

/*
static PyObject *py_dmabuf_export_sync_file(PyObject *self, PyObject *args) {
  int buf_fd;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &buf_fd);
  dma_buf_export_sync_file sync_file = {0, 0};
  sync_file.flags = DMA_BUF_SYNC_READ;
  ioctl(buf_fd, DMA_BUF_IOCTL_EXPORT_SYNC_FILE, &sync_file);
  return Py_BuildValue("i", sync_file.fd);
}

static PyObject *py_dmabuf_import_sync_file(PyObject *self, PyObject *args) {
  int buf_fd;
  int fd;
  if (PyTuple_Size(args) != 2) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "ii", &buf_fd, &fd);
  dma_buf_import_sync_file sync_file = {0, 0};
  sync_file.flags = DMA_BUF_SYNC_READ;
  sync_file.fd = fd;
  ioctl(buf_fd, DMA_BUF_IOCTL_IMPORT_SYNC_FILE, &sync_file);
  return Py_BuildValue("i", sync_file.fd);
}
*/

static PyObject *py_dmabuf_sync_start(PyObject *self, PyObject *args) {
  int buf_fd;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &buf_fd);
  return Py_BuildValue("i", dmabuf_sync_start(buf_fd));
}

static PyObject *py_dmabuf_sync_stop(PyObject *self, PyObject *args) {
  int buf_fd;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &buf_fd);
  return Py_BuildValue("i", dmabuf_sync_stop(buf_fd));
}

static PyObject *py_close(PyObject *self, PyObject *args) {
  int fd;
  if (PyTuple_Size(args) != 1) {
    PyErr_SetString(self, "func_ret_str args error");
  }
  PyArg_ParseTuple(args, "i", &fd);
  return Py_BuildValue("i", close(fd));
}

static PyMethodDef Methods[] = {
  {"borrow_fd_from_pid", py_borrow_fd_from_pid, METH_VARARGS, "borrow_fd_from_pid"},
  {"tmpfile", py_tmpfile, METH_VARARGS, "tmpfile"},
  {"pidfd_open", py_pidfd_open, METH_VARARGS, "pidfd_open"},
  {"pidfd_getfd", py_pidfd_getfd, METH_VARARGS, "pidfd_getfd"},
  {"dmabuf_heap_open", py_dmabuf_heap_open, METH_VARARGS, "dmabuf_heap_open"},
  {"dmabuf_heap_close", py_dmabuf_heap_close, METH_VARARGS, "dmabuf_heap_close"},
  {"dmabuf_heap_alloc", py_dmabuf_heap_alloc, METH_VARARGS, "dmabuf_heap_alloc"},
  {"dmabuf_sync_start", py_dmabuf_sync_start, METH_VARARGS, "dmabuf_sync_start"},
  {"dmabuf_sync_stop", py_dmabuf_sync_stop, METH_VARARGS, "dmabuf_sync_stop"},
  {"close", py_close, METH_VARARGS, "close"},
  // {"dmabuf_export_sync_file", py_dmabuf_export_sync_file, METH_VARARGS, "dmabuf_export_sync_file"},
  // {"dmabuf_import_sync_file", py_dmabuf_import_sync_file, METH_VARARGS, "dmabuf_import_sync_file"},
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