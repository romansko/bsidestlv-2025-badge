#!/bin/bash

function fail() {
    echo "Failed to flash"
    exit 1
}

echo "Erasing device and flashing..."

make && ./minichlink -w firmware.bin flash -b || fail

echo "Waiting for device to boot..."
sleep 3
sudo chmod a+rw /dev/tty.usbmodem24F78F06A9B81 || fail

echo "Performing self-test..."
./src/tool/sanity_test.py || fail
