#!/usr/bin/env python3

import re
import json
import click
import os
import argparse
import shlex

@click.command()
@click.argument("compilation-database")
@click.argument("file")
def main(compilation_database, file):
  with open(compilation_database) as fh:
    db = json.load(fh)

  build_dir = os.path.dirname(os.path.realpath(compilation_database))
  print(build_dir)

  file = os.path.abspath(file)
  print(file)

  include_dirs =[]

  p = argparse.ArgumentParser()
  p.add_argument("-I", action='append')

  for item in db:
    # print(item["file"])
    if file == item["file"]:
      # print(item)

      cmd = shlex.split(item["command"])
      # print(cmd)
      # includes = p.parse_known_args(cmd)[0].I
      args, _ = p.parse_known_args(cmd)
      include_dirs = args.I

      break

  include_dirs = [os.path.abspath(os.path.join(build_dir, i)) for i in include_dirs]

  # print(include_dirs)

  includes = sorted(set(parse_file(file, include_dirs)))
  for i in includes:
    print(i)

def find_file(file, include_dirs):
  for d in include_dirs:
    path = os.path.abspath(os.path.join(d, file))
    if os.path.exists(path):
      return path
  return None

def parse_file(file, include_dirs, seen = None, depth=0):
  if seen is None: seen = []
  if file in seen:
    # print("skip")
    return []
  seen.append(file)

  with open(file) as fh:
    content = fh.read()

  includes = []
  m = re.findall(r"#include ([<\"].*[\">])", content)
  # print(m)
  for inc in m:
    includes.append(inc)
    # print(depth*"  ", inc)
    # print(re.search(r"^<(.*)>$", inc))
    m2 = re.search(r"^\"(.*)\"$", inc)
    if m2:
      # print(m2.group(1))
      path = find_file(m2.group(1), include_dirs)
      if path is not None:
        # print("path", path)
        includes += parse_file(path, include_dirs, seen, depth+1)

  return includes

main()
