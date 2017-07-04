#!/usr/bin/python
from os import listdir
from os.path import isfile, join
import subprocess
import argparse

parser = argparse.ArgumentParser(description='Runs test suits.')
parser.add_argument('--mode', dest='mode', default='release',
                   help='Choose appropriate mode (debug or release)')

args = parser.parse_args()


tests = 'test/sat_20_vars'
onlyfiles = [f for f in listdir(tests) if isfile(join(tests, f))]

print('Test case sat_20_vars:')
for test in onlyfiles:
  print('Running test: %s'%test)
  if (args.mode == 'debug'):
    subprocess.call('./out/debug/CDCL %s' %(tests + '/' + test), shell=True)
  else:
    subprocess.call('./out/release/CDCL %s' %(tests + '/' + test), shell=True)


tests = 'test/sat_50_vars'
onlyfiles = [f for f in listdir(tests) if isfile(join(tests, f))]

print('Test case sat_50_vars:')
for test in onlyfiles:
  print('Running test: %s'%test)
  if (args.mode == 'debug'):
    subprocess.call('./out/debug/CDCL %s' %(tests + '/' + test), shell=True)
  else:
    subprocess.call('./out/release/CDCL %s' %(tests + '/' + test), shell=True)

tests = 'test/unsat_50_vars'
onlyfiles = [f for f in listdir(tests) if isfile(join(tests, f))]

print('Test case unsat_50_vars:')
for test in onlyfiles:
  print('Running test: %s'%test)
  if (args.mode == 'debug'):
    subprocess.call('./out/debug/CDCL %s' %(tests + '/' + test), shell=True)
  else:
    subprocess.call('./out/release/CDCL %s' %(tests + '/' + test), shell=True)

tests = 'test/graph'
onlyfiles = [f for f in listdir(tests) if isfile(join(tests, f))]

print('Test case graph:')
for test in onlyfiles:
  print('Running test: %s'%test)
  if (args.mode == 'debug'):
    subprocess.call('./out/debug/CDCL %s' %(tests + '/' + test), shell=True)
  else:
    subprocess.call('./out/release/CDCL %s' %(tests + '/' + test), shell=True)