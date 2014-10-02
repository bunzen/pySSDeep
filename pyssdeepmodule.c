
/*
 * Copyright (c) 2014, Geir Skjotskift
 * All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/ 

#include <stdio.h>
#include <fuzzy.h>
#include <Python.h>

static PyObject *
ssdeep_fuzzy_hash_buf(PyObject *self, PyObject *args) {

  char *result; /* ssdeep hash */
  unsigned char *buff; /* function argument */
  int buffsize; /* length of function argument (may contain \0) */
  PyObject *return_value; /* create a python object before return to allow for free */

  /* parse out the function argument containting the buffer that should be
   * hashed by ssdeep.
   */
  if (!PyArg_ParseTuple(args, "s#", &buff, &buffsize)) {
    PyErr_SetObject(PyExc_ValueError, Py_BuildValue("s", "Unable to parse string buffer from argument list"));
    return NULL;
  }

  if ((result = malloc(FUZZY_MAX_RESULT)) == NULL) {
    PyErr_NoMemory();
    return NULL;
  }

  /* compute the ssdeep hash from buffer */
  if (fuzzy_hash_buf(buff, buffsize, result) != 0) {
    PyErr_SetObject(PyExc_ValueError, Py_BuildValue("s", "Unable to compute hash"));
    free(result);
    return NULL;
  }

  /* build a python string and free up result buffer. */
  return_value =  Py_BuildValue("s", result);
  free(result);

  return return_value;
}


static PyObject *
ssdeep_fuzzy_hash_filename(PyObject *self, PyObject *args) {

  char *result; /* ssdeep hash*/
  char *filename; /* function argument */
  PyObject *return_value; /* create a python object before return to allow for free */

  /* parse out the function argument containting the buffer that should be                                           
   * hashed by ssdeep.                                                                                               
   */
  if (!PyArg_ParseTuple(args, "s", &filename)) {
    PyErr_SetObject(PyExc_ValueError, Py_BuildValue("s", "Unable to parse filename from argument list"));
    return NULL;
  }

  if ((result = malloc(FUZZY_MAX_RESULT)) == NULL) {
    PyErr_NoMemory();
    return NULL;
  }

  /* compute the ssdeep hash from buffer */
  if (fuzzy_hash_filename(filename, result) != 0) {
    PyErr_SetObject(PyExc_ValueError, Py_BuildValue("s", "Unable to compute hash"));
    free(result);
    return NULL;
  }

  /* build a python string and free up result buffer. */
  return_value =  Py_BuildValue("s", result);
  free(result);

  return return_value;

}

static PyObject *
ssdeep_fuzzy_compare(PyObject *self, PyObject *args) {
  int distance; /* difference between hash signature one and two (return value)*/
  char *sig1, *sig2; /* local storage for the two hash signatures */
  
  if (!PyArg_ParseTuple(args, "ss", &sig1, &sig2)) {
    PyErr_SetObject(PyExc_ValueError, Py_BuildValue("s", "Unable to parse two string signatures from argument list"));
    return NULL;
  }

  distance = fuzzy_compare(sig1, sig2);
  return Py_BuildValue("i", distance);
}

static PyMethodDef FuzzyMethods[] = {
  {"fuzzy_hash_buf", ssdeep_fuzzy_hash_buf, METH_VARARGS, "Hash a string buffer"},
  {"fuzzy_hash_filename", ssdeep_fuzzy_hash_filename, METH_VARARGS, "Hash a file given the filename"},
  {"fuzzy_compare", ssdeep_fuzzy_compare, METH_VARARGS, "Compare two hash signatures"},
  {NULL, NULL, 0, NULL} /* sentinel */
};


PyMODINIT_FUNC
initpyssdeep(void) {
  (void) Py_InitModule("pyssdeep", FuzzyMethods);
}

