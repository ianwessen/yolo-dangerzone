#!/usr/bin/env - python
from math import *
from optparse import OptionParser
parser = OptionParser(usage="%prog [options] card1.txt card2.txt > card3.txt")
parser.add_option("-t", "--type", dest="type", default="lnN",    help="how to interpret uncertainties")
parser.add_option("-m", "--mode", dest="mode", default="linear", help="how to combine uncertainties")

(options, args) = parser.parse_args()
if len(args) != 2: raise RuntimeError, "must specify exactly two datacards";
f1 = [l.split() for l in open(args[0], "r")]
f2 = [l.split() for l in open(args[1], "r")]
if len(f1) != len(f2): raise RuntimeError, "different number of lines in the two files"


for i in xrange(len(f1)):
    if f1[i][0] != f2[i][0]: raise RuntimeError, "Mismatching line %d of the files" % i
    out = [ f1[i][0] ]
    # add up yield
    if options.type == "lnN":
        out += [ "%9.4f" % (float(f1[i][1]) + float(f2[i][1])) ]
        if len(f1[i]) != 3 or len(f2[i]) != 3 : raise RuntimeError, "For lnN, must have two entries"
        if options.mode == "linear":
            out += [ "%9.4f" % (float(f1[i][2]) + float(f2[i][2]))  ]
        elif options.mode == "quad":
            out += [ "%9.4f" % (hypot(float(f1[i][2]), float(f2[i][2]))) ]
        else:
            raise RuntimeError, "Unknown mode "+options.mode
    elif options.type == "gamma":
        if len(f1[i]) not in [3,4] or len(f2[i]) not in [3,4] : raise RuntimeError, "For lnN, must have two or three entries, found %d, %d" % (len(f1[i]), len(f2[i]))
        if len(f1[i]) != len(f2[i]): raise RuntimeError, "Must have the same number of columns"
        if options.mode == "linear":
            # add up events
            if f1[i][1] != f2[i][1]: raise RuntimeError, "Must have the same yield in control region"
            out += [ f1[i][1] ]
            out += [ "%8.5f" % (float(f1[i][2]) + float(f2[i][2])) ]
            if len(f1[i]) == 4:
                out += [ "%8.5f" % (float(f1[i][3]) + float(f2[i][3])) ]
            else:
                raise RuntimeError, "Unsupported mode "+options.mode+" for gamma"
        else:
            raise RuntimeError, "Unsupported type "+options.type
        print "   ".join(out)

                
