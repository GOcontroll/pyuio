#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdint.h>
#include <Python.h>
#include <stdio.h>

//int process_read(pid_t pid, uint64_t address, uint8_t *buffer, size_t size) {
static PyObject *process_read(PyObject *self, PyObject *args) {

    struct iovec remote[1];
    struct iovec local[1];
   
    int32_t pid;
    uint64_t address;
    size_t size;
    uint64_t bufferAddress;
    uint8_t *buffer;

    if(!PyArg_ParseTuple(args, "ikkk", &pid, &address, &bufferAddress, &size)){
        return NULL;
    }
    //not terribly happy with how I'm passing this buffer if someone is reading this and knows a better way, please let me know.
    buffer = (uint8_t *) bufferAddress;

    remote[0].iov_base = (void *)address;
    remote[0].iov_len = size;

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    
    return Py_BuildValue("l", process_vm_readv(pid, local, 1, remote, 1, 0));
}

//int process_write(pid_t pid, uint64_t address, uint8_t *data, size_t size) {
static PyObject *process_write(PyObject *self, PyObject *args) {
    struct iovec remote[1];
    struct iovec local[1];

    int32_t pid;
    uint64_t address;
    size_t size;
    uint64_t bufferAddress;
    uint8_t *buffer;

    if(!PyArg_ParseTuple(args, "ikkk", &pid, &address, &bufferAddress, &size)){
        return NULL;
    }
    //not terribly happy with how I'm passing this buffer if someone is reading this and knows a better way, please let me know.
    buffer = (uint8_t *) bufferAddress;

    local[0].iov_base = buffer;
    local[0].iov_len = size;

    remote[0].iov_base = (void *) address;
    remote[0].iov_len = size;

    return Py_BuildValue("l", process_vm_writev(pid, local, 1, remote, 1, 0));
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

PyMODINIT_FUNC PyInit_pyuiolib(void) {
    return PyModule_Create(&pyuiomodule);
}