#! /bin/sh
set -e
PYTHON=$1
SOURCEDIR=$2
BINDIR=$3
PYTHONPATH=${BINDIR}/py-dnatraits:${SOURCEDIR} ${PYTHON} ${SOURCEDIR}/test/test_dna_traits.py
