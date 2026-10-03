#!/usr/bin/env python3
"""
prc2bin - split a Palm OS resource database (.prc) into one file per resource.

Replacement for the historic prc-tools 'prc2bin' helper, which the original
build uses to extract the code resources of the linked application so that
regcode/encrypt can encrypt code0002/code0006 before the PRC is rebuilt.

Each resource is written as <type><id as 4 hex digits>.bin into the current
directory (or --outdir), the naming scheme build-prc expects.
"""

import argparse
import os
import struct
import sys

HEADER_SIZE = 78            # DatabaseHdrType up to and including the record list header
ENTRY_SIZE = 10             # type (4) + id (2) + local offset (4)


def read_resources(data):
    """Return a list of (type, id, bytes) in database order."""
    attributes = struct.unpack_from('>H', data, 32)[0]
    if not attributes & 0x0001:
        raise ValueError('not a resource database (dmHdrAttrResDB not set)')

    count = struct.unpack_from('>H', data, HEADER_SIZE - 2)[0]
    entries = []
    for i in range(count):
        rtype, rid, offset = struct.unpack_from('>4sHI', data, HEADER_SIZE + i * ENTRY_SIZE)
        entries.append((rtype.decode('latin-1'), rid, offset))

    resources = []
    for i, (rtype, rid, offset) in enumerate(entries):
        end = entries[i + 1][2] if i + 1 < count else len(data)
        resources.append((rtype, rid, data[offset:end]))
    return resources


def main():
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument('prc')
    parser.add_argument('--outdir', default='.')
    args = parser.parse_args()

    with open(args.prc, 'rb') as f:
        data = f.read()

    for rtype, rid, payload in read_resources(data):
        name = os.path.join(args.outdir, '%s%04x.bin' % (rtype, rid))
        with open(name, 'wb') as out:
            out.write(payload)
        print('%s %5d %6d bytes' % (rtype, rid, len(payload)))

    return 0


if __name__ == '__main__':
    sys.exit(main())
