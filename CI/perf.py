#!/usr/bin/env python3
from __future__ import print_function
import subprocess as sp
import sys
import time
from datetime import datetime
import re
import csv
import json

import psutil

def get_mem_total():
  return int(sp.check_output(["free -m | awk '/Mem:/ { print $2 }'"], shell=True).strip())
def get_mem():
  return int(sp.check_output(["free -m | awk '/Mem:/ { print $3 }'"], shell=True).strip())


def main():
  assert len(sys.argv) >= 2
  dbf = sys.argv[1]

  with open(dbf) as fh:
    db = json.load(fh)

  #  print(db)
  with open("build_timing.csv", "w") as fh:
    writer = csv.writer(fh)
    writer.writerow(["file", "memory", "time"])

    for item in db:
      cmd = item["command"]
      file = item["file"]
      cwd = item["directory"]
      p = psutil.Popen(cmd, shell=True, stdout=sp.PIPE, stderr=sp.STDOUT, cwd=cwd)

      maxmem = 0
      start = datetime.now()
      while p.poll() is None:
        mem = 0
        for subp in p.children(recursive=True):
          # print(subp.name(), subp.pid)
          try:
            mem += subp.memory_info().rss
            # cpu += subp.cpu_percent(interval=None)
          except psutil.NoSuchProcess:
            # process not available anymore, ok
            pass
          except psutil.AccessDenied:
            pass

        #  print(mem)
        maxmem = max(mem, maxmem)
        time.sleep(0.1)
        sys.stdout.write(f"\r{file}: {mem*1e-9:.2f}G")
      end = datetime.now()
      delta = end - start

      print(f"\n => max: {maxmem*1e-9:.2f}G, {delta}")
      writer.writerow([file, maxmem, delta.total_seconds()])
      fh.flush()



#  def main():
  #  assert len(sys.argv) >= 2
  #  cmd = sys.argv[1]
  #  print(cmd)

  #  p = psutil.Popen(cmd, shell=True, stdout=sp.PIPE, stderr=sp.STDOUT)
  #  samples = [(datetime.now(), 0, 0)]
  #  with open('build_timing.csv', 'w+') as csvfile:
    #  writer = csv.writer(csvfile, delimiter=',')
    #  writer.writerow(["datetime", "memory", "cpu"])

    #  while p.poll() is None:
      #  # print(samples)
      #  t = (samples[-1][0] - samples[0][0])
      #  t = t.seconds + t.microseconds/1e6
      #  print("{1:%H:%M:%S} - {0:6>.2f}s - {2:>8.2f}MB - {3:>6.2f}% CPU".format(t, *samples[-1]))
      #  writer.writerow(samples[-1])
      #  # mem = get_mem()
      #  now = datetime.now()
      #  mem = 0
      #  cpu = 0
      #  # print(p.memory_info())
      #  for subp in p.children(recursive=True):
        #  # print(subp.name(), subp.pid)
        #  try:
          #  mem += subp.memory_info().rss
          #  # cpu += subp.cpu_percent(interval=None)
        #  except psutil.NoSuchProcess:
          #  # process not available anymore, ok
          #  pass
        #  # print(subp.memory_info())

      #  cpu = psutil.cpu_percent()

      #  # in KB, go to MB
      #  mem = mem / 1e6
      #  # print(mem)
      #  samples.append((now, mem, cpu))
      #  time.sleep(0.5)

    #  samples.append((datetime.now(), get_mem()))
    #  writer.writerow(samples[-1])
  #  delta = datetime.now() - samples[0][0]
  #  print("TOTAL TIME: ", delta)


if "__main__" == __name__:
  main()
