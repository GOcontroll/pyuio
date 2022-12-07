#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdint.h>
#include <Python.h>
#include <stdio.h>

//int process_read(pid_t pid, uint64_t address, uint8_t *buffer, size_t size) {
static PyObject *process_read(PyObject *self, PyObject *args) {
    printf("process_read called\n");
    struct iovec remote[1];
    struct iovec local[1];
   
    int32_t pid;
    uint64_t address;
    size_t size;
    uint8_t *buffer;

    printf("parsing python arguments\n");

    if(!PyArg_ParseTuple(args, "ikw*k", &pid, &address, &buffer, &size)){
        return NULL;
    }

    printf("arguments parsed, pid: %d, address: %d, size: %d\n",pid,address,size);

    remote[0].iov_base = (void *)address;
    remote[0].iov_len = size;

    local[0].iov_base = buffer;
    local[0].iov_len = size;

    printf("calling process_vm_readv\n");

    ssize_t ret = process_vm_readv(pid, local, 1, remote, 1, 0);

    printf("returning %d",ret);
    
    return Py_BuildValue("l", ret);
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