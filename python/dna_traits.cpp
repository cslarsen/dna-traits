/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <Python.h>
#include "dnatraits.hpp"
#include "genome.hpp"

static PyObject* parse(PyObject* module, PyObject* args)
{
  try {
    char *file = NULL;
    if ( !PyArg_ParseTuple(args, "s", &file) )
      return NULL;

    auto pygenome = Genome_new(&GenomeType, NULL, NULL);
    parse_file(file, *reinterpret_cast<PyGenome*>(pygenome)->genome);
    return pygenome;
  }
  catch ( const std::exception& e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}

static PyObject* new_empty(PyObject* module, PyObject* args)
{
  return Genome_new(&GenomeType, NULL, NULL);
}

static PyMethodDef methods[] = {
  {"parse", parse, METH_VARARGS,
   "Parses a 23andMe genome text file and returns a dict of RSID->GENOTYPE."},
  {"new_genome", new_empty, METH_VARARGS,
    "Returns a new, empty Genome."},
  {NULL, NULL}
};

extern "C"
void init_dna_traits()
{
  // Initialize custom python types
  GenomeType.tp_new = PyType_GenericNew;
  if ( PyType_Ready(&GenomeType) < 0 )
    return;

  auto module = Py_InitModule3("_dna_traits", methods,
                               "A fast parser for 23andMe genome files");

  Py_INCREF(&GenomeType);
  PyModule_AddObject(module, "Genome",
                     reinterpret_cast<PyObject*>(&GenomeType));
}
