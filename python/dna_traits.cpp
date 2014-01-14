/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <Python.h>
#include "dna.h"

static char from_nucleotide(const Nucleotide& n)
{
  switch ( n ) {
    case A: return 'A';
    case T: return 'T';
    case C: return 'C';
    case G: return 'G';
    default: return '-';
  }
}

static PyObject* parse(PyObject* module, PyObject* args)
{
  char *file = NULL;

  if ( !PyArg_ParseTuple(args, "s", &file) )
    return NULL;

  try {
    DNA dna(1e6);
    parse_file(file, dna);

    // copy all items to a python dictionary
    auto dict = PyDict_New();

    for ( auto e : dna.snp ) {
      const auto rsid = e.first;
      const auto genotype = e.second;

      // "AA"
      const char value[3] = {from_nucleotide(genotype.first),
                             from_nucleotide(genotype.second),
                             '\0'};
      // "rs<number>"
      char key[16];
      sprintf(key, "rs%d", rsid);

      PyDict_SetItemString(dict, key, PyString_FromString(value));
    }

    return dict;
  } catch ( const std::exception& e) {
    PyErr_SetString(PyExc_RuntimeError, e.what());
    return NULL;
  }
}

static PyMethodDef methods[] = {
  {"parse", parse, METH_VARARGS,
   "Parses a 23andMe genome text file and returns a dict of RSID->GENOTYPE."},
  {NULL, NULL}
};

extern "C"
void initdna_traits()
{
  Py_InitModule3("dna_traits", methods,
      "A fast parser for 23andMe genome files");
}
