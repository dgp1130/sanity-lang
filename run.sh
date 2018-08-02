#!/bin/bash

BIN_DIR="build/bin"
COMPILER="${BIN_DIR}/Sanity"
PROG_OBJECT_FILE="${BIN_DIR}/prog.o"
STDLIB_BUILD_DIR="${BIN_DIR}/stdlib"
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
mkdir -p "${STDLIB_BUILD_DIR}"
for SOURCE_FILE in src/stdlib/*.c; do
    OBJ_FILE=$(echo "${SOURCE_FILE}" | sed "s,.*/,,g" | sed "s/\.c$/.o/g")
    gcc -Wall -c "${SOURCE_FILE}" -o "${STDLIB_BUILD_DIR}/${OBJ_FILE}"
done

# Link the Sanity object code against the standard library.
gcc -o "${PROG}" "${PROG_OBJECT_FILE}" "${STDLIB_BUILD_DIR}"/*

# Run the final result.
"${PROG}"