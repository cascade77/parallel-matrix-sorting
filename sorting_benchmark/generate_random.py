#!/usr/bin/env python3
"""
Generate random integers for sorting benchmarks
Usage: python3 generate_random.py <count> > output.txt
"""

import sys
import random

if len(sys.argv) != 2:
    print("Usage: python3 generate_random.py <count>", file=sys.stderr)
    sys.exit(1)

count = int(sys.argv[1])

# Generate random integers between 0 and 1000000
for _ in range(count):
    print(random.randint(0, 1000000))
