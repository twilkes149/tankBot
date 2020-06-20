#!/bin/bash
echo "launched tankbot" > startup.txt
python3 webstreaming.py --ip 0.0.0.0 --port 8080
