/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <Python.h>
#include <structmember.h>
#include "dnatraits.hpp"

struct PyGenome {
  PyObject_HEAD
  Genome *genome;
};

PyObject* Genome_eq(PyGenome*, PyObject*);
PyObject* Genome_first(PyGenome*);
PyObject* Genome_getitem(PyObject*, PyObject*);
PyObject* Genome_intersect_rsid(PyGenome*, PyObject*);
PyObject* Genome_intersect_snp(PyGenome*, PyObject*);
PyObject* Genome_last(PyGenome*);
PyObject* Genome_load(PyGenome*, PyObject*);
PyObject* Genome_load_factor(PyGenome*);
PyObject* Genome_new(PyTypeObject*, PyObject*, PyObject*);
PyObject* Genome_save(PyGenome*, PyObject*);
PyObject* Genome_y_chromosome(PyGenome*);
Py_ssize_t Genome_length(PyObject*);
int Genome_init(PyGenome*, PyObject*, PyObject*);
void Genome_dealloc(PyGenome* self);

extern PyMappingMethods Genome_map;
extern PyMemberDef Genome_members[];
extern PyMethodDef Genome_methods[];
extern PyTypeObject GenomeType;
