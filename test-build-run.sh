#!/bin/bash
# This should build, test and tun the Kosen raytracer

# Test
cd ./test
make
./tests
testStatus=$?
if [ $testStatus != 0 ]
then
    echo "Tests are failing. Please fix these, you naughty developer, before continuing."
    exit $testStatus
else
    echo "Tests seem to be passing. Good work!"
fi
cd ..

# Build
make
buildStatus=$?
if [ $buildStatus != 0 ]
then
    echo "Seems to be a build problem. Aborting."
    exit $buildStatus
else
    echo "Kosen builds well. Good work!"
fi

# Run
./raytracing scene1.test
