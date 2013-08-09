#!/usr/bin/env python

import re
from sys import argv, stdout, stderr, exit
from optparse import OptionParser

# import ROOT with a fix to get batch mode (http://root.cern.ch/phpBB3/viewtopic.php?t=3198)
argv.append( '-b-' )
import ROOT
ROOT.gROOT.SetBatch(True)
argv.remove( '-b-' )

parser = OptionParser(usage="usage: %prog [options] mlfitTEST8TeV_comb_0j1j_2D.root mlfit/2D/mlfittoys_8TeV.root  \nrun with --help to get list of options")
parser.add_option("-f", "--format",   dest="format", default="text", type="string",  help="Output format ('text', 'latex'")
parser.add_option("-b", "--blind",    dest="blind",  default=False,  action="store_true",  help="Blind the signal yields")

(options, args) = parser.parse_args()
if len(args) == 0:
    parser.print_usage()
    exit(1)

file = ROOT.TFile.Open(args[0]);
fit_s = file.Get("norm_fit_s")
fit_b = file.Get("norm_fit_b")
if fit_s == None: raise RuntimeError, "Missing fit_s in %s. Did you run MaxLikelihoodFit with --saveNorm?" % file;
if fit_b == None: raise RuntimeError, "Missing fit_b in %s. Did you run MaxLikelihoodFit with --saveNorm?" % file;

tablepostfit= {}

#print "normalizations post-fit:"
iter = fit_s.createIterator()
while True:
    norm_s = iter.Next()
    if norm_s == None: break;
    norm_b = fit_b.find(norm_s.GetName())
    m = re.match(r"n_exp\w+bin(\w+)_proc_(\w+)", norm_s.GetName());
    if m == None: raise RuntimeError, "Non-conforming object name %s" % norm_s.GetName()
    if norm_b == None: raise RuntimeError, "Missing normalization %s for background fit" % norm_s.GetName()
    #print "%-30s %-30s %7.3f %7.3f" % (m.group(1), m.group(2), norm_s.getVal(), norm_b.getVal())
    tablepostfit[m.group(1)] = tablepostfit.get(m.group(1), {})
    tablepostfit[m.group(1)][m.group(2)]=norm_s.getVal()


tableprefit= {}
#print "normalizations pre-fit:"
filepre = ROOT.TFile.Open(args[1]);
fit_s = filepre.Get("norm_fit_s")
fit_b = filepre.Get("norm_fit_b")
if fit_s == None: raise RuntimeError, "Missing fit_s in %s. Did you run MaxLikelihoodFit with --saveNorm?" % file;
if fit_b == None: raise RuntimeError, "Missing fit_b in %s. Did you run MaxLikelihoodFit with --saveNorm?" % file;

iter = fit_s.createIterator()
while True:
    norm_s = iter.Next()
    if norm_s == None: break;
    norm_b = fit_b.find(norm_s.GetName())
    m = re.match(r"n_exp\w+bin(\w+)_proc_(\w+)", norm_s.GetName());
    if m == None: raise RuntimeError, "Non-conforming object name %s" % norm_s.GetName()
    if norm_b == None: raise RuntimeError, "Missing normalization %s for background fit" % norm_s.GetName()
    #print "%-30s %-30s %7.3f %7.3f" % (m.group(1), m.group(2), norm_s.getVal(), norm_b.getVal())
    tableprefit[m.group(1)] = tableprefit.get(m.group(1), {})
    tableprefit[m.group(1)][m.group(2)]=norm_s.getVal()

channels = ['of_0j','of_1j','sf_0j','sf_1j']
processes_of = ['bkg_qqww','bkg_ggww','bkg_top','bkg_wj','bkg_wgstar','bkg_dy','bkg_others','ggH','vbfH','wzttH']
processes_sf = ['WW','ggWW','Top','WJet','VgS','DYLL','VV','ggH','vbfH','wzttH']

if options.format == 'text':
    print "%-30s %-30s %-20s %-20s %-10s" % ('channel', 'process', 'pre-fit', 'post-fit', 'rel. shift')
elif options.format == 'latex':
    print "\\begin{tabular}{|l|r|r|r|r|} \\hline\\hline ";
    print "%-30s & %-30s & %-20s & %-20s & %-10s \\\\" % ('channel', 'process', 'pre-fit', 'post-fit', 'rel. shift')
    print "\\hline"
else:
    raise RuntimeError, "format has to be either text or html";

for ch in channels:
    if ch=='of_0j' or ch=='of_1j':
        for proc in processes_of:
            if options.format == 'text':
                if options.blind and (proc=='ggH' or proc=='vbfH' or proc=='wzttH'):
                    print "%-30s %-30s %7.3f %20.3f %20.3f" % (ch, proc, tableprefit[ch][proc], 0, 0)
                else:
                    print "%-30s %-30s %7.3f %20.3f %20.3f" % (ch, proc, tableprefit[ch][proc], tablepostfit[ch][proc], (tablepostfit[ch][proc]-tableprefit[ch][proc])/tableprefit[ch][proc])
            elif options.format == 'latex':
                chstr = ch.replace(r"_", r"\_")
                procstr = proc.replace(r"_", r"\_")
                if options.blind and (proc=='ggH' or proc=='vbfH' or proc=='wzttH'):
                    print "%-30s & %-30s & %7.3f & %20.3f & %20.3f \\\\" % (chstr, procstr, tableprefit[ch][proc], 0, 0)
                else:
                    print "%-30s & %-30s & %7.3f & %20.3f & %20.3f \\\\" % (chstr, procstr, tableprefit[ch][proc], tablepostfit[ch][proc], (tablepostfit[ch][proc]-tableprefit[ch][proc])/tableprefit[ch][proc])
    else:
        for proc in processes_sf:
            if options.format == 'text':
                print "%-30s %-30s %7.3f %20.3f %20.3f" % (ch, proc, tableprefit[ch][proc], tablepostfit[ch][proc], (tablepostfit[ch][proc]-tableprefit[ch][proc])/tableprefit[ch][proc])        
            elif options.format == 'latex':
                chstr = ch.replace(r"_", r"\_")
                procstr = proc.replace(r"_", r"\_")
                if options.blind and (proc=='ggH' or proc=='vbfH' or proc=='wzttH'):
                    print "%-30s & %-30s & %7.3f & %20.3f & %20.3f \\\\" % (chstr, procstr, tableprefit[ch][proc], 0, 0)                
                else:
                    print "%-30s & %-30s & %7.3f & %20.3f & %20.3f \\\\" % (chstr, procstr, tableprefit[ch][proc], tablepostfit[ch][proc], (tablepostfit[ch][proc]-tableprefit[ch][proc])/tableprefit[ch][proc])
    if options.format == 'latex':
        print "\\hline"

if options.format == 'latex':
    print "\\hline"
    print "\\end{tabular}"
else:
    print 'done.'
