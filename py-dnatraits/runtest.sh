#! /bin/sh
set -e
PYTHON=$1
SOURCEDIR=$2
BINDIR=$3

# Hack for OSX: Ignore PYTHONEXECUTABLE, assume it was compiledusing the system
# Python
#PYTHONPATH=${BINDIR}/py-dnatraits:${SOURCEDIR} ${PYTHON} ${SOURCEDIR}/test/test_dna_traits.py
PYTHONPATH=${BINDIR}/py-dnatraits:${SOURCEDIR} /usr/bin/python ${SOURCEDIR}/test/test_dna_traits.py
