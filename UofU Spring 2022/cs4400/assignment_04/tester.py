#!/usr/bin/env python3

# Run the task runner application and send appropriate inputs/signals

import sys
from subprocess import Popen, PIPE, STDOUT, run
import os
import time
import signal

import argparse

parser = argparse.ArgumentParser(description='Test your taskrunner.')
parser.add_argument("-l", action='store_true', default=False, help="should task runner log to files")
parser.add_argument("-t", default="./taskrunner", help="task runner filename")
parser.add_argument("testfile")

parsed = parser.parse_args()

lines = open(parsed.testfile).readlines()

graphFile = lines[0].strip()

(piperfd, pipewfd) = os.pipe()
piperead = os.fdopen(piperfd, "rt", buffering=1)
pipewrite = os.fdopen(pipewfd, "wt", buffering=1)

args = [parsed.t]
if parsed.l:
    args.append('-l')
args.append(graphFile)

process = Popen(args, stdin=piperead, stdout=PIPE, stderr=STDOUT,
                bufsize=0)


for line in lines[1:]:
    words = line.split()
    if words[0] == 'kill':
        toWrite = "%s %s\n" % tuple(words[:2])
        pipewrite.write(toWrite)
        pipewrite.flush()
    elif words[0] == 'status':
        toWrite = "status"
        if len(words) > 1:
            toWrite += " %s" % words[1]
        toWrite += "\n"
        pipewrite.write(toWrite)
        pipewrite.flush()
    elif words[0] == 'sleep':
        time.sleep(float(words[1]))
    elif words[0] == 'ctrl-c':
        process.send_signal(signal.SIGINT)
    elif words[0] == 'pgrep':
        res = run(['pgrep', words[1]], stdin=PIPE, stdout=PIPE, stderr=STDOUT)
        if res.returncode != 1:
            print("TEST FAILED.  All processes named %s should have been terminated" % words[1])
            sys.exit(-1)
    else:
        print("unknown command")
        break
process.wait(10)
print("return:", process.returncode)
print("stdout:")
print(process.stdout.read().decode("utf-8"))
