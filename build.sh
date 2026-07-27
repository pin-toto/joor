#!/bin/bash

echo "Building Joor OS..."

make clean
make all

if [ $? -eq 0 ]; then
    echo "Kernel built successfully!"
    echo ""
    echo "To run:"
    echo "  make run-elf"
    echo "  or: qemu-system-x86_64 -kernel build/kernel.elf -vga std -m 32"
else
    echo "Build failed!"
    exit 1
fi
