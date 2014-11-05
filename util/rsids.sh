#!/bin/bash
# Print unique RSIDs in genome
awk '/^[^#i]/ {print substr($1,3);}' $* | sort
