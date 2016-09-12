/*
 * Copyright (C) 2014, 2016 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#ifndef INC_DNATRAITS_GENOME_HPP_20160903
#define INC_DNATRAITS_GENOME_HPP_20160903

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
PyObject* Genome_rsids(PyGenome*);
PyObject* Genome_save(PyGenome*, PyObject*);
PyObject* Genome_snps(PyGenome*);
PyObject* Genome_y_chromosome(PyGenome*);
Py_ssize_t Genome_length(PyObject*);
extern PyMappingMethods Genome_map;
extern PyMemberDef Genome_members[];
extern PyMethodDef Genome_methods[];
extern PyTypeObject GenomeType;
int Genome_init(PyGenome*, PyObject*, PyObject*);
void Genome_dealloc(PyGenome* self);

#endif
