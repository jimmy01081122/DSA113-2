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
    for j in {01..07}; do 
        infile="${i}_public-${j}.in"
        outfile="${i}_public-${j}.out"
        myout="my_${i}_public-${j}.out"

        if [ ! -f "$infile" ] || [ ! -f "$outfile" ]; then
            echo "⚠️  Skipping Test  ${i}_public-${j}: Missing file(s)."
            continue
        fi

        ./12 < "$infile" > "$myout"

        if diff "$myout" "$outfile" > /dev/null; then
            echo "✅ Test  ${i}_public-${j} Passed"
        else
            echo "❌ Test  ${i}_public-${j} Failed (我的) (correct answer)"
            diff "${myout}" "${outfile}" | head -n 5
        fi
    done
done
