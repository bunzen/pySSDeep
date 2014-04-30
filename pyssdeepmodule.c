
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

#include <fuzzy.h>
#include <Python.h>

static Pyobject *
ssdeep_fuzzy_hash_buf(PyObject *self, PyObject *args) {

  char *result; /* ssdeep hash */
  char *buff; /* function argument */
  int buffsize; /* length of function argument (may contain \0) */
  PyObject *return_value; /* create a python object before return to allow for free */

  /* parse out the function argument containting the buffer that should be
   * hashed by ssdeep.
   */
  if (!PyArg_ParseTuple(args, "s#", &buff, &buffsize)) {
    PyErr_SetObject(PyExc_ValueError, "Unable to parse string buffer from argument list");
    return NULL;
  }

  if ((result = malloc(FUZZY_MAX_RESULT)) == NULL) {
    PyErr_NoMemory();
    return NULL;
  }

  /* compute the ssdeep hash from buffer */
  if (fuzzy_hash_buf(buff, buffsize, result) != 0) {
    PyErr_SetObject(PyExc_ValueError, ("Unable to compute hash"));
    free(result);
    return NULL;
  }

  /* build a python string and free up result buffer. */
  return_value =  Py_BuildValue("s", result);
  free(result);

  return return_value;
}


static PyMethodDef FuzzyMethods[] = {
  {"fuzzy_hash_buf", ssdeep_fuzzy_hash_buf, METH_VARARGS, "Hash a string buffer"},
  {NULL, NULL, 0, NULL} /* sentinel */
};


PyMODINIT_FUNC
initssdeep(void) {
  (void) Py_InitModule("ssdeep", FuzzyMethods);
}

