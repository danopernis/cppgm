#!/bin/sh

DIR=$1
BINARY=$2

for t in $DIR/*.t; do
    # TODO trigraphs should be ignored when processing raw string literal
    if test `basename $t` = "200-trigraphs.t"; then
        continue
    fi

    echo -n "$t: "

    testbase=`echo $t |sed 's/\.t$//'`

    mytest="${testbase}.my"
    mytest_exit_status="${mytest}.exit_status"
    reftest="${testbase}.ref"
    reftest_exit_status="${reftest}.exit_status"

    $BINARY < $t > ${mytest}
    if test $? -eq 0; then
        echo EXIT_SUCCESS > ${mytest_exit_status}
    else
        echo EXIT_FAILURE > ${mytest_exit_status}
    fi

    mytest_exit_status_data=`cat ${mytest_exit_status}`
    reftest_exit_status_data=`cat ${reftest_exit_status}`
    if test ${reftest_exit_status_data} != ${mytest_exit_status_data}; then
        echo "FAIL"
        echo "Expected ${reftest_exit_status_data}, got ${mytest_exit_status_data}"
        exit 1
    fi

    diff $reftest $mytest 2>&1 >/dev/null
    if test EXIT_FAILURE != ${mytest_exit_status_data} -a $? -ne 0; then
        echo "FAIL"
        echo "Output does not match reference implementation, following commands might be useful:"
        echo "cat $t"
        echo "xxd $t"
        echo "diff -y $reftest $mytest"
        exit 1
    fi

    echo "PASS";
done

echo "ALL TESTS PASS"
