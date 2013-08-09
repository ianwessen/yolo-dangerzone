#!/usr/bin/env python

import sys
import os
import ROOT
from ROOT import *
import HWWAnalysis.Misc.odict as odict
import hwwlimits
import optparse
import re

 
tagname = 'comb_shape'
basepath = os.getcwd()+'/significance/'

masses = [115, 120, 125, 130, 135, 140, 150, 160, 170, 180]

points = ['mean', 'observed']

def openTFile(path, option=''):
    f =  ROOT.TFile.Open(path,option)
    if not f.__nonzero__() or not f.IsOpen():
        raise NameError('File '+path+' not open')
    return f

def closeTFile(f):
    f.Close()

def getTree( file, tree ):
    t = file.Get(tree)
    if not t.__nonzero__():
        raise NameError('Tree '+str(tree)+' doesn\'t exist in '+file.GetName())
    return t

def getMedian( hist ):
    integral=0
    value=0
    norm=hist.Integral()
    for bin in range(hist.GetNbinsX()):
        if norm==0: return 0
        if integral>0.5: break
        integral += hist.GetBinContent(bin)/norm
        value = hist.GetBinCenter(bin)
    return value

def getValue(file, q, mass):

    print "mass = "+str(mass)+" for q = "+str(q)
    cut = 'limit!=0 && limit<100'
    if q is 'mean':
        cut += ' && iToy>0'
    if q is 'observed':
        cut += ' && iToy==0'
    cut += ' && mh=='+str(mass)
    tree = getTree(file,'limit')
    command = 'limit>>h'
    tree.Draw(command,cut,'goff')
    hist = gDirectory.Get("h")
    #value = hist.GetMean()
    value = getMedian(hist)
    hist.Delete()
    return value

def printTable(file, table):
    ## latex table
    print >> file, r'\documentclass[a4paper]{article}'
    print >> file, r'\begin{document}'
    print >> file, r'\pagestyle{empty}'
    print >> file, r'\begin{tabular}{|c|cc|}'
    print >> file, r'\hline'
    print >> file, r'Higgs mass [$\mathrm{GeV/c^2}$] & Observed significance & Expected Significance \\'
    print >> file, r'\hline'
    print >> file, r'\hline'
    for mass in table:
        print >> file, r' {mass} & {observed:.2f} & {mean:.2f} \\'.format(mass=mass,**(table[mass]))
    print >> file, r'\hline'
    print >> file, r'\end{tabular}'
    print >> file, r'\end{document}'  

def main():
    usage = 'usage: %prog [dir] [cmd]'
    parser = optparse.OptionParser(usage)
    parser.add_option('--twodsuffix','-t',dest='suffix',help='suffix',default='')
    parser.add_option('-y', '--year'     , dest='year'        , help='Year'                                  , default=None   , type='float'   )    
    (opt, args) = parser.parse_args()
    
    if len(args) != 1:
        parser.error('One and only one datacard tag at the time')

    tag = args[0]

    if tag not in hwwlimits.dcnames['all']:
        parser.error('Wrong tag: '+', '.join(sorted(hwwlimits.dcnames['all'])))

    tevstr='_8TeV'
    if opt.year==2011:
        tevstr='_7TeV'
    elif opt.year==20112012:
        tevstr='_78TeV'

    tagname = tag+'_shape'+opt.suffix+tevstr

    print tagname

    table = odict.OrderedDict()

    if not os.path.exists(basepath+'/higgsCombineHWW_'+tagname+'.ProfileLikelihood.root'):
        hadd = 'hadd -k '+basepath+'/higgsCombineHWW_'+tagname+'.ProfileLikelihood.root '+basepath+'/higgsCombineHWW_'+tagname+'.ProfileLikelihood.mH*.root'
        print hadd
        os.system(hadd)
    
    sample = basepath+'/higgsCombineHWW_'+tagname+'.ProfileLikelihood.root'
    
    if not opt.suffix in sample:
        return
    if not '.root' in sample:
        return
    if not tagname in sample:
        return
    print sample

    for mass in masses:
        line = {}
        for point in points:
            f = openTFile(sample)
            
            value = getValue(f,point,mass)
            line[point] = value
            closeTFile(f)
        table[mass] = line

    latex = open(basepath+'/'+tagname+'.tex','w')
    printTable(latex,table)
    latex.close()


    ## summary file
    summary = open(basepath+'/'+tagname+'.summary', 'w')
    for mass in table:
        summary.write('{mass} {observed:.3f} 99 {mean:.3f} \n'.format( mass=mass, **(table[mass]) ) )

    summary.close()

if __name__ == '__main__':
    main()







## hww-1.55fb.mH120.comb.txt
## Observed Limit: r < 3.5430
## Expected  2.5%: r < 1.2811
## Expected 16.0%: r < 1.7041
## Expected 50.0%: r < 2.3613
## Expected 84.0%: r < 3.2799
## Expected 97.5%: r < 4.3577


## hww-1.55fb.mH130.comb.txt
## Observed Limit: r < 1.7859
## Expected  2.5%: r < 0.6184
## Expected 16.0%: r < 0.8226
## Expected 50.0%: r < 1.1398
## Expected 84.0%: r < 1.5832
## Expected 97.5%: r < 2.1035
