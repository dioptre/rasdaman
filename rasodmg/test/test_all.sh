#!/usr/bin/ksh

runbm.sh -e 20 tmov_16.ql tmov_32.ql tmov_64.ql
runbm.sh -n 10 tsel_sliced.ql tsel_16.ql tsel_32.ql tsel_64.ql
runbm.sh -e 20 troll_sliced.ql troll_16.ql troll_32.ql troll_64.ql
runbm.sh -e 20 cmov_16.ql cmov_32.ql cmov_64.ql
runbm.sh -n 10 csel_sliced.ql csel_16.ql csel_32.ql csel_64.ql
runbm.sh -e 20 croll_sliced.ql croll_16.ql croll_32.ql croll_64.ql
runbm.sh -n 5 earth_16.ql earth_32.ql earth_64.ql small_16.ql small_32.ql small_64.ql
