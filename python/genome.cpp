/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "genome.h"

PyMemberDef Genome_members[] = {
  {NULL}
};

PyMappingMethods Genome_map = {
  Genome_length,
  Genome_getitem,
  NULL // setitem
};

PyMethodDef Genome_methods[] = {
  // TODO: make ychromo a property
  {"ychromo", (PyCFunction)Genome_ychromo, METH_NOARGS,
   "Returns True if genome contains a Y-chromosome."},
  {"load_factor", (PyCFunction)Genome_load_factor, METH_NOARGS,
    "Returns the hash map's load factor."},
  {"save", (PyCFunction)Genome_save, METH_VARARGS,
    "Saves genome to binary format."},
  {"load", (PyCFunction)Genome_load, METH_VARARGS,
    "Loads genome from binary format."},
  {"first", (PyCFunction)Genome_first, METH_NOARGS,
    "Returns first RSID."},
  {"last", (PyCFunction)Genome_last, METH_NOARGS,
    "Returns last RSID."},
  {NULL}
};

PyTypeObject GenomeType = {
  PyObject_HEAD_INIT(NULL)
  0, // obsize
  "dna_traits.Genome", // tpname
  sizeof(Genome), // basicsize
  0, // itemsize
  (destructor)Genome_dealloc, // dealloc
  0, // print
  0, // getattr
  0, // setattr
  0, // tpcompare
  0, // tprepr
  0, // tp as number
  0, // tp as seq
  &Genome_map, // tp as map
  0, // tp hash
  0, // tp call
  0, // tp str
  0, // tp getattro
  0, // tp setattro
  0, // tp as buff
  Py_TPFLAGS_DEFAULT, // tpflags
  "A genome with SNPs, genotypes, etc.\r\n\r\n"
  "You can query for RSIDs by doing foo[\"rs123\"].", // docs
  0, // traverse
  0, // clear
  0, // rich compare
  0, // weaklistoffset
  0, // iter
  0, // iternext
  Genome_methods, // methods
  Genome_members, // members
  0, // getset
  0, // base
  0, // dict
  0, // descr get
  0, // descr set
  0, // dictoffset
  (initproc)Genome_init, // init
  0, // alloc
  Genome_new, // tp new
};

void Genome_dealloc(Genome* self)
{
  delete(self->dna);
  self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}

PyObject* Genome_new(PyTypeObject* type,
                     PyObject* args,
                     PyObject *kw)
{
  auto p = reinterpret_cast<Genome*>(type->tp_alloc(type, 0));

  if ( p != NULL )
    p->dna = new DNA(1e6);

  return reinterpret_cast<PyObject*>(p);
}

// Genome.__init__(self, *args, **kw)
int Genome_init(Genome*, PyObject*, PyObject*)
{
  return 0;
}

PyObject* Genome_ychromo(Genome* self)
{
  return self->dna->ychromo? Py_True : Py_False;
}

PyObject* Genome_first(Genome* self)
{
  return Py_BuildValue("I", self->dna->first);
}

PyObject* Genome_last(Genome* self)
{
  return Py_BuildValue("I", self->dna->last);
}

PyObject* Genome_load_factor(Genome* self)
{
  return Py_BuildValue("d", self->dna->snp.load_factor());
}

PyObject* Genome_load(Genome* self, PyObject* args)
{
  PyObject *fname;
  if ( !PyArg_UnpackTuple(args, "name", 1, 1, &fname) )
    return NULL;

  const char *name = PyString_AsString(fname);
  return self->dna->load(name) ? Py_True : Py_False;
}

PyObject* Genome_save(Genome* self, PyObject* args)
{
  PyObject *fname;
  if ( !PyArg_UnpackTuple(args, "name", 1, 1, &fname) )
    return NULL;

  const char *name = PyString_AsString(fname);
  return self->dna->save(name) ? Py_True : Py_False;
}

static char from_nucleotide(const Nucleotide& n)
{
  switch ( n ) {
    case A: return 'A';
    case T: return 'T';
    case C: return 'C';
    case G: return 'G';
    case D: return 'D';
    case I: return 'I';
    case NONE: return '-';
  }
}

// Genome.__len__
Py_ssize_t Genome_length(PyObject* self)
{
  auto genome = reinterpret_cast<Genome*>(self);
  return static_cast<Py_ssize_t>(genome->dna->snp.size());
}

PyObject* Genome_getitem(PyObject* self, PyObject* rsid_)
{
  if ( !PyInt_Check(rsid_) ) {
    PyErr_SetString(PyExc_ValueError, "RSIDs not a number.");
    return NULL;
  }

  const size_t rsid_sizet = PyInt_AsSsize_t(rsid_);
  const uint32_t rsid = static_cast<uint32_t>(rsid_sizet);

  if ( rsid_sizet > rsid ) {
    PyErr_SetString(PyExc_KeyError, "RSID out of range.");
    return NULL;
  }

  auto genome = reinterpret_cast<Genome*>(self);

  if ( !genome->dna->has(rsid) ) {
    char err[32];
    sprintf(err, "No rs%u in genome", rsid);
    PyErr_SetString(PyExc_KeyError, err);
    return NULL;
  }

  auto genotype = genome->dna->operator[](rsid);
  char buf[3] = {from_nucleotide(genotype.first),
                 from_nucleotide(genotype.second), 0};

  return Py_BuildValue("s", buf);
}
