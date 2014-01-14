/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "python.h"

struct Genome{
  PyObject_HEAD
  DNA* dna;
};

static void Genome_dealloc(Genome* self)
{
  delete(self->dna);
  self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}

static PyObject* Genome_new(PyTypeObject* type,
                            PyObject* args,
                            PyObject *kw)
{
  auto p = reinterpret_cast<Genome*>(type->tp_alloc(type, 0));

  if ( p != NULL )
    p->dna = new DNA(1e6);

  return reinterpret_cast<PyObject*>(p);
}

// Genome.__init__(self, *args, **kw)
static int Genome_init(Genome*, PyObject*, PyObject*)
{
  return 0;
}

static PyObject* Genome_ychromo(Genome* self)
{
  return self->dna->ychromo? Py_True : Py_False;
}

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

static PyMemberDef Genome_members[] = {
  {NULL}
};

// Genome.__len__
static Py_ssize_t Genome_length(PyObject* self)
{
  auto genome = reinterpret_cast<Genome*>(self);
  return static_cast<Py_ssize_t>(genome->dna->snp.size());
}

static PyObject* Genome_getitem(PyObject* self, PyObject* pykey)
{
  char *key = PyString_AsString(pykey);

  // require "rs" prefix
  if ( !(key[0]=='r' && key[1]=='s') ) {
    PyErr_SetString(PyExc_KeyError, "Incorrectly formatted RSID.");
    return NULL;
  }

  // require numerical suffix
  for ( auto p = key+2; *p; ++p )
    if ( !isdigit(*p) ) {
      PyErr_SetString(PyExc_KeyError, "Incorrectly formatted RSID.");
      return NULL;
    }

  uint32_t rsid = atoi(key+2);

  auto genome = reinterpret_cast<Genome*>(self);

  if ( !genome->dna->has(rsid) ) {
    PyErr_SetString(PyExc_KeyError, "No such RSID in genome");
    return NULL;
  }

  auto genotype = genome->dna->operator[](rsid);
  char buf[3] = {from_nucleotide(genotype.first),
                 from_nucleotide(genotype.second), 0};

  return Py_BuildValue("s", buf);
}

static PyMappingMethods Genome_map = {
  Genome_length,
  Genome_getitem,
  NULL // setitem
};

static PyMethodDef Genome_methods[] = {
  // TODO: make ychromo a property
  {"ychromo", (PyCFunction)Genome_ychromo, METH_NOARGS,
   "Returns True if genome contains a Y-chromosome."},
  {NULL}
};

static PyTypeObject GenomeType = {
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
