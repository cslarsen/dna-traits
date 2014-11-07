/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <Python.h>
#include <structmember.h>
#include "dna.h"

struct Genome {
  PyObject_HEAD
  DNA *dna;
};

PyObject* Genome_first(Genome*);
PyObject* Genome_getitem(PyObject*, PyObject*);
PyObject* Genome_last(Genome*);
PyObject* Genome_load(Genome*, PyObject*);
PyObject* Genome_load_factor(Genome*);
PyObject* Genome_new(PyTypeObject*, PyObject*, PyObject*);
PyObject* Genome_save(Genome*, PyObject*);
PyObject* Genome_ychromo(Genome*);
Py_ssize_t Genome_length(PyObject*);
int Genome_init(Genome*, PyObject*, PyObject*);
void Genome_dealloc(Genome* self);

extern PyMappingMethods Genome_map;
extern PyMemberDef Genome_members[];
extern PyMethodDef Genome_methods[];
extern PyTypeObject GenomeType;
