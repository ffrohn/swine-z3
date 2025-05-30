#!/bin/bash

# creates a StarExec-bundle with the binary from $REPO/build

ME=`basename "$0"`

function print {
    echo
    echo "$ME: $1"
    echo
}

function check {
    EXIT_CODE=$?
    if [[ $EXIT_CODE -ne 0 ]]; then
        print "$1 failed, exit code $EXIT_CODE"
        exit $EXIT_CODE
    fi
}

BUILD=../build
BIN=swine-z3
BUNDLE=../bin
TEMPLATE=../bundle.template
OUT=../swine-z3.zip

if [[ ! -d $OUT ]]; then
    rm $OUT
fi

if [[ -d $BUNDLE ]]; then
    rm -r $BUNDLE
fi

print "building SwInE Z3..."
./build.sh
check "building SwInE Z3"
print "creating bundle..."
cp -r $TEMPLATE $BUNDLE
cp $BUILD/$BIN $BUNDLE
zip -r $OUT $BUNDLE
check "creating bundle"

print "bundle created successfully"

rm -r $BUNDLE
