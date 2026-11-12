#!/usr/bin/env bash
set -e

echo "=== Building MiniOS ==="
make clean
make

echo ""
echo "=== Running Automated MiniOS Integration Tests (Chapters 3-7) ==="
./minios << 'EOF'
help
system
run date
submit audit_logs
workers 4
race
quit
EOF

echo ""
echo "=== Cleaning Build Artifacts ==="
make clean
echo "All MiniOS modules verified successfully!"