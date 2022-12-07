#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdint.h>
#include <Python.h>

//int process_read(pid_t pid, uint64_t address, uint8_t *buffer, size_t size) {
static PyObject *process_read(PyObject *self, PyObject *args) {
    struct iovec remote[1];
    struct iovec local[1];

    int32_t pid;
    uint64_t address;
    size_t size;
    uint8_t *buffer;

    if(!PyArg_ParseTuple(args, "iky*k", &pid, &address, &buffer, &size)){
        return NULL;
    }

    remote[0].iov_base = (void *)address;
    remote[0].iov_len = size;

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    
    return PyLong_FromLong(process_vm_readv(pid, local, 1, remote, 1, 0));
}

//int process_write(pid_t pid, uint64_t address, uint8_t *data, size_t size) {
static PyObject *process_write(PyObject *self, PyObject *args) {
    struct iovec remote[1];
    struct iovec local[1];

    int32_t pid;
    uint64_t address;
    size_t size;
    uint8_t *buffer;

    if(!PyArg_ParseTuple(args, "iky*k", &pid, &address, &buffer, &size)){
        return NULL;
    }

    local[0].iov_base = buffer;
    local[0].iov_len = size;

    remote[0].iov_base = (void *) address;
    remote[0].iov_len = size;


    return PyLong_FromLong(process_vm_writev(pid, local, 1, remote, 1, 0));
}

static PyMethodDef PyuioMethods[] = {
    {"_process_read", process_read, METH_VARARGS, "Python interface for the process_vm_readv function"},
    {"_process_write", process_write, METH_VARARGS, "Python interface for the process_vm_writev function"},
    {NULL,NULL,0,NULL}
};

static struct PyModuleDef pyuiomodule = {
    PyModuleDef_HEAD_INIT,
    "pyuiolib",
    "Python Linux Userspace IO interface library",
    -1,
    PyuioMethods
};

PyMODINIT_FUNC PyInit_pyuio(void) {
    return PyModule_Create(&pyuiomodule);
}