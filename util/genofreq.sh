#!/bin/sh

if [ $# -eq 0 ]; then
  echo "Usage: $0 genome-file"
  echo "Print frequency of genotypes."
  exit 1
fi

awk '/^[^#]/ {print $4;}' $* | sort | uniq -c | sort -nr
