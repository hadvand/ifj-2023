#!/usr/bin/env bash
# Use at Merlin: ./run.sh MyProgram.swift  (both MyProgram.swift and ifj23.swift in the current directory)
# Full use: ./run.sh MyProgram.swift -suppress-warnings
swift $2 <(cat $1 ifj23.swift)

