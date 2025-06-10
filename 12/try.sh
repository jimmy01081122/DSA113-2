#!/bin/bash

# Step 1: Compile
echo "🔧 Compiling 12.c ..."
gcc 12.c -o 12 -static -O2 -std=c11
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed."
    exit 1
fi
echo "✅ Compilation succeeded."

# Step 2: Run tests
echo "🚀 Running public testcases..."

for i in {1..5}; do
        infile="sample${i}-01.in"
        outfile="sample${i}-01.out"
        myout="my_sample${i}-01.out"

        if [ ! -f "$infile" ] || [ ! -f "$outfile" ]; then
            echo "⚠️  Skipping Test  sample${i}-01: Missing file(s)."
            continue
        fi

        ./12 < "$infile" > "$myout"

        if diff "$myout" "$outfile" > /dev/null; then
            echo "✅ Test sample${i}-01 Passed"
        else
            echo "❌ Test  sample${i}-01 Failed"
            diff "$myout" "$outfile" | head -n 5
        fi
    done
