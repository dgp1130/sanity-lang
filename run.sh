#!/bin/bash

BIN_DIR="build/bin"
COMPILER="${BIN_DIR}/Sanity"
PROG_OBJECT_FILE="${BIN_DIR}/prog.o"
STDLIB_OBJECT_FILE="${BIN_DIR}/stdlib.o"
PROG="${BIN_DIR}/prog"

# Verify inputs.
INPUT_FILE="${1}"
if [ "${INPUT_FILE}" == "" ]; then
  echo "Must provide a file a compile."
  exit 1
fi

# Build the Sanity compiler, stored at ${COMPILER}.
cmake --build . --target Sanity

# Compile the Sanity source code to LLVM IR, then pipe it into `llc`, the LLVM compiler which converts it to assembly
# for the current architecture. Pipe that into `gcc` which will parse the assembly into object code.
"${COMPILER}" --input "${INPUT_FILE}" | llc | gcc -x assembler - -c -o "${PROG_OBJECT_FILE}"

# Compile the standard library for Sanity.
gcc -Wall -c src/stdlib/*.c -o "${STDLIB_OBJECT_FILE}"

# Link the Sanity object code against the standard library.
gcc -o "${PROG}" "${PROG_OBJECT_FILE}" "${STDLIB_OBJECT_FILE}"

# Run the final result.
"${PROG}"