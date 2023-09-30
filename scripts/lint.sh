#!/bin/bash

HERMES_RUN_ROOT=$1

cpplint --recursive \
"${HERMES_RUN_ROOT}/src" "${HERMES_RUN_ROOT}/include" "${HERMES_RUN_ROOT}/test"
